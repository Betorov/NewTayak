#pragma once
class Rule
{
private:
	string _notTerminal;
	vector<string> _ruleSet;
public:
	Rule();

	string& NotTerminal() {
		return _notTerminal;
	}
	void NotTerminal(string notTerm) {
		_notTerminal = notTerm;
	}

	vector<string>& RuleSet() {
		return _ruleSet;
	}
	void RuleSet(vector<string> rule) {
		_ruleSet = rule;
	}

	void ShowRules();

	~Rule();
};

