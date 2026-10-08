#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>

#include <string>
#include <vector>
#include <unordered_map>

#include <iomanip>
#include <cstdlib>

#include "SafeBuffer.hpp"
#include "LogGenerator.hpp"
#include "ParseLogs.hpp"
#include "RawLogsWriter.hpp"

class ParsedLogWriter
{
private:
    std::unordered_map<std::string, logInfo> current_log_vector;
public:
    ParsedLogWriter(SafeLogMap& log_vector) {}

    void write(SafeLogMap& log_vector)
    {
        if (std::filesystem::exists("ParsedLogs.txt")) std::filesystem::remove("ParsedLogs.txt");
        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(log_vector.mtx);
                log_vector.cv.wait(lock, [&]() {return !log_vector.data.empty() || log_vector.is_done; });
                if (log_vector.data.empty() && log_vector.is_done)
                {
                    break;
                }
                current_log_vector = std::move(log_vector.data);
                log_vector.cv.notify_one();
            }

            std::ofstream parsedLogWriter("ParsedLogs.txt", std::ios::app);
            if (!parsedLogWriter.is_open()) return;
            for (const auto& [lvl, info] : current_log_vector)
            {
                int counterI = 1;
                parsedLogWriter << "\nУровень логирования: " << lvl;
                parsedLogWriter << "\n\tИнформация о нем:";
                parsedLogWriter << "\n\t[ ] Сколько раз встретился этот уровень: " << info.count;

                parsedLogWriter << "\n\t[ ] Какие комментарии содержит:";
                for (const auto& [comment, count] : info.logCommentCount)
                {
                    parsedLogWriter << "\n\t\t" << counterI++ << ")" << "Комментарий: " << comment;
                    parsedLogWriter << "\n\t\tСколько раз встретился этот комментарий: " << count;

                    const std::vector<LogDateTime>& keyDateTime = info.logCommentDateTime.at(comment);

                    for (int i = 0; i < keyDateTime.size(); i++)
                    {
                        parsedLogWriter << "\n\t\tДаты в которые появился комментарий: " << keyDateTime[i].date << " " << keyDateTime[i].time;
                    }
                    parsedLogWriter << "\n";
                }
            }
            current_log_vector.clear();
        }
    }

    ~ParsedLogWriter() {}
};