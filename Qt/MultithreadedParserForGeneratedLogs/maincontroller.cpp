#include "maincontroller.h"

MainController::MainController(QObject *parent): QObject(parent)
{
    connect(&uiTimer, &QTimer::timeout, this, &MainController::onUiUpdate);
}

void MainController::onUiUpdate()
{
    if(is_done)
    {
        emit isDoneChanged();
        uiTimer.stop();
    }
}

Q_INVOKABLE void MainController::initialization(size_t log_count)
{
    this->log_count = log_count;
}

Q_INVOKABLE void MainController::start_threads()
{
   lgeneratorThread = std::thread(&LogGenerator::generate, &LGenerator, log_count, std::ref(SafeVectorForSimpleWriter));
   plogsThread = std::thread(&ParseLogs::parse, &PLogs, std::ref(SafeVectorForSimpleWriter), std::ref(log_vector), std::ref(SafeVectorForParse));
   rlwriterThread = std::thread(&RawLogsWriter::write, &RLWriter, std::ref(SafeVectorForParse), std::ref(SafeVectorForSimpleWriter));
   plwriterThread =  std::thread(&ParsedLogWriter::write, &PLWriter, std::ref(log_vector));

   uiTimer.start(30);
}

Q_INVOKABLE void MainController::stop_threads()
{

}

Q_INVOKABLE void MainController::reset_threads()
{

}

bool MainController::getIsDone() const
{
    return is_done;
}

MainController::~MainController()
{
    uiTimer.stop();

    is_done = true;

    if(lgeneratorThread.joinable()) lgeneratorThread.join();
    if(plwriterThread.joinable()) plwriterThread.join();
    if(plogsThread.joinable()) plogsThread.join();
    if(rlwriterThread.joinable()) rlwriterThread.join();
}