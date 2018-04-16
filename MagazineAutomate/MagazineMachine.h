#pragma once
#include "Rule.h"
class MagazineMachine
{
private:
	int _deep;
	string _inputString;
	string _magazineString;

	vector<string> splitString(string line, string _ch);
public:
	void Deep(int deep) {
		_deep = deep;
	}

	vector<string> P;
	vector<string> Z;

	MagazineMachine();
	~MagazineMachine();

	void work(vector<Rule> rules, string expression, string magazineString, int channel, string recursedInput, string recursedMagazine);
	
	void showAll();

	bool isCorrectString(string input, vector<Rule> rules);

	Rule Format(string inputLine);
};

