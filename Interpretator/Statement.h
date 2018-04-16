#pragma once
#include "Token.h"
#include "SyntaxError.h"
#include "Expressions.h"
#include "Contexts.h"

class Statement
{
public:
	virtual void Execute(Contexts &context) = 0;
	static Statement *Create(tokenIt &it);
	virtual ~Statement() {};
protected:
	static void Expect(eTokenID eExpected, const sToken &sActual) throw (SyntaxError);
	static void OutputTabs(long tabLevel);
};

typedef std::shared_ptr<Statement> StatementPtr;
typedef std::list< StatementPtr > statementList;

class IfStatement : public Statement
{
public:
	IfStatement(tokenIt &it) throw (SyntaxError);
	virtual void Execute(Contexts &context);
private:
	Expression::ExprPtr m_expression;
	StatementPtr m_statement;
};

class WhileStatement : public Statement
{
public:
	WhileStatement(tokenIt &it) throw (SyntaxError);
	virtual void Execute(Contexts &context);
private:
	Expression::ExprPtr m_expression;
	StatementPtr m_statement;
};

class PrintStatement : public Statement
{
public:
	PrintStatement(tokenIt &it) throw (SyntaxError);
	virtual void Execute(Contexts &context);
private:
	Expression::ExprPtr m_expression;
};

class CompoundStatement : public Statement
{
public:
	CompoundStatement(tokenIt &it) throw (SyntaxError);
	virtual void Execute(Contexts &context);
private:
	statementList m_statements;
};

class AssignmentStatement : public Statement
{
public:
	AssignmentStatement(tokenIt &it) throw (SyntaxError);
	virtual void Execute(Contexts &context);
private:
	std::string m_variable;
	Expression::ExprPtr m_expression;
};

