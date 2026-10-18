#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QQmlEngine>
#include <QObject>
#include <QString>

#include <cmath>
#include <cctype>
#include <string>
#include <stdexcept>

class Calculator: public QObject
{
    private:
        Q_OBJECT
        QML_ELEMENT

        unsigned int pos;
        std::string expression;
        double answer;

        void formated()
        {
            while (pos < expression.size() && expression[pos] == ' ')
            {
                pos++;
            }
        }

        double parseNumbers()
        {
            formated();
            std::string numbers;
            while (pos < expression.size() && (isdigit(expression[pos]) || expression[pos] == '.'))
            {
                numbers += expression[pos];
                pos++;
            }
            if (!numbers.empty()) return std::stod(numbers);
            else throw std::invalid_argument("Недопустимые символы!");
        }

        double parseFifth() //  числа константы
        {
            double number = 0;
            if (pos < expression.size())
            {
                formated();
                if (expression.substr(pos, 2) == "pi") { pos += 2; return 3.141592653589793; }
                else if (expression[pos] == 'e') { pos++;    return 2.718281828459045; }
                else if (expression.substr(pos, 3) == "tau") { pos += 3; return 6.283185307179586; }
                else if (expression.substr(pos, 3) == "phi") { pos += 3; return 1.618033988749895; }
                else if (expression.substr(pos, 3) == "inf") { pos += 3; return INFINITY; }
                else number = parseNumbers();
            }
            return number;
        }

        double parseFourth() // унарные, скобки, функции
        {
            double number = 0;
            if (pos < expression.size())
            {
                formated();
                if (expression[pos] == '(')
                {
                    pos++;
                    number = parseFirst();
                    if (expression[pos] == ')')
                    {
                        if (pos < expression.size()) pos++;
                    }
                    else
                    {
                        throw std::invalid_argument("Неверная запись!");
                    }
                }
                else if (expression[pos] == '-')
                {
                    pos++;
                    return -1 * (parseFourth());
                }
                else if (expression[pos] == '+')
                {
                    pos++;
                    return parseFourth();
                }
                else if (expression.substr(pos, 4) == "sqrt")
                {
                    pos += 4;
                    return sqrt(parseFourth());
                }
                else if (expression.substr(pos, 4) == "cbrt")
                {
                    pos += 4;
                    return cbrt(parseFourth());
                }
                else if (expression.substr(pos, 4) == "asin")
                {
                    pos += 4;
                    return asin(parseFourth());
                }
                else if (expression.substr(pos, 4) == "acos")
                {
                    pos += 4;
                    return acos(parseFourth());
                }
                else if (expression.substr(pos, 3) == "atg")
                {
                    pos += 3;
                    return atan(parseFourth());
                }
                else if (expression.substr(pos, 3) == "sin")
                {
                    pos += 3;
                    return sin(parseFourth());
                }
                else if (expression.substr(pos, 3) == "cos")
                {
                    pos += 3;
                    return cos(parseFourth());
                }
                else if (expression.substr(pos, 2) == "tg")
                {
                    pos += 2;
                    return tan(parseFourth());
                }
                else if (expression.substr(pos, 3) == "ctg")
                {
                    pos += 3;
                    return 1.0 / tan(parseFourth());
                }
                else if (expression.substr(pos, 5) == "log10")
                {
                    pos += 5;
                    return log10(parseFourth());
                }
                else if (expression.substr(pos, 4) == "log2")
                {
                    pos += 4;
                    return log2(parseFourth());
                }
                else if (expression.substr(pos, 3) == "log")
                {
                    pos += 3;
                    return log(parseFourth());
                }
                else if (expression.substr(pos, 3) == "abs")
                {
                    pos += 3;
                    return std::abs(parseFourth());
                }
                else if (expression.substr(pos, 3) == "deg")
                {
                    pos += 3;
                    return parseFourth() * (180.0 / 3.141592653589793);
                }
                else
                {
                    number = parseFifth();
                }
            }

            while (pos < expression.size())
            {
                formated();
                if(expression[pos] == '!')
                {
                    pos++;
                    if(number < 0) throw std::invalid_argument("Ошибка! Отрицательный факториал!");
                    number = std::tgamma(number + 1);
                }
                else break;
            }

            return number;
        }

        double parseThird() // ^
        {
            double number = parseFourth();

            while (pos < expression.size())
            {
                formated();
                if (expression[pos] == '^')
                {
                    pos++;
                    double expr = parseThird();
                    return pow(number, expr);
                }
                else break;
            }
            return number;
        }

        double parseSecond() // * /
        {
            double number = parseThird();
            while (pos < expression.size())
            {
                formated();
                if (expression[pos] == '*' || expression[pos] == '/')
                {
                    char operation = expression[pos];
                    pos++;
                    if (operation == '*') number *= parseThird();
                    else if (operation == '/') number /= parseThird();
                }
                else break;
            }
            return number;
        }

        double parseFirst() // + -
        {
            double number = parseSecond();
            while (pos < expression.size())
            {
                formated();
                if (expression[pos] == '+' || expression[pos] == '-')
                {
                    char operation = expression[pos];
                    pos++;
                    if (operation == '+') number += parseSecond();
                    else if (operation == '-') number -= parseSecond();
                }
                else break;
            }
            return number;
        }

    public:
        explicit Calculator(QObject *parent = nullptr): QObject(parent), pos(0), expression(""), answer(0) {}

        Q_INVOKABLE QString calculate(const QString &expression)
        {
            try
            {
                pos = 0;
                if(expression.isEmpty()) { throw std::invalid_argument(""); }
                this->expression = expression.toStdString();
                answer = parseFirst();
                formated();
                if (pos < expression.size()) { throw std::invalid_argument("Некорректные символы в конце выражения!"); }
                if (std::isinf(answer)) { throw std::invalid_argument("Бесконечность(infinity)."); }
                if (std::isnan(answer)) { throw std::invalid_argument("Недопустимый результат вычислений!"); }
                return QString::number(answer);
            }
            catch(std::exception &exception)
            {
                return QString(exception.what());
            }
            catch (...)
            {
                return QString("Ошибка!");
            }
        }
};



#endif // CALCULATOR_H
