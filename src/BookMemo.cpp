
#include "StdAfx.h"
#include "BookMemo.h"
#include "global.h"

CBookMemo::CBookMemo()
{
	iFile = NULL;
	iOffset = 0;
}

CBookMemo::~CBookMemo()
{
	CloseFile();
}

void CBookMemo::SetFilename(LPCTSTR aFilename)
{
	//����.mem��չ�����ɡ�
	iFilename = aFilename;
	iFilename = iFilename + _T(".mem");

	//�������ݡ�
	if (!Load())
	{
		iOffset = 0;
	}
}

unsigned int CBookMemo::GetOffset()
{
	return iOffset;
}

void CBookMemo::SetOffset(unsigned int aOffset)
{
	iOffset = aOffset;

	Save();
}

bool CBookMemo::Load()
{
	//���ļ���
	if (!OpenFile(_T("rb")))
		return false;

	//ͷ��
	CMzStringW head;
	if (!LoadStr(iFile, head))
		return false;
	if (head.Compare(BOOKMEMO_HEAD) != 0)
		return false;

	//�汾�š�
	int version = 0;
	if (!LoadInt(iFile, version))
		return false;
	if (version != BOOKMEMO_VERSION)
		return false;

	//memo.
	switch (version)
	{
	case 0:
		if (!LoadMemoV0(iFile))
			return false;
		break;
	default:
		return false;
	}

	//�ر��ļ���
	CloseFile();

	return true;
}

bool CBookMemo::Save()
{
	//���ļ���
	if (!OpenFile(_T("wb")))
		return false;

	//ͷ��
	if (!SaveStr(iFile, BOOKMEMO_HEAD))
		return false;

	//�汾�š�
	if (!SaveInt(iFile, BOOKMEMO_VERSION))
		return false;

	//memo.
	if (!SaveMemo(iFile))
		return false;

	//�ر��ļ���
	CloseFile();

	return true;
}

bool CBookMemo::OpenFile(LPCTSTR aMode)
{
	CloseFile();

	iFile = _tfopen(iFilename, aMode);

	return (iFile != NULL);
}

void CBookMemo::CloseFile()
{
	if (iFile != NULL)
	{
		fclose(iFile);
		iFile = NULL;
	}
}

bool CBookMemo::SaveInt(FILE* aFile, int aValue)
{
	return (1 == fwrite(&aValue, sizeof(int), 1, aFile));
}

bool CBookMemo::SaveStr(FILE* aFile, LPCTSTR aValue)
{
	bool ret = false;
	int len = _tcslen(aValue);

	if (SaveInt(aFile, len))
		ret = (len == fwrite(aValue, sizeof(TCHAR), len, aFile));

	return ret;
}

bool CBookMemo::LoadInt(FILE* aFile, int& aValue)
{
	int ret = 0;

	return (1 == fread(&aValue, sizeof(int), 1, aFile));
}

bool CBookMemo::LoadStr(FILE* aFile, CMzStringW& aValue)
{
	bool ret = false;
	int len = 0;

	if (LoadInt(aFile, len))
	{
		aValue.SetBufferSize(len);
		ret = (len == fread(aValue, sizeof(TCHAR), len, aFile));
	}

	return ret;
}

bool CBookMemo::LoadMemoV0(FILE* aFile)
{
	//offset.
	if (!LoadInt(aFile, (int&)iOffset))
		return false;

	return true;
}

bool CBookMemo::SaveMemo(FILE* aFile)
{
	//offset.
	if (!SaveInt(aFile, iOffset))
		return false;

	return true;
}
