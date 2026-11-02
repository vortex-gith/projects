#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cmath>

class Calculator
{
private:
	unsigned int pos;
	std::string question;

	void formated()
	{
		while (pos < question.size() && question[pos] == ' ')
		{
			pos++;
		}
	}
	double parseNumbers()
	{
		formated();
		std::string numbers;
		while (pos < question.size() && (isdigit(question[pos]) || question[pos] == '.')) //
		{
			numbers += question[pos];
			pos++;
		}
		if (!numbers.empty()) return std::stod(numbers);
		else return 0;
	}
	double parseFifth() //  числа константы
	{
		double number = 0;
		if (pos < question.size())
		{
			formated();
			if (question.substr(pos, 2) == "pi") { pos += 2; return 3.141592653589793; }
			else if (question[pos] == 'e') { pos++;    return 2.718281828459045; }
			else if (question.substr(pos, 3) == "tau") { pos += 3; return 6.283185307179586; }
			else if (question.substr(pos, 3) == "phi") { pos += 3; return 1.618033988749895; }
			else if (question.substr(pos, 3) == "inf") { pos += 3; return INFINITY; }
			else number = parseNumbers();
		}
		return number;
	}
	double parseFourth() // унарные, скобки, функции
	{
		double number = 0;
		if (pos < question.size())
		{
			formated();
			if (question[pos] == '(')
			{
				pos++;
				number = parseFirst();
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
				return -1 * (parseFourth());
			}
			else if (question[pos] == '+')
			{
				pos++;
				return parseFourth();
			}
			else if (question.substr(pos, 4) == "sqrt")
			{
				pos += 4;
				return sqrt(parseFourth());
			}
			else if (question.substr(pos, 4) == "cbrt")
			{
				pos += 4;
				return cbrt(parseFourth());
			}
			else if (question.substr(pos, 4) == "asin")
			{
				pos += 4;
				return asin(parseFourth());
			}
			else if (question.substr(pos, 4) == "acos")
			{
				pos += 4;
				return acos(parseFourth());
			}
			else if (question.substr(pos, 3) == "atg")
			{
				pos += 3;
				return atan(parseFourth());
			}
			else if (question.substr(pos, 3) == "sin")
			{
				pos += 3;
				return sin(parseFourth());
			}
			else if (question.substr(pos, 3) == "cos")
			{
				pos += 3;
				return cos(parseFourth());
			}
			else if (question.substr(pos, 2) == "tg")
			{
				pos += 2;
				return tan(parseFourth());
			}
			else if (question.substr(pos, 3) == "ctg")
			{
				pos += 3;
				return 1.0 / tan(parseFourth());
			}
			else if (question.substr(pos, 5) == "log10")
			{
				pos += 5;
				return log10(parseFourth());
			}
			else if (question.substr(pos, 4) == "log2")
			{
				pos += 4;
				return log2(parseFourth());
			}
			else if (question.substr(pos, 3) == "log")
			{
				pos += 3;
				return log(parseFourth());
			}
		}
		return parseFifth();
	}
	double parseThird() // ^
	{
		double number = parseFourth();
		while (pos < question.size())
		{
			formated();
			if (question[pos] == '^')
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
		while (pos < question.size())
		{
			formated();
			if (question[pos] == '*' || question[pos] == '/')
			{
				char operation = question[pos];
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
		while (pos < question.size())
		{
			formated();
			if (question[pos] == '+' || question[pos] == '-')
			{
				char operation = question[pos];
				pos++;
				if (operation == '+') number += parseSecond();
				else if (operation == '-') number -= parseSecond();
			}
			else break;
		}
		return number;
	}
public:
	Calculator() : pos(0), question("") {}

	double calculate(const std::string& question)
	{
		pos = 0;
		this->question = question;
		return parseFirst();
	}
};

int main()
{
	setlocale(0, "");

	std::string question;
	Calculator calc;
	std::cout << "Введите выражение:\n";
	getline(std::cin, question);

	if (!question.empty())
	{
		try
		{
			double result = calc.calculate(question);
			std::cout << "Ответ: ";
			if (result == INFINITY) { std::cout << "Бесконечность"; }
			else std::cout << result;
		}
		catch (...)
		{
			std::cout << "Ошибка!";
			return 0;
		}

	}
	else std::cout << "Строка пустая.";
}