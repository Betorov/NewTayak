#include "stdafx.h"
#include "StringTokenizer.h"
#include "CodeRunner.h"
#include <string.h>

using namespace std;

struct sTokenStr
{
	eTokenID tok;
	const char *str;
};

static const sTokenStr Tokens[] =
{
	tWhile, "while",
	tIf, "if",
	tPrint, "print",
	tBegin, "{",
	tEnd, "}",
	tOr, "||",
	tAnd, "&&",
	tPlus, "+",
	tMinus, "-",
	tTimes, "*",
	tDiv, "/",
	tAssignment, "=",
	tNe, "!=",
	tLt, "<",
	tGt, ">",
	tEq, "==",
	tOpenBracket, "(",
	tCloseBracket, ")",
	tStr, ".",
	tEOF, NULL
};


CodeRunner::CodeRunner(string program) throw(SyntaxError)
{
	tokenList tl = Lexical(program);

	tokenList::iterator it = tl.begin();

	while (it->tok != tEOF)
		m_statements.push_back(StatementPtr(Statement::Create(it)));
}

void CodeRunner::Execute()
{
	for (statementList::iterator it = m_statements.begin(); it != m_statements.end(); it++)
		it->get()->Execute(m_context);
}

CodeRunner::~CodeRunner()
{
}

tokenList CodeRunner::Lexical(string sProgram)
{
	tokenList result;

	CStringTokenizer s(sProgram.c_str(), " \r\n\t");
	for (char const *sTok = s.GetNextToken(); sTok != NULL; sTok = s.GetNextToken())
	{

		bool bFound = false;
		for (const sTokenStr *pTokenStr = Tokens; pTokenStr->str != NULL && !bFound; pTokenStr++)
			if ((bFound = (strcmp(pTokenStr->str, sTok)) == 0))
				result.push_back(sToken(pTokenStr->tok, sTok));

		if (!bFound)
		{
			string ident(sTok);
			long nNumber = 0;
			if (ident == "0" || (nNumber = atol(ident.c_str())) != 0)
				result.push_back(sToken(tNumber, nNumber));
			else if (ident == "'")
				result.push_back(sToken(tStr, ident.c_str()));
			else
				result.push_back(sToken(tIdentifier, ident.c_str()));
		}

	}

	result.push_back(sToken(tEOF, ""));

	return result;
}
