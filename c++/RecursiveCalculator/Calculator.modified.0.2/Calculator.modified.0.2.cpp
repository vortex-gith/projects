#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include <cmath>

static std::string question;
static unsigned int pos = 0; //size_t

double parseFirst(std::string& question, unsigned int& pos); //Шаблоны что бы компилятор не ругался
double parseSecond(std::string& question, unsigned int& pos);
double parseThird(std::string& question, unsigned int& pos);
double parseFourth(std::string& question, unsigned int& pos);
double parseFifth(std::string& question, unsigned int& pos);
double parseNumbers(std::string& question, unsigned int& pos);

void formated(std::string& question, unsigned int& pos)
{
	while (pos < question.size() && question[pos] == ' ')
	{
		pos++;
	}
}

double parseNumbers(std::string& question, unsigned int& pos)
{
	formated(question, pos);
	std::string numbers;
	while (pos < question.size() && (isdigit(question[pos]) || question[pos] == '.')) //
	{
		numbers += question[pos];
		pos++;
	}
	if (!numbers.empty()) return std::stod(numbers);
	else return 0;
}

double parseFifth(std::string& question, unsigned int& pos) //  числа константы
{
	double number = 0;
	if (pos < question.size())
	{
		formated(question, pos);
		if (question.substr(pos, 2) == "pi") { pos += 2; return 3.141592653589793; }
		else if (question[pos] == 'e') { pos++;    return 2.718281828459045; }
		else if (question.substr(pos, 3) == "tau") { pos += 3; return 6.283185307179586; }
		else if (question.substr(pos, 3) == "phi") { pos += 3; return 1.618033988749895; }
		else if (question.substr(pos, 3) == "inf") { pos += 3; return INFINITY; }
		else number = parseNumbers(question, pos);
	}
	return number;
}

double parseFourth(std::string& question, unsigned int& pos) // унарные, скобки, функции
{
	double number = 0;
	if (pos < question.size())
	{
		formated(question, pos);
		if (question[pos] == '(')
		{
			pos++;
			number = parseFirst(question, pos);
			if (question[pos] == ')')
			{
				if (pos < question.size()) pos++;
				return number;
			}
			else {
				std::cout << "Ошибка";  return 0;
			}
		}
		else if (question[pos] == '-')
		{
			pos++;
			return -1 * (parseFourth(question, pos));
		}
		else if (question[pos] == '+')
		{
			pos++;
			return parseFourth(question, pos);
		}
		else if (question.substr(pos, 4) == "sqrt")
		{
			pos += 4;
			return sqrt(parseFourth(question, pos));
		}
		else if (question.substr(pos, 4) == "cbrt")
		{
			pos += 4;
			return cbrt(parseFourth(question, pos));
		}
		else if (question.substr(pos, 4) == "asin")
		{
			pos += 4;
			return asin(parseFourth(question, pos));
		}
		else if (question.substr(pos, 4) == "acos")
		{
			pos += 4;
			return acos(parseFourth(question, pos));
		}
		else if (question.substr(pos, 3) == "atg")
		{
			pos += 3;
			return atan(parseFourth(question, pos));
		}
		else if (question.substr(pos, 3) == "sin")
		{
			pos += 3;
			return sin(parseFourth(question, pos));
		}
		else if (question.substr(pos, 3) == "cos")
		{
			pos += 3;
			return cos(parseFourth(question, pos));
		}
		else if (question.substr(pos, 2) == "tg")
		{
			pos += 2;
			return tan(parseFourth(question, pos));
		}
		else if (question.substr(pos, 3) == "ctg")
		{
			pos += 3;
			return 1.0 / tan(parseFourth(question, pos));
		}
		else if (question.substr(pos, 5) == "log10")
		{
			pos += 5;
			return log10(parseFourth(question, pos));
		}
		else if (question.substr(pos, 4) == "log2")
		{
			pos += 4;
			return log2(parseFourth(question, pos));
		}
		else if (question.substr(pos, 3) == "log")
		{
			pos += 3;
			return log(parseFourth(question, pos));
		}
	}
	return parseFifth(question, pos);
}


double parseThird(std::string& question, unsigned int& pos) // ^
{
	double number = parseFourth(question, pos);
	while (pos < question.size())
	{
		formated(question, pos);
		if (question[pos] == '^')
		{
			pos++;
			double expr = parseThird(question, pos);
			return pow(number, expr);
		}
		else break;
	}
	return number;
}

double parseSecond(std::string& question, unsigned int& pos) // * /
{
	double number = parseThird(question, pos);
	while (pos < question.size())
	{
		formated(question, pos);
		if (question[pos] == '*' || question[pos] == '/')
		{
			char operation = question[pos];
			pos++;
			if (operation == '*') number *= parseThird(question, pos);
			else if (operation == '/') number /= parseThird(question, pos);
		}
		else break;
	}
	return number;
}

double parseFirst(std::string& question, unsigned int& pos) // + - 
{
	double number = parseSecond(question, pos);
	while (pos < question.size())
	{
		formated(question, pos);
		if (question[pos] == '+' || question[pos] == '-')
		{
			char operation = question[pos];
			pos++;
			if (operation == '+') number += parseSecond(question, pos);
			else if (operation == '-') number -= parseSecond(question, pos);
		}
		else break;
	}
	return number;
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
	if (choice != 1 && choice != 2) { std::cout << "Ошибка!\n"; return 0; }

	switch (choice)
	{
	case 1:
	{
		std::cout << "ОПИСАНИЕ\n\n";

		std::cout << "1) Основные знаки стандартные: +, -, *, /.\n";
		std::cout << "   Для возведения в степень используется символ ^.\n\n";

		std::cout << "2) Калькулятор понимает скобки ( ).\n\n";

		std::cout << "3) Можно использовать функции: sin, cos, tg, ctg.\n";
		std::cout << "   Также есть корни sqrt, cbrt и логарифмы log(натуральный), log2, log10.\n\n";

		std::cout << "4) В калькулятор вшиты константы: pi, e, tau, phi.\n";
		std::cout << "   Их можно просто писать буквами вместо чисел.\n\n";

		std::cout << "5) Дробные числа пишутся через точку (например, 0.5).\n";
		std::cout << "   Пробелы между символами не важны.\n\n";

		std::cout << "6) Калькулятор может провести операции с\n";
		std::cout << "   большим колчисеством чисел, можете не ограничиваться.\n\n";

		std::cout << "7) В калькулятор нельзя записывать что-то\n";
		std::cout << "   кроме чисел и операционных символов.\n\n";
		break;
	}

	case 2:
	{
		std::cout << "Введите выражение...\n";
		getline(std::cin, question);
		std::cout << std::fixed << std::setprecision(6);
		if (!question.empty())
		{
			try
			{
				double result = parseFirst(question, pos);
				if (result == INFINITY)
				{
					std::cout << "Бесконечность";
					return 0;
				}
				std::cout << "Ответ: " << result;
			}
			catch (...)
			{
				std::cout << "Ошибка в выражении!";
			}
		}
	}
	}
}