#pragma once
#include "SyntaxError.h"
#include "Statement.h"
#include "Token.h"
#include "Contexts.h"

class CodeRunner
{
private:
	static tokenList Lexical(std::string sProgram);
	statementList m_statements;
	Contexts m_context;
public:
	CodeRunner(std::string program) throw(SyntaxError);
	void Execute();
	~CodeRunner();
};

