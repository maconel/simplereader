#pragma once

#include <mzfc_inc.h>

#define BOOKMEMO_HEAD _T("bookmemo")
#define BOOKMEMO_VERSION 0x00000000

class CBookMemo
{
public:
	CBookMemo();
	virtual ~CBookMemo();

	void SetFilename(LPCTSTR aFilename);
	unsigned int GetOffset();
	void SetOffset(unsigned int aOffset);
protected:
	CMzStringW iFilename;
	FILE* iFile;
	unsigned int iOffset;

	bool Load();
	bool Save();
	bool OpenFile(LPCTSTR aMode);
	void CloseFile();

	bool SaveInt(FILE* aFile, int aValue);
	bool SaveStr(FILE* aFile, LPCTSTR aValue);
	bool LoadInt(FILE* aFile, int& aValue);
	bool LoadStr(FILE* aFile, CMzStringW& aValue);

	bool LoadMemoV0(FILE* aFile);
	bool SaveMemo(FILE* aFile);
};
