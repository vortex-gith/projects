#include "loggenerator.h"

LogGenerator::LogGenerator()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

std::string LogGenerator::generateSupporter(const std::string& log_type, const std::string& comment)
{
    std::time_t t = (static_cast<std::time_t>(rand()) * 1000) + 1755689600;
    std::tm now;
    localtime_s(&now, &t);

    std::string day_str = (now.tm_mday < 10 ? "0" : "") + std::to_string(now.tm_mday);
    std::string mon_str = ((now.tm_mon + 1) < 10 ? "0" : "") + std::to_string(now.tm_mon + 1);
    std::string year_str = std::to_string(now.tm_year + 1900);
    std::string hour_str = (now.tm_hour < 10 ? "0" : "") + std::to_string(now.tm_hour);
    std::string min_str = (now.tm_min < 10 ? "0" : "") + std::to_string(now.tm_min);

    return " [" + day_str + "." + mon_str + "." + year_str + "] ["
           + hour_str + ":" + min_str + "] [" + log_type + "] " + comment + "\n";
}

void LogGenerator::generate(int log_count, SafeBuffer& SafeVector, SafeBuffer& SafeVectorForSimpleWrite)
{
    std::vector<std::string> raw_vector_logs;
    std::string currentLog;

    int counter_supporter = log_count;

    for (int i = 0; i < log_count; i++)
    {
        int random_number = rand() % warning_list.size();
        int count = 0;

        count = (rand() % 3) + random_number * 3;
        std::string comment = comment_list[count];

        currentLog = generateSupporter(warning_list[random_number], comment);
        raw_vector_logs.push_back(currentLog);

        counter_supporter--;

        if (raw_vector_logs.size() == 3000)
        {
            {
                std::unique_lock<std::mutex> lock(SafeVectorForSimpleWrite.mtx);
                SafeVectorForSimpleWrite.safe_vector.push_back(std::move(raw_vector_logs));
                raw_vector_logs.clear();
            }
            SafeVectorForSimpleWrite.cv.notify_one();
        }
    }
    if (!raw_vector_logs.empty())
    {
        {
            std::unique_lock<std::mutex> lock(SafeVectorForSimpleWrite.mtx);
            SafeVectorForSimpleWrite.safe_vector.push_back(std::move(raw_vector_logs));
            raw_vector_logs.clear();
        }
        SafeVectorForSimpleWrite.cv.notify_one();
    }

    {
        std::unique_lock <std::mutex> lock(SafeVectorForSimpleWrite.mtx);
        SafeVectorForSimpleWrite.is_done = true;
    }

    SafeVectorForSimpleWrite.cv.notify_all();
}

LogGenerator::~LogGenerator() {}
