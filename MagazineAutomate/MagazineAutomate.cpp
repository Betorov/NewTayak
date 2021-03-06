// MagazineAutomate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Rule.h"
#include "MagazineMachine.h"
#include "StreamReaders.h"

void showRulesRead(vector<Rule> rules) {
	vector<Rule>::iterator it = rules.begin();

	for (it; it != rules.end(); ++it) {
		it->ShowRules();		
	}
	cout << endl;
}

int main()
{
	setlocale(0, "Rus");
	std::cout << "Обработка магазинным автоматом" << std::endl;
	while (true) {
		std::cout << "Введите путь к файлу или 'q' для выхода" << std::endl;
		std::string lineSymbol;
		std::getline(std::cin, lineSymbol);
		if (lineSymbol._Equal("q"))
			break;

		vector<Rule> rules;// RulesAutomate
		StreamReaders* automatStream = new StreamReaders();//StreamFile
		MagazineMachine* build = new MagazineMachine();

		lineSymbol = "C:/test1.txt";
		automatStream->open(lineSymbol);//C:/test1.txt
		// строка для магазинного автомата
		// m/
		while ((!automatStream->eof())) {
			rules.push_back(build->Format(automatStream->readLine()));
		}
		automatStream->close();
		string magazineString = (rules.begin())->NotTerminal() + "&";
		showRulesRead(rules);

		lineSymbol.clear();
		cout << "Введите выражение" << endl;
		std::getline(std::cin, lineSymbol);
		build->Deep(lineSymbol.size() * 50);
		if (build->isCorrectString(lineSymbol, rules)) {
			cout << "Строка введена корректно" << endl;
			vector<Rule>::iterator it = rules.begin();

			for (it; it != rules.end(); ++it) {
				if (it->NotTerminal().front() == magazineString.front()) {
					vector<string>::iterator itString = it->RuleSet().begin();
					for (itString; itString != it->RuleSet().end(); ++itString) {

						string tempMagasineString = magazineString;
						tempMagasineString = tempMagasineString.substr(1, tempMagasineString.size());
						tempMagasineString += *itString;
						tempMagasineString = tempMagasineString.substr(1, tempMagasineString.size());
						tempMagasineString.push_back('&');

						build->work(rules, lineSymbol, tempMagasineString, 1, "", "");
					}
				}
			}
		}
		else
			cout << "WTF" << endl;// WTF???

		delete automatStream;
		delete build;
	}
	

	
    return 0;
}

