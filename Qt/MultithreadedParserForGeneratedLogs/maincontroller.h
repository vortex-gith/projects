#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <thread>

#include <QObject>
#include <QTimer>
#include <QtQml/qqmlregistration.h>

#include "loggenerator.h"
#include "parsedlogswriter.h"
#include "parselogs.h"
#include "rawlogswriter.h"

class MainController : public QObject
{
private:
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isDone READ getIsDone NOTIFY isDoneChanged FINAL)

    size_t log_count = 0;
    bool is_done = false;

    SafeLogMap log_vector;
    std::mutex log_vector_mtx;
    std::condition_variable log_vector_cv;

    SafeBuffer SafeVectorForSimpleWriter;
    SafeBuffer SafeVectorForParse;

    LogGenerator LGenerator;
    ParseLogs PLogs;
    RawLogsWriter RLWriter;
    ParsedLogWriter PLWriter;

    std::thread lgeneratorThread;
    std::thread plogsThread;
    std::thread rlwriterThread;
    std::thread plwriterThread;

    QTimer uiTimer;
signals:
    void isDoneChanged();
public:
    explicit MainController(QObject *parent = nullptr): QObject(parent) {}

    Q_INVOKABLE void initialization(size_t log_count);

    Q_INVOKABLE void start_threads();

    Q_INVOKABLE void stop_threads();

    Q_INVOKABLE void reset_threads();

    Q_INVOKABLE bool getIsDone() const;

    ~MainController();
};

#endif // MAINCONTROLLER_H
