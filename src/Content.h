
#pragma once

#include <mzfc_inc.h>

#define READBUF_SIZE 2048
#define CONTENT_LEN 2048
#define PREVPAGE_LEN 300

enum FILE_TYPE
{
	FT_UNICODE = 0,
	FT_GBK = 1,
	FT_UTF8 = 2
};

class CContent
{
public:
	CContent();
	virtual ~CContent();

	bool OpenFile(LPCTSTR aFilename);
	bool ReOpenFile();
	void CloseFile();
	void ClearContent();
	void Seek(unsigned int aOffset);
	int Read();
	int ReadBack();
	int FatchLine(WCHAR* aLine);
	void RoolBack(unsigned int aLength);
	LPCTSTR GetFilename();
	unsigned int GetFileSize();
	unsigned int GetOffset();
	int GetContentLength();
protected:
	WCHAR iContent[CONTENT_LEN + 1];
	WCHAR* iFirst;
	int iContentLength;
	int iOffset;
	char iConvertRemain[READBUF_SIZE];
	int iConvertRemainSize;
	FILE* iFile;
	CMzStringW iFilename;
	unsigned int iFileSize;
	FILE_TYPE iFileType;

	void CheckHead();
	int ReadUnicode();
	int ReadGbk();
	int ReadUtf8();
	long SmartTell();
};
