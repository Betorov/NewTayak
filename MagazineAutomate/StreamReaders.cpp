#include "stdafx.h"
#include "StreamReaders.h"


StreamReaders::StreamReaders()
{
}

StreamReaders::StreamReaders(string path)
{
	fileStream = new fstream(path.c_str(), ios::binary | ios::in | ios::ate);

	_sizeFile = fileStream->tellg();

	setPosition(0);
}

void StreamReaders::open(string path)
{
	if (fileStream == nullptr) {
		fileStream = new fstream(path.c_str(), ios::binary | ios::in | ios::ate);

		_sizeFile = fileStream->tellg();

		setPosition(0);
	}
}

string StreamReaders::readLine()
{
	string line;

	getline(*fileStream, line);

	_position = fileStream->tellg();

	return line;
}

char StreamReaders::readByte()
{
	char buff;

	fileStream->read(&buff, sizeof(char));
	_position = fileStream->tellg();

	return buff;
}

void StreamReaders::close()
{
	if (fileStream->is_open())
		fileStream->close();
}

StreamReaders::~StreamReaders()
{
	if (fileStream != nullptr)
		delete fileStream;
}

