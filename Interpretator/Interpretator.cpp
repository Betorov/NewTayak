// Interpretator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CodeRunner.h"
using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	if (argc != 2)
	{
		cout << "interpret <filename>" << endl;
		return 1;
	}

	string programText;
	ifstream is(argv[1]);
	if (is)
	{
		while (!is.eof())
		{
			char c;
			is.get(c);
			programText += c;
		}
	}
	else
	{
		cout << "Невозможно открыть файл " << argv[1] << endl;
		return 1;
	}


	try
	{
		CodeRunner p(programText);
		p.Execute();

	}
	catch (SyntaxError &e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "Неожиданныя ошибка" << endl;
	}

	return 0;
}

