// NewTayak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TextCalculate.h"
/*
http://en.cppreference.com/w/cpp/regex/regex_iterator
http://www.informit.com/articles/article.aspx?p=2064649&seqNum=9
Моя книга бъерна
*/

int main()
{
	while (true) {
		std::string lineSymbol;
		std::getline(std::cin, lineSymbol);
		if (lineSymbol._Equal("q"))
			break;

		Literal* calculate = new Literal();
		cout << calculate->createReverseText(lineSymbol) << endl;

		cout << "Result " << calculate->Calculate() << endl;

		//std::cout << lineSymbol;
		if (!lineSymbol.empty())
			lineSymbol.clear();
		delete calculate;

	}
	system("pause");
	return 0;
}

//TextCalculate* a = new TextCalculate(lineSymbol);
//string num("[0-9]+(\\.[0-9]*)?");
//string num = "[0-9]+(\\.[0-9]*)?";
//string operation("[()+*/-]");
/*string func("log");

string regs = num + "|" + operation + "|" + func;
//regex regexp(num + "|" + operation + "|" + func);
regex regexp(regs);



std::string text("(35 + 41) - log(23.4)-()");
sregex_iterator words_begin(text.begin(), text.end(), regexp);

auto words_end = std::sregex_iterator();

for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
	std::smatch match = *i;
	std::string match_str = match.str();

}
*/

