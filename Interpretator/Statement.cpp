﻿#include "stdafx.h"
#include "Statement.h"
using namespace std;

// Statement

void Statement::Expect(eTokenID eExpected, const sToken &sActual) throw (SyntaxError)
{
	if (eExpected != sActual.tok)
	{
		stringstream sErrMsg;
		sErrMsg << "��������� ���-�� ������ " << sActual.s;
		throw SyntaxError(sErrMsg.str());
	}
}

Statement *Statement::Create(tokenIt &it)
{
	switch (it->tok)
	{
	case tIf:
		return new IfStatement(it);
	case tPrint:
		return new PrintStatement(it);
	case tWhile:
		return new WhileStatement(it);
	case tBegin:
		return new CompoundStatement(it);
	case tIdentifier:
		return new AssignmentStatement(it);
	default:
		throw SyntaxError(string("�����������: ") + it->s);
	}
}

// IfStatement

IfStatement::IfStatement(tokenIt &it) throw(SyntaxError)
{
	Expect(tIf, *it++);
	m_expression = Expression::Parse(it);
	m_statement = StatementPtr(Statement::Create(it));
}

void IfStatement::Execute(Contexts &context)
{
	if (m_expression->Evaluate(context))
		m_statement->Execute(context);
}

// WhileStatement

WhileStatement::WhileStatement(tokenIt &it) throw(SyntaxError)
{
	Expect(tWhile, *it++);
	m_expression = Expression::Parse(it);
	m_statement = StatementPtr(Statement::Create(it));
}

void WhileStatement::Execute(Contexts &context)
{
	while (m_expression->Evaluate(context))
		m_statement->Execute(context);
}

// PrintStatement

PrintStatement::PrintStatement(tokenIt &it) throw(SyntaxError)
{
	Expect(tPrint, *it++);
	m_expression = Expression::Parse(it);
}

void PrintStatement::Execute(Contexts &context)
{
	cout << ((m_expression->Evaluate(context) != NULL) ? std::to_string(m_expression->Evaluate(context)) : m_expression->EvaluateStr(context)) << endl;
}

// CompoundStatement

CompoundStatement::CompoundStatement(tokenIt &it) throw(SyntaxError)
{
	Expect(tBegin, *it++);
	while (it->tok != tEnd && it->tok != tEOF)
		m_statements.push_back(StatementPtr(Statement::Create(it)));
	Expect(tEnd, *it++);
}

void CompoundStatement::Execute(Contexts &context)
{
	for (statementList::iterator it = m_statements.begin(); it != m_statements.end(); it++)
		it->get()->Execute(context);
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement(tokenIt &it) throw(SyntaxError)
{
	Expect(tIdentifier, *it);
	m_variable = it->s;
	it++;
	Expect(tAssignment, *it++);
	m_expression = Expression::Parse(it);
}

void AssignmentStatement::Execute(Contexts &context)
{
	context.SetValue(m_variable, m_expression->Evaluate(context));
}
