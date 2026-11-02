#include <iostream>
#include <filesystem>
#include <fstream>

#include <thread>
#include <atomic>
#include <mutex>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>

#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <functional> // std::ref

#include "SafeBuffer.hpp"
#include "LogGenerator.hpp"
#include "ParsedLogWriter.hpp"
#include "ParseLogs.hpp"
#include "RawLogsWriter.hpp"


int main()
{
    std::ios::sync_with_stdio(false);
    setlocale(0, "");

    size_t log_count = 0;
    bool is_done = false;

    try
    {
        std::cout << "Введите количество сгенерируемых логов: ";
        std::cin >> log_count;
    }
    catch (std::invalid_argument& error)
    {
        std::cerr << "Недопустимое значение!";
        return 0;
    }
    catch (...)
    {
        std::cerr << "Ошибка ввода.";
        return 0;
    }

    SafeLogMap log_vector;
    std::mutex log_vector_mtx;
    std::condition_variable log_vector_cv;

    SafeBuffer SafeVectorForSimpleWriter;
    SafeBuffer SafeVectorForParse;

    LogGenerator Generator;
    RawLogsWriter RawWriter;
    ParseLogs Parser;
    ParsedLogWriter ParsedWriter;

    std::thread LogGeneratorThread(&LogGenerator::generate, &Generator, log_count, std::ref(SafeVectorForSimpleWriter));
    std::thread RawLogsWriterThread(&RawLogsWriter::write, &RawWriter, std::ref(SafeVectorForParse), std::ref(SafeVectorForSimpleWriter));
    std::thread ParseLogsThread(&ParseLogs::parse, &Parser, std::ref(SafeVectorForSimpleWriter), std::ref(log_vector), std::ref(SafeVectorForParse));
    std::thread ParsedLogWriterThread(&ParsedLogWriter::write, &ParsedWriter, std::ref(log_vector));

    LogGeneratorThread.join();
    RawLogsWriterThread.join();
    ParseLogsThread.join();
    ParsedLogWriterThread.join();

    system("start RawLogs.txt");
    system("start ParsedLogs.txt");
}