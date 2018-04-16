#include "stdafx.h"
#include "Literal.h"


bool Literal::checkOperation(string oper)
{
	//()+*/-
	if (oper._Equal("+")
		||
		oper._Equal("-")) {
		if (operation.empty()) {
			operation.push(oper);
			return true;
		}
		if (this->checkPriorityLow()) {
			symbol.push(operation.top());
			operation.pop();
			operation.push(oper);
			return true;
		}
		if (this->checkPriorityHigh()) {
			symbol.push(operation.top());
			operation.pop();
			operation.push(oper);
			return true;
		}
		if (this->checkPriorityFunc()) {
			symbol.push(operation.top());
			operation.pop();
			operation.push(oper);
		}
		operation.push(oper);
		return true;
	}
	else if (
		oper._Equal("*")
		||
		oper._Equal("/")) {
		//Реализация приоритета очереди для умножить и делить
		if (operation.empty()) {
			operation.push(oper);
			return true;
		}
		if (this->checkPriorityHigh()) {
			operation.push(oper);
			return true;
		}
		if (this->checkPriorityLow()) {
			symbol.push(oper);
			return true;
		}
		if (this->checkPriorityFunc()) {
			symbol.push(operation.top());
			operation.pop();
			operation.push(oper);
		}
		operation.push(oper);
		return true;
	}
	else if(
		oper._Equal("(")
		||
		oper._Equal(")")) {
		if (oper._Equal("(")) {
			countBracket++;
			operation.push(oper);
			return true;
		}
		else {
			this->pushOperationWithBracket();			
			countBracket--;
			return true;
		}
	}
	else if (oper._Equal("log")) {
		if (operation.empty()) {
			operation.push(oper);
			return true;
		}
		operation.push(oper);
		return true;
	}
	return false;
}

Literal::Literal()
{
	countBracket = 0;
	countSymbols = 0; countOperation = 0;
	regeXp = make_shared<regex>(num + "|" + operationRules + "|" + func);
}

string Literal::createReverseText(string text)
{
	sregex_iterator words_begin(text.begin(), text.end(), *regeXp);
	auto words_end = std::sregex_iterator();
	//Нужно для проверки корректную ли строку ввели
	int NSymbolsAll = 0;
	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		std::smatch match = *i;
		std::string match_str = match.str();
		//std::cout << match_str << std::endl;
		
		if (pushReadNewSymbols(match_str))
			NSymbolsAll+=match_str.size();
	}

	//Если не соответствие операций к скобка то возвращаем некорректную строку
	if ((countSymbols != (countOperation + 1))
		||
		NSymbolsAll != text.size()
		||
		countBracket != 0) {
		return "Enter correctly line";
	}
	//Выталкиваем все символы оставшиеся
	while (!operation.empty()) {
		symbol.push(operation.top());
		operation.pop();
	}
	string textNew = "";
	operation = symbol;
	while (!operation.empty()) {
		textNew += operation.top();
		operation.pop();
	}
	return textNew;
}

double Literal::Calculate()
{
	if (symbol.empty())
		return 0.0;
	symbol = reverseStack(symbol);
	string::size_type sz;
	int N = symbol.size();
	for (int i = 0; i < N; i++) {
		if (symbol.top()._Equal("*")) {
			double right = calculateExc.top();
			calculateExc.pop();
			double left = calculateExc.top();
			calculateExc.pop();
			right = left * right;
			calculateExc.push(right);
			symbol.pop();
		}
		else if (symbol.top()._Equal("/")) {
			double right = calculateExc.top();
			calculateExc.pop();
			double left = calculateExc.top();
			calculateExc.pop();
			right = left / right;
			calculateExc.push(right);
			symbol.pop();
		}
		else if (symbol.top()._Equal("+")) {
			double right = calculateExc.top();
			calculateExc.pop();
			double left = calculateExc.top();
			calculateExc.pop();
			right = left + right;
			calculateExc.push(right);
			symbol.pop();
		}
		else if (symbol.top()._Equal("-")) {
			double right = calculateExc.top();
			calculateExc.pop();
			double left = calculateExc.top();
			calculateExc.pop();
			right = left - right;
			calculateExc.push(right);
			symbol.pop();
		}
		else if (symbol.top()._Equal("log")) {
			double argumentLog = calculateExc.top();
			calculateExc.pop();
			calculateExc.push(std::log10(argumentLog));
			symbol.pop();
		}
		else {
			calculateExc.push(
				std::stod(symbol.top(), &sz)
			);
			symbol.pop();
		}
	}
	if (calculateExc.size() == 1)
		return calculateExc.top();
	return 0;
}


Literal::~Literal()
{
	regeXp.reset();
}
