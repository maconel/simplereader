
#include "StdAfx.h"
#include "Content.h"
#include "toolbox.h"

CContent::CContent()
{
	ClearContent();
	iOffset = 0;
	iFile = NULL;
	iFileSize = 0;
	iFileType = FT_UNICODE;
}

CContent::~CContent()
{
	CloseFile();
}

bool CContent::OpenFile(LPCTSTR aFilename)
{
	iFilename = aFilename;

	ClearContent();
	iOffset = 0;

	iFile = _tfopen(iFilename, _T("rb"));
	if (iFile != NULL)
	{
		fseek(iFile, 0, SEEK_END);
		iFileSize = ftell(iFile);
		fseek(iFile, 0, SEEK_SET);
		CheckHead();
	}

	return (iFile != NULL);
}

bool CContent::ReOpenFile()
{
	fclose(iFile);
	iFile = NULL;
	ClearContent();
	iFile = _tfopen(iFilename, _T("rb"));
	if (iFile != NULL)
	{
		fseek(iFile, 0, SEEK_END);
		iFileSize = ftell(iFile);
		fseek(iFile, 0, SEEK_SET);
		CheckHead();
		Seek(iOffset);

		return true;
	}
	return false;
}

void CContent::CloseFile()
{
	if (iFile != NULL)
	{
		fclose(iFile);
		iFile = NULL;
		ClearContent();
		iOffset = 0;
		iFileSize = 0;
		iFilename = _T("");
	}
}

void CContent::ClearContent()
{
	iContent[0] = L'\0';
	iFirst = iContent;
	iContentLength = 0;
	iConvertRemainSize = 0;
}

void CContent::Seek(unsigned int aOffset)
{
	if (iFile != NULL)
	{
		ClearContent();

		iOffset = aOffset;
		if (iFileType == FT_UNICODE && iOffset % 2 != 0)
			iOffset += 1;
		if (iOffset < 0)
			iOffset = 0;
		if (iOffset > iFileSize)
			iOffset = iFileSize;

		fseek(iFile, iOffset, SEEK_SET);
		if (iOffset == 0)
			CheckHead();
	}
}

int CContent::Read()
{
	int ret = 0;

	if (iFile == NULL)
	{
		ret = -1;
	}
	else
	{
		//将buf装满。如果原本buf不满，就把未读内容放到buf开头，然后读满剩余空间。
		if (iFile != NULL)
		{
			memcpy(iContent, iFirst, iContentLength * 2);
			iFirst = iContent;

			switch (iFileType)
			{
			case FT_UNICODE:
				ret = ReadUnicode();
				break;
			case FT_GBK:
				ret = ReadGbk();
				break;
			case FT_UTF8:
				ret = ReadUtf8();
				break;
			default:
				ret = -1;
				break;
			}

			iContent[iContentLength] = L'\0';
		}
		else
		{
			ret = -1;
		}
	}

	return ret;
}

int CContent::ReadBack()
{
	Seek(iOffset - PREVPAGE_LEN);
	return Read();
}

int CContent::FatchLine(WCHAR* aLine)
{
	int ret = 0;
	WCHAR* p = wcschr(iFirst, L'\n');
	if (p == NULL)
	{
		p = iFirst + iContentLength;
	}
	else
	{
		p++;
	}

	wcsncpy(aLine, iFirst, p - iFirst);
	aLine[p - iFirst] = L'\0';
	ret = p - iFirst;

	iContentLength -= (p - iFirst);
	iFirst += (p - iFirst);

	return ret;
}

void CContent::RoolBack(unsigned int aLength)
{
	iFirst -= aLength;
	iContentLength += aLength;

	if (iFirst < iContent)
	{
		iContentLength -= (iContent - iFirst);
		iFirst = iContent;
	}
}

LPCTSTR CContent::GetFilename()
{
	return iFilename;
}

unsigned int CContent::GetFileSize()
{
	return iFileSize;
}

unsigned int CContent::GetOffset()
{
	return iOffset;
}

int CContent::GetContentLength()
{
	return iContentLength;
}

void CContent::CheckHead()
{
	if (iFile != NULL)
	{
		unsigned char head[3];
		int readSize = fread(&head, sizeof(char), 3, iFile);

		if (readSize > 0)
		{
			//unicode.
			if (head[0] == 0xFF && head[1] == 0xFE)
			{
				fseek(iFile, 2, SEEK_SET);
				iFileSize -= 2;
				iFileType = FT_UNICODE;
			}
			//utf8.
			else if (head[0] == 0xEF && head[1] == 0xBB && head[2] == 0xBF)
			{
				iFileSize -= 3;
				iFileType = FT_UTF8;
			}
			//gbk.
			else
			{
				fseek(iFile, 0, SEEK_SET);
				iFileType = FT_GBK;
			}
		}
	}
}

int CContent::ReadUnicode()
{
	int ret = 0;

	ret = fread(&iContent[iContentLength], 1, (CONTENT_LEN - iContentLength) * 2, iFile);
	iContentLength += ret / 2;

	iOffset = SmartTell() - iContentLength * 2;
	if (iOffset % 2 != 0)
	{
		if (fread(&iContent[iContentLength], 1, 1, iFile) > 0)
		{
			iOffset++;
			iContentLength++;
		}
	}

	return ret;
}

int CContent::ReadGbk()
{
	int ret = 0;

	ret = fread(&iConvertRemain[iConvertRemainSize], 1, READBUF_SIZE - iConvertRemainSize, iFile);
	if (ret > 0)
	{
		int remainSizeU = iContentLength;
		int remainSize = 0;
		u2a(iContent, remainSizeU, NULL, remainSize, WC_COMPOSITECHECK | WC_DEFAULTCHAR);
		iConvertRemainSize += ret;
		iOffset = SmartTell() - iConvertRemainSize - remainSize;

		int convertSize = iConvertRemainSize;
		int convertSizeU = CONTENT_LEN - iContentLength;
		bool rst = true;

		if (!a2u(iConvertRemain, convertSize, &iContent[iContentLength], convertSizeU, MB_ERR_INVALID_CHARS))
		{
			iConvertRemainSize--;
			convertSize--;
			if (!a2u(&iConvertRemain[1], convertSize, &iContent[iContentLength], convertSizeU, MB_ERR_INVALID_CHARS))
			{
				iConvertRemainSize++;
				convertSize++;
				if (!a2u(iConvertRemain, convertSize, &iContent[iContentLength], convertSizeU, 0))
				{
					rst = false;
				}
			}
		}
		if (rst)
		{
			iConvertRemainSize -= convertSize;
			iContentLength += convertSizeU;
			memcpy(iConvertRemain, &iConvertRemain[convertSize], iConvertRemainSize);
		}
		else
		{
			iConvertRemainSize = 0;
		}
	}

	return ret;
}

int CContent::ReadUtf8()
{
	int ret = 0;

	ret = fread(&iConvertRemain[iConvertRemainSize], 1, READBUF_SIZE - iConvertRemainSize, iFile);
	if (ret > 0)
	{
		int remainSizeU = iContentLength;
		int remainSize = 0;
		char* p = NULL;
		unicode_to_utf8(iContent, remainSizeU, NULL, remainSize);
		iConvertRemainSize += ret;
		iOffset = SmartTell() - iConvertRemainSize - remainSize;

		bool rst = true;

		for (p=iConvertRemain;;)
		{
			int convertSize = iConvertRemainSize;
			int convertSizeU = CONTENT_LEN - iContentLength;

			utf8_to_unicode(p, convertSize, &iContent[iContentLength], convertSizeU);
			if (convertSize == 0)
			{
				if (iConvertRemainSize > 6)
				{
					p++;
					iConvertRemainSize--;
				}
				else
				{
					break;
				}
			}

			p += convertSize;
			iConvertRemainSize -= convertSize;

			iContentLength += convertSizeU;
		}

		memcpy(iConvertRemain, p, iConvertRemainSize);
	}

	return ret;
}

long CContent::SmartTell()
{
	long ret = -1;

	ret = ftell(iFile);
	if (ret == -1)
	{
		ReOpenFile();
		ret = ftell(iFile);
	}

	return ret;
}
