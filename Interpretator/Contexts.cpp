#include "stdafx.h"
#include "Contexts.h"


using namespace std;

long Contexts::GetValue(string variable)
{
	long nResult;
	VarMap::iterator it = m_symbols.find(variable);
	if (it == m_symbols.end())
		nResult = SetValue(variable, 0);
	else
		nResult = it->second;

	return nResult;
}

long Contexts::SetValue(string variable, long value)
{
	return m_symbols[variable] = value;
}
