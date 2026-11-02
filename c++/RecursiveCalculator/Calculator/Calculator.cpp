#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

double calculate(std::vector <char>& inpOperations, const std::vector<double>& inpNumbers)
{
    if (inpOperations.size() != inpNumbers.size() - 1) {
        std::cout << "Ошибка! Несоответствие количества операций!\n";
        return 0;
    }

    double result = inpNumbers[0];

    switch (inpOperations[0])
    {
    case '+': result += inpNumbers[1]; break;
    case '-': result -= inpNumbers[1]; break;
    case '*': result *= inpNumbers[1]; break;
    case '/': if (inpNumbers[1] != 0) result /= inpNumbers[1];
            else std::cout << "Ошибка! Делить на ноль не рекомендуется.\n"; break;
    default:
        std::cout << "Неизвестная операция!\n";
    }

    return result;
}

int main()
{
    setlocale(0, "");

    int choice;

    std::cout << "Выберите опцию\n";
    std::cout << "1)Описание продукта\n";
    std::cout << "2)Приступить к вычислениям\n";
    std::cout << "Ваш выбор: ";

    std::cin >> choice;
    std::cin.ignore();

    if (choice != 1 && choice != 2)
    {
        std::cout << "Ошибка!\n";
        return 0;
    }

    switch (choice)
    {
    case 1:
    {
        std::cout << "1)Для умножения, деления, сложения и вычитания\n";
        std::cout << "использутся следующие символы *, /, +, -.\n\n";

        std::cout << "2)Калькулятор может провести операции с\n";
        std::cout << "большим колчисеством чисел, можете не ограничиваться.\n\n";

        std::cout << "3)В калькулятор нельзя записывать что-то\n";
        std::cout << "кроме чисел и операционных символов.\n\n";

        std::cout << "4)В калькуляторе присутствует операционная\n";
        std::cout << "выборка.\n\n";
        break;
    }
    case 2:
    {
        double result = 0;
        std::vector <double> numbers, groupNumbers;

        std::string question, wordNumber;
        std::vector <std::string> inpNumbers;

        char wordOperation;
        std::vector <char> inpOperations, groupOperations;

        std::cout << "Вводите... (*, /, +, -):\n";
        std::cout << std::fixed << std::setprecision(10);

        std::getline(std::cin, question);

        if (question.empty())
        {
            std::cout << "Ошибка! Слово не содержит символов.";
            return 0;
        }

        std::string formatted;
        for (char c : question) {
            if (c == '+' || c == '-' || c == '*' || c == '/')
            {
                formatted += ' ';
                formatted += c;
                formatted += ' ';
            }
            else
            {
                formatted += c;
            }
        }

        question = formatted;

        for (int i = 0; i < question.size(); i++)
        {
            if (question[i] == ' ')
            {
                continue;
            }

            while (i <= question.size() && question[i] != ' ')
            {
                if (question[i] == '*' || question[i] == '/' || question[i] == '+' || question[i] == '-')
                {
                    wordOperation = question[i];
                    if (i + 1 > question.size() || question[i + 1] == ' ')
                    {
                        inpOperations.push_back(wordOperation);
                        break;
                    }
                }
                else
                {
                    wordNumber += question[i];
                    if (i + 1 > question.size() || question[i + 1] == ' ')
                    {
                        inpNumbers.push_back(wordNumber);
                        wordNumber.clear();
                        break;
                    }
                }
                i++;
            }
        }

        for (int i = 0; i < inpOperations.size(); i++)
        {
            if (inpOperations[i] == '*' || inpOperations[i] == '/')
            {
                if (i + 1 >= inpNumbers.size()) {
                    std::cout << "Ошибка! Не хватает чисел для операции!\n";
                    break;
                }
                groupOperations.push_back(inpOperations[i]);

                groupNumbers.insert(groupNumbers.begin(), std::stod(inpNumbers[i + 1]));
                groupNumbers.insert(groupNumbers.begin(), std::stod(inpNumbers[i]));

                result = calculate(groupOperations, groupNumbers);

                inpOperations.erase(inpOperations.begin() + i);
                inpNumbers.erase(inpNumbers.begin() + i);
                inpNumbers.erase(inpNumbers.begin() + i);
                inpNumbers.insert(inpNumbers.begin() + i, std::to_string(result));

                groupOperations.clear();
                groupNumbers.clear();
                i--;
            }


        }
        for (int i = 0; i < inpOperations.size(); i++)
        {
            if (inpOperations[i] == '+' || inpOperations[i] == '-')
            {
                groupOperations.push_back(inpOperations[i]);

                groupNumbers.push_back(std::stod(inpNumbers[i]));
                groupNumbers.push_back(std::stod(inpNumbers[i + 1]));

                result = calculate(groupOperations, groupNumbers);

                inpOperations.erase(inpOperations.begin() + i);
                inpNumbers.erase(inpNumbers.begin() + i);
                inpNumbers.erase(inpNumbers.begin() + i);
                inpNumbers.insert(inpNumbers.begin() + i, std::to_string(result));

                groupOperations.clear();
                groupNumbers.clear();
                i--;
            }
        }

        if (!inpNumbers.empty())
            std::cout << "Результат вычислений: " << inpNumbers[0];
    }
    }
}