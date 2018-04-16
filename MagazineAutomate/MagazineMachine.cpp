#include "stdafx.h"
#include "MagazineMachine.h"


vector<string> MagazineMachine::splitString(string line, string _ch)
{
	vector<string> arr;
	size_t prev = 0;
	size_t next;
	size_t delta = _ch.length();

	while ((next = line.find(_ch, prev)) != string::npos) {
		//Отладка-start
		string tmp = line.substr(prev, next - prev);		
		//Отладка-end
		arr.push_back(line.substr(prev, next - prev));
		prev = next + delta;
	}
	//Отладка-start
	string tmp = line.substr(prev);	
	//Отладка-end
	arr.push_back(line.substr(prev));

	return arr;
}

MagazineMachine::MagazineMachine()
{
	_deep = 100;
}


MagazineMachine::~MagazineMachine()
{
	if (!_inputString.empty())
		_inputString.clear();
	if (!_magazineString.empty())
		_magazineString.clear();
	if (!P.empty())
		P.clear();
	if (!Z.empty())
		Z.clear();
}

void MagazineMachine::work(vector<Rule> rules, string expression, string magazineString, int channel, string recursedInput, string recursedMagazine)
{
	try {
		if (channel > _deep)
			return;
		if ((expression._Equal("") && magazineString._Equal("&"))) {
			_inputString = recursedInput.substr(0, recursedInput.size() - 1);
			magazineString = recursedMagazine.substr(0, recursedMagazine.size() - 1);
			//magazineString = magazineString.replace("&", "h0");
			magazineString = magazineString.replace(
				magazineString.find_first_of("&", 0), 
				magazineString.find_first_of("&", 0), 
				"h0"
			);
			_magazineString = magazineString;
			
			cout << "Обрабатывает строку" << endl;
			showAll();
		}
		if (magazineString._Equal("&")) {
			return;
		}
		if (!expression.empty()) {
			if (expression.front() == magazineString.front()) {
				auto tempInputString = expression;
				auto tempMagazineString = magazineString;
				auto recursedInputString = recursedInput;
				auto recursedInputMagazine = recursedMagazine;

				tempInputString = expression.substr(1, tempInputString.size());
				tempMagazineString = magazineString.substr(1, tempMagazineString.size());

				recursedInputString += tempInputString + "|";
				recursedInputMagazine += tempMagazineString + "|";

				work(rules, tempInputString, tempMagazineString, (channel + 1), recursedInputString, recursedInputMagazine);

			}
		}
		if (isupper(magazineString.front()) != 0)
		{
			vector<Rule>::iterator itRule = rules.begin();
			for (itRule; itRule != rules.end(); ++itRule) {
				string buf; buf.push_back(magazineString.front());
				if (itRule->NotTerminal()._Equal(buf)) {
					for (string set : itRule->RuleSet()) {
						auto tempSet = magazineString;
						auto recursedInputString = recursedInput;
						auto recursedMagazineString = recursedMagazine;
						tempSet = tempSet.substr(1,tempSet.size());
						tempSet = set + tempSet;
						recursedInputString += expression + "|";
						recursedMagazineString += tempSet + "|";

						work(rules, expression, tempSet, channel + 1, recursedInputString, recursedMagazineString);
					}
				}
			}
		}
	
	}
	catch (exception ex) {
		cerr << ex.what();
	}
	
}

void MagazineMachine::showAll()
{	
	vector<string> inputStrings = splitString(_inputString, "|");
	vector<string> magazineStrings = splitString(_magazineString, "|");

	vector<string>::iterator it = inputStrings.begin();
	vector<string>::iterator itMagaz = magazineStrings.begin();
	++it;
	int i = 0;
	for (it, itMagaz; (it != inputStrings.end()) && (itMagaz != magazineStrings.end()); ++itMagaz, ++it) {
		if ((i + 1) == magazineStrings.size()) {			
			cout << "(s0," << *it << " " << "h0" << ")" << endl;
		}
		else
			cout << "(s0," << *it << ", " << *itMagaz << ")" << " -> " << endl;
		i++;
	}
}

bool MagazineMachine::isCorrectString(string input, vector<Rule> rules)
{
	int i = 0;

	for (char it : input) {
		if (isupper(it) != 0) {
			cout << "В строке есть терминальные символы" << endl;
			return false;
		}
		vector<Rule>::iterator itRule = rules.begin();
		//Start for itRule
		for (itRule; itRule != rules.end(); ++itRule) {
			Z.push_back(itRule->NotTerminal());

			vector<string>::iterator itRuleString = itRule->RuleSet().begin();
			for (itRuleString; itRuleString != itRule->RuleSet().end(); ++itRuleString) {
				for (int j = 0; j < itRuleString->size(); j++) {
					string set = *itRuleString;
					// Start if else
					char buf = ' ';
					if (i <= j)
						buf = set[i];
					else
						return false;
					//cout << set.size() << "size j:" << j << " symbol " << set[j]; //Работает магия плюсов
					if (isupper(set[i]) != 0) {
						auto flag = true;
						for (string symbol : Z) {
							if (symbol.c_str()[0] == set[j]) {
								flag = false;
								break;
							}
						}
						if (flag) {
							string push;
							push.push_back(set[j]);
							Z.push_back(push);
						}
					}
					else {
						auto flag = true;
						for (string symbol : Z) {
							if (symbol.c_str()[0] == set[j]) {
								flag = false;
								break;
							}
						}
						if (flag) {
							string push;
							push.push_back(set[j]);
							Z.push_back(push);
						}
						flag = true;
						for (string symbol : P) {
							if (symbol.c_str()[0] == set[j]) {
								flag = false;
								break;
							}
						}
						if (flag) {
							string push;
							push.push_back(set[j]);
							P.push_back(push);
						}
					}
					// End if else
					if (set[j] == it) return true;
				}
			}
		}
		// End for itRule
		i++;
	}
	cout << "Символ unknown";
	return false;
}

Rule MagazineMachine::Format(string inputLine)
{
	Rule* rule = new Rule();
	try {
		size_t transitionIndex = inputLine.find_first_of('>', 0);

		string ruleParts[2] = { inputLine.substr(0, transitionIndex), inputLine.substr(transitionIndex + 1, inputLine.size() - 1) };

		rule->NotTerminal(ruleParts[0]);
		vector<string> rules = splitString(ruleParts[1], "|");

		vector<string>::iterator it = rules.begin();
		for (it; it != rules.end(); ++it) {
			if (it->_Equal("")) {
				cout << "Wrong Rule";
				return *rule;
			}
		}

		//Delete /r
		string reBuildLastTerm = rules.back();
		rules.pop_back();
		rules.push_back(reBuildLastTerm.substr(0, reBuildLastTerm.find_first_of('\r', 0)));

		rule->RuleSet(rules);
	}
	catch (exception ex) {
		cout << ex.what();
	}	
	return *rule;
}
