#include "maincontroller.h"

Q_INVOKABLE void MainController::initialization(size_t log_count)
{
    this->log_count = log_count;
}

Q_INVOKABLE void MainController::start_threads()
{
    std::thread lgeneratorThread(&LogGenerator::generate, &LGenerator, log_count, std::ref(SafeVectorForSimpleWriter));
    std::thread plogsThread(&ParseLogs::parse, &PLogs, std::ref(SafeVectorForSimpleWriter), std::ref(log_vector), std::ref(SafeVectorForParse));
    std::thread plwriterThread(&RawLogsWriter::write, &PLWriter, std::ref(SafeVectorForParse), std::ref(SafeVectorForSimpleWriter));
    std::thread rlwriterThread(&ParsedLogWriter::write, &RLWriter, std::ref(log_vector));

    lgeneratorThread.join();
    plwriterThread.join();
    plogsThread.join();
    rlwriterThread.join();
}

Q_INVOKABLE void MainController::stop_threads()
{

}

Q_INVOKABLE void reset_threads()
{

}