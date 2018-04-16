#include "stdafx.h"
#include "Expressions.h"


using namespace std;

// Expression

void Expression::Expect(eTokenID eExpected, const sToken& sActual) throw (SyntaxError)
{
	if (eExpected != sActual.tok)
	{
		stringstream sErrMsg;
		sErrMsg << "��������� ���-�� ������ " << sActual.s;
		throw SyntaxError(sErrMsg.str());
	}
}

Expression::ExprPtr Expression::Parse(tokenIt& it) throw (SyntaxError)
{
	return ExprPtr(OrExp::Parse(it));
}

// OrExp

Expression::ExprPtr OrExp::Parse(tokenIt& it)
{
	OrExp* orExp = new OrExp;
	orExp->m_exprs.push_back(AndExp::Parse(it));

	while (it->tok == tOr)
	{
		it++;
		orExp->m_exprs.push_back(AndExp::Parse(it));
	}

	return orExp->m_exprs.size() == 1 ? *(orExp->m_exprs.begin()) : ExprPtr(orExp);
}

long OrExp::Evaluate(Contexts& context)
{
	long result = 0;
	for (Exprs::iterator it = m_exprs.begin(); it != m_exprs.end(); it++)
		result |= it->get()->Evaluate(context);
	return result;
}

// AndExp

Expression::ExprPtr AndExp::Parse(tokenIt& it)
{
	AndExp* andExp = new AndExp;
	andExp->m_exprs.push_back(ComparisonExp::Parse(it));

	while (it->tok == tAnd)
	{
		it++;
		andExp->m_exprs.push_back(ComparisonExp::Parse(it));
	}

	return andExp->m_exprs.size() == 1 ? *(andExp->m_exprs.begin()) : ExprPtr(andExp);
}

long AndExp::Evaluate(Contexts& context)
{
	long result = 1;
	for (Exprs::iterator it = m_exprs.begin(); it != m_exprs.end(); it++)
		result &= it->get()->Evaluate(context);
	return result;
}

// ComparisonExp

ComparisonExp::ComparisonExp(ExprPtr lhs, ExprPtr rhs, eTokenID tok)
	: m_lhs(lhs)
	, m_rhs(rhs)
	, m_tok(tok)
{
}

Expression::ExprPtr ComparisonExp::Parse(tokenIt& it)
{
	ExprPtr lhs, rhs;

	lhs = AddExp::Parse(it);

	eTokenID tok = it->tok;
	if (tok == tEq || tok == tLt || tok == tGt || tok == tNe)
	{
		it++;
		rhs = AddExp::Parse(it);
	}

	return rhs.get() == NULL ? ExprPtr(lhs) : ExprPtr(new ComparisonExp(lhs, rhs, tok));
}

long ComparisonExp::Evaluate(Contexts& context)
{
	switch (m_tok)
	{
	case tEq:
		return m_lhs->Evaluate(context) == m_rhs->Evaluate(context);
	case tGt:
		return m_lhs->Evaluate(context) > m_rhs->Evaluate(context);
	case tLt:
		return m_lhs->Evaluate(context) < m_rhs->Evaluate(context);
	case tNe:
		return m_lhs->Evaluate(context) != m_rhs->Evaluate(context);
	default:
		return -1;
	}
}

// AddExp

Expression::ExprPtr AddExp::Parse(tokenIt& it)
{
	AddExp* addExp = new AddExp;
	addExp->m_exprs.push_back(MultExp::Parse(it));

	while (it->tok == tPlus || it->tok == tMinus)
	{
		bool bAdd = it->tok == tPlus;
		addExp->m_ops.push_back(bAdd);
		it++;
		addExp->m_exprs.push_back(MultExp::Parse(it));
	}

	return addExp->m_exprs.size() == 1 ? *(addExp->m_exprs.begin()) : ExprPtr(addExp);
}

long AddExp::Evaluate(Contexts& context)
{
	Exprs::iterator it = m_exprs.begin();
	long result = it->get()->Evaluate(context);
	for (std::list<bool>::iterator opIt = m_ops.begin(); opIt != m_ops.end(); opIt++)
	{
		it++;
		if (*opIt)
			result += it->get()->Evaluate(context);
		else
			result -= it->get()->Evaluate(context);
	}
	return result;
}

// MultExp

Expression::ExprPtr MultExp::Parse(tokenIt& it)
{
	MultExp* multExp = new MultExp;
	multExp->m_exprs.push_back(Atom::Parse(it));

	while (it->tok == tTimes || it->tok == tDiv)
	{
		multExp->m_ops.push_back(it->tok);
		it++;
		multExp->m_exprs.push_back(Atom::Parse(it));
	}

	return multExp->m_exprs.size() == 1 ? *(multExp->m_exprs.begin()) : ExprPtr(multExp);
}

long MultExp::Evaluate(Contexts& context)
{
	Exprs::iterator it = m_exprs.begin();
	long result = it->get()->Evaluate(context);
	for (std::list<eTokenID>::iterator opIt = m_ops.begin(); opIt != m_ops.end(); opIt++)
	{
		it++;
		if (*opIt == tTimes)
			result *= it->get()->Evaluate(context);
		else if (*opIt == tDiv)
			result /= it->get()->Evaluate(context);
		else
			result %= it->get()->Evaluate(context);
	}
	return result;
}

// Atom

Expression::ExprPtr Atom::Parse(tokenIt& it)
{
	Atom* result;

	switch (it->tok)
	{
	case tStr:
		it++;
		result = new MString(it->s);
		for (int i = 1; i < it->s.length(); *it++);
		Expect(tStr, *it++);
		break;
	case tNumber:
		result = new Number(it->number);
		it++;
		break;
	case tIdentifier:
		result = new Identifier(it->s);
		it++;
		break;
	case tOpenBracket:
		it++;
		result = new CompoundAtom(Expression::Parse(it));
		Expect(tCloseBracket, *it++);
		break;
	default:
		throw SyntaxError(string("����������� ��������� : ") + it->s);
	}

	return ExprPtr(result);
}


std::string ComparisonExp::EvaluateStr(Contexts& context)
{
	return "";
}

std::string MultExp::EvaluateStr(Contexts& context)
{
	return "";
}

std::string CompoundAtom::EvaluateStr(Contexts& context)
{
	return "";
}

std::string Identifier::EvaluateStr(Contexts& context)
{
	return "";
}

std::string Number::EvaluateStr(Contexts& context)
{
	return "";
}

std::string OrExp::EvaluateStr(Contexts& context)
{
	return "";
}

std::string AndExp::EvaluateStr(Contexts& context)
{
	return "";
}

std::string AddExp::EvaluateStr(Contexts& context)
{
	return "";
}