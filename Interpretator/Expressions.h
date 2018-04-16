#pragma once
#include "SyntaxError.h"
#include "Token.h"
#include "Contexts.h"

class Expression
{
public:
	typedef std::shared_ptr<Expression> ExprPtr;
	typedef std::list<ExprPtr> Exprs;
	typedef std::shared_ptr<Exprs> ExprsPtr;
	static ExprPtr Parse(tokenIt &it) throw(SyntaxError);
	virtual long Evaluate(Contexts &context) = 0;
	virtual std::string EvaluateStr(Contexts &context) = 0;
	virtual ~Expression() {};
protected:
	static void Expect(eTokenID eExpected, const sToken &sActual) throw (SyntaxError);
};

class MultExp : public Expression
{
public:
	static ExprPtr Parse(tokenIt &it);
	virtual long Evaluate(Contexts &context);
	virtual std::string EvaluateStr(Contexts &context);
private:
	Exprs m_exprs;
	std::list<eTokenID> m_ops;
};

class AddExp : public Expression
{
public:
	static ExprPtr Parse(tokenIt &it);
	virtual long Evaluate(Contexts &context);
	virtual std::string EvaluateStr(Contexts &context);
private:
	Exprs m_exprs;
	std::list<bool> m_ops;
};

class ComparisonExp : public Expression
{
public:
	ComparisonExp(ExprPtr lhs, ExprPtr rhs, eTokenID tok);
	static ExprPtr Parse(tokenIt &it);
	virtual long Evaluate(Contexts &context);
	virtual std::string EvaluateStr(Contexts &context);
private:
	ExprPtr m_lhs;
	ExprPtr m_rhs;
	eTokenID m_tok;
};

class AndExp : public Expression
{
public:
	static ExprPtr Parse(tokenIt &it);
	virtual long Evaluate(Contexts &context);
	virtual std::string EvaluateStr(Contexts &context);
private:
	Exprs m_exprs;
};

class OrExp : public Expression
{
public:
	static ExprPtr Parse(tokenIt &it);
	virtual long Evaluate(Contexts &context);
	virtual std::string EvaluateStr(Contexts &context);
private:
	Exprs m_exprs;
};

class Atom : public Expression
{
public:
	static ExprPtr Parse(tokenIt &it);
};

class Number : public Atom
{
public:
	Number(long number) : m_number(number) {}
	virtual long Evaluate(Contexts &context) { return m_number; }
	virtual std::string EvaluateStr(Contexts &context);
private:
	long m_number;
};

class Identifier : public Atom
{
public:
	Identifier(std::string identifier) : m_identifier(identifier) {}
	virtual long Evaluate(Contexts &context) { return context.GetValue(m_identifier); }
	virtual std::string EvaluateStr(Contexts &context);
private:
	std::string m_identifier;
};

class CompoundAtom : public Atom
{
public:
	CompoundAtom(ExprPtr expr) : m_expression(expr) {}
	virtual long Evaluate(Contexts &context) { return m_expression->Evaluate(context); }
	virtual std::string EvaluateStr(Contexts &context);
private:
	ExprPtr m_expression;
};

class MString : public Atom
{
public:
	MString(std::string str) : str(str) {}
	virtual std::string EvaluateStr(Contexts &context) { return str; }
	virtual long Evaluate(Contexts &context) { return NULL; }
private:
	std::string str;
};
