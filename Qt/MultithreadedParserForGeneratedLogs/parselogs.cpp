#include "parselogs.h"

void ParseLogs::parse(SafeBuffer& SafeVectorForParsedWriter, SafeLogMap& log_vector, SafeBuffer& SafeVectorForParse)
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(SafeVectorForParse.mtx);
            SafeVectorForParse.cv.wait(lock, [&]() {return !SafeVectorForParse.safe_vector.empty() || SafeVectorForParse.is_done; });
            if (SafeVectorForParse.safe_vector.empty() && SafeVectorForParse.is_done)
            {
                break;
            }
            currentSafeVector.safe_vector = std::move(SafeVectorForParse.safe_vector);
        }
        SafeVectorForParse.cv.notify_one();

        for (std::vector<std::string>& vector : currentSafeVector.safe_vector)
        {
            for (std::string& inputString : vector)
            {
                size_t pos1 = inputString.find("[");
                size_t pos2 = inputString.find("]");

                std::string newInputStringDate = "";
                if (pos1 == std::string::npos || pos2 == std::string::npos) continue;
                newInputStringDate = inputString.substr(pos1, pos2 - pos1 + 1);

                size_t pos3 = inputString.find("[", pos2);
                size_t pos4 = inputString.find("]", pos3);

                std::string newInputStringTime = "";
                if (pos3 == std::string::npos || pos4 == std::string::npos) continue;
                newInputStringTime = inputString.substr(pos3, pos4 - pos3 + 1);

                size_t pos5 = inputString.find("[", pos4);
                size_t pos6 = inputString.find("]", pos5);

                std::string newInputStringLvl = "";
                if (pos5 == std::string::npos || pos6 == std::string::npos) continue;
                newInputStringLvl = inputString.substr(pos5, pos6 - pos5 + 1);

                std::string newInputStringComment = "";
                if (pos6 + 1 == std::string::npos || pos6 + 1 >= inputString.size()) continue;
                newInputStringComment = inputString.substr(pos6 + 1);

                logInfo& info = current_log_vector.data[newInputStringLvl];

                info.count++;
                info.logCommentCount[newInputStringComment]++;
                info.logCommentDateTime[newInputStringComment].push_back({ newInputStringDate, newInputStringTime });
            }
        }

        {
            std::unique_lock<std::mutex> lock(log_vector.mtx);
            log_vector.cv.wait(lock, [&]() {return log_vector.data.empty();});
            log_vector.data = std::move(current_log_vector.data);
        }
        log_vector.cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(log_vector.mtx);
        log_vector.is_done = true;
    }

    log_vector.cv.notify_one();
}