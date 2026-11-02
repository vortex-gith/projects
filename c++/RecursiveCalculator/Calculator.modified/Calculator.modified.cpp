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
	while (pos < question.size() && (isdigit(question[pos]) || question[pos] == '.'))
	{
		numbers += question[pos];
		pos++;
	}
	if (!numbers.empty()) return std::stod(numbers);
}

double parseThird(std::string& question, unsigned int& pos)
{
	while (pos < question.size())
	{
		/*formated(question, pos);
		if (question[pos] == '(')
		{
			pos++;
			std::string subquestion;
			double number;
			int index = pos;
			while (pos < question.size() && question[pos] != ')') pos++; 
			if (question[pos] == ')')
			{
				subquestion = question.substr(index, pos - index);
				unsigned int subpos = 0;
				number = parseFirst(subquestion, subpos);
				pos++;
				return number;
			}
		}
		else return parseNumbers(question, pos);*/
		formated(question, pos);
		if (question[pos] == '(')
		{
			pos++;
			double expr = parseFirst(question, pos);
			if (question[pos] == ')')
			{
				if(pos < question.size()) pos++;
				return expr;
			}

		}
		else return parseNumbers(question, pos);
	}
}

double parseSecond(std::string& question, unsigned int& pos)
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

double parseFirst(std::string& question, unsigned int& pos)
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
		else break;;
	}
	return number;
}

int main()
{
	setlocale(0, "");

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
				std::cout << "Ошибка в выражении или результат - бесконечность.";
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