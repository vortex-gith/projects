#include "maincontroller.h"

Q_INVOKABLE void MainController::initialization(size_t log_count)
{
    this->log_count = log_count;
}

Q_INVOKABLE void MainController::start_threads()
{
    std::thread Generate(&LogGenerator::generate, &LGenerator, log_count, std::ref(SafeVectorForSimpleWriter));
    std::thread PutRawLogs(&RawLogsWriter::write, &PLWriter, std::ref(SafeVectorForParse), std::ref(SafeVectorForSimpleWriter));
    std::thread ParseLogs(&ParseLogs::parse, &PLogs, std::ref(SafeVectorForSimpleWriter), std::ref(log_vector), std::ref(SafeVectorForParse));
    std::thread PutParseLogs(&ParsedLogWriter::write, std::ref(RLWriter), std::ref(log_vector));

    Generate.join();
    PutRawLogs.join();
    ParseLogs.join();
    PutParseLogs.join();
}

Q_INVOKABLE void MainController::stop_threads()
{

}

Q_INVOKABLE void reset_threads()
{

}