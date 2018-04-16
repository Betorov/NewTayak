#pragma once
class StreamReaders
{
private:
	fstream* fileStream;

	int _position;
	int _sizeFile;
public:
	int getPosition() {
		return _position;
	}
	void setPosition(int position) {
		_position = position;

		if (fileStream->is_open())
			fileStream->seekg(position);
	}

	int getSizeFile() {
		return _sizeFile;
	}

	StreamReaders();
	StreamReaders(string path);

	void open(string path);
	void close();
	bool eof() {
		return fileStream->eof();
	}

	string readLine();
	char readByte();

	~StreamReaders();
};

