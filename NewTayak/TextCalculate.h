#pragma once
#include "Literal.h"
class TextCalculate
{
private:

	string text;
	string reverseText;

	shared_ptr<Literal> literals;
public:
	TextCalculate();
	TextCalculate(string text) {
		literals = make_shared<Literal>();
		this->text = text;
	}
	
	void calculate();

	~TextCalculate();
};

