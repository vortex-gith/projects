#ifndef _ALCULATOR_H
#define _ALCULATOR_H

#include <QQmlEngine>
#include <QObject>
#include <QString>

class Calculator: public QObject
{
private:
    Q_OBJECT
    QML_ELEMENT



public:
    explicit Calculator(QObject *parent = nullptr): QObject(parent) {}

    Q_INVOKABLE QString calculate(const QString &expression)
    {
        return "Ответ";
    }
};



#endif // _ALCULATOR_H
