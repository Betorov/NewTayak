#pragma once
class Literal
{
private:
	template<class T>
	stack<T> reverseStack(stack<T> stacks) {
		stack<T> reverseStack;
		while (!stacks.empty()) {
			reverseStack.push(stacks.top());
			stacks.pop();
		}
		return reverseStack;
	}

	stack<double> calculateExc;
	shared_ptr<regex> regeXp;
	string num = "[0-9]+(\\.[0-9]*)?";
	string operationRules = "[()+*/-]";
	string func = "log";

	int countBracket;
	
	bool checkOperation(string oper);
	bool checkPriority() {
		if (operation.empty())
			return false;
		if (symbol.size() < 1 || operation.top()._Equal("("))
			return false;
		if (operation.size() < 1)
			return false;
	}
	//true - если в стеке меньший приоритет 
	bool checkPriorityHigh() {
		if (!checkPriority())
			return false;
		if (operation.top()._Equal("+") || operation.top()._Equal("-")) 
			return true;
		return false;
	}
	//true - если в стеке больший приоритет 
	bool checkPriorityLow() {
		if (!checkPriority())
			return false;
		if (operation.top()._Equal("*")
			||
			operation.top()._Equal("/"))
			return true;
		return false;
	}
	bool checkPriorityFunc() {
		if (!checkPriority())
			return false;
		if (operation.top()._Equal("log"))
			return true;
		return false;
	}
	//Нужно для проверки на корректность выражения
	int countSymbols, countOperation;
	bool pushReadNewSymbols(string match_str) {
		if (!checkOperation(match_str)) {
			countSymbols++;
			symbol.push(match_str);
			return true;
		}
		else {
			if (!(match_str._Equal("(")
				|| match_str._Equal(")")
				|| match_str._Equal("log")))
				countOperation++;
			return true;
		}
		return false;
	}
	//Пушит все операции до открытия скобки
	void pushOperationWithBracket() {
		if (countBracket > 0) {
			while (!operation.top()._Equal("(")) {
				symbol.push(operation.top());
				operation.pop();
			}
			operation.pop();
		}
	}
	//Проверяет не является ли последним элементом скобка
	stack<string> symbol;
	stack<string> operation;
public:
	Literal();

	string createReverseText(string text);

	double Calculate();

	~Literal();
};

