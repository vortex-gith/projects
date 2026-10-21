#ifndef RAWLOGSWRITER_H
#define RAWLOGSWRITER_H

#include <utility>
#include <mutex>

#include "safebuffer.h"

class RawLogsWriter
{
private:
    SafeBuffer currentSafeVector;
public:
    RawLogsWriter(SafeBuffer& SafeVectorForSimpleWriter, SafeBuffer& SafeVectorForParse) {}

    void write(SafeBuffer& SafeVectorForParse, SafeBuffer& SafeVectorForSimpleWriter)
    {
        if (std::filesystem::exists("RawLogs.txt")) std::filesystem::remove("RawLogs.txt");
        std::ofstream rawLogWriter("RawLogs.txt", std::ios::app);
        if (!rawLogWriter.is_open()) return;
        while (true)
        {
            std::unique_lock<std::mutex> lock(SafeVectorForSimpleWriter.mtx);

            SafeVectorForSimpleWriter.cv.wait(lock, [&]() {return !SafeVectorForSimpleWriter.safe_vector.empty() || SafeVectorForSimpleWriter.is_done; });
            if (SafeVectorForSimpleWriter.safe_vector.empty() && SafeVectorForSimpleWriter.is_done)
            {
                break;
            }
            currentSafeVector.safe_vector = std::move(SafeVectorForSimpleWriter.safe_vector);
            for (std::vector<std::string>& vector : currentSafeVector.safe_vector)
            {
                for (std::string& string : vector)
                {
                    rawLogWriter << string;
                }
            }

            {
                std::unique_lock<std::mutex> lock(SafeVectorForParse.mtx);
                SafeVectorForParse.cv.wait(lock, [&]() {return SafeVectorForParse.safe_vector.empty(); });
                SafeVectorForParse.safe_vector = std::move(currentSafeVector.safe_vector);
                SafeVectorForParse.cv.notify_one();
            }

            if (SafeVectorForSimpleWriter.is_done && SafeVectorForSimpleWriter.safe_vector.empty())
            {
                std::lock_guard<std::mutex> lock(SafeVectorForParse.mtx);
                SafeVectorForParse.is_done = true;
                SafeVectorForParse.cv.notify_one();
            }
        }
    }

    ~RawLogsWriter() {}
};

#endif // RAWLOGSWRITER_H
