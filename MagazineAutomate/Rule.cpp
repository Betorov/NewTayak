#include "stdafx.h"
#include "Rule.h"


Rule::Rule()
{
}


void Rule::ShowRules()
{
	if (_ruleSet.empty() || _notTerminal.empty())
		return;

	string result;

	vector<string>::iterator it = _ruleSet.begin();
	for (it; it != _ruleSet.end(); ++it) {
		result += (*it + "|");
	}

	cout << "__ this:"<< _notTerminal.c_str() << ">" << result.c_str() << endl;
}

Rule::~Rule()
{
	if (!_ruleSet.empty())
		_ruleSet.clear();
	if (!_notTerminal.empty())
		_notTerminal.clear();
}
