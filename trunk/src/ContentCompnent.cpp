
#include "StdAfx.h"
#include "ContentCompnent.h"

CContentCompnent::CContentCompnent()
{
	iForeColor = 0;
	iFont = NULL;
	iFontHeight = 28;
}

CContentCompnent::~CContentCompnent()
{
	if (iFont != NULL)
	{
		DeleteObject(iFont);
		iFont = NULL;
	}
}

void CContentCompnent::Draw(HDC aDc)
{
	COLORREF oldTextColor;
	HFONT oldFont = NULL;
	SIZE size;
	TCHAR line[CONTENT_LEN + 1];
	int len = 0;
	int fit = 0;
	RECT rect = iRect;
	const int LINE_SPACE = 7;

	//������
	CCompnent::Draw(aDc);

	//��ɫ��
	oldTextColor = SetTextColor(aDc, iForeColor);

	//���塣
	if (iFont == NULL)
		iFont = CreateFont(aDc, iFontHeight);
	if (iFont != NULL)
	{
		oldFont = (HFONT)SelectObject(aDc, iFont);
	}

	//���֡�
	if (iContent != NULL)
	{
		for (;;)
		{
			//��һ�����ݡ�
			TCHAR* begin = line;
			len = iContent->FatchLine(line);
			if (len == 0)
				break;
			if (line[len - 1] == _T('\n'))
				len--;
			if (line[len - 1] == _T('\r'))
				len--;

			//������һ���л���dc. ֱ������ȫ����򻭵�����Ļ�ס�
			do
			{
				if (!GetTextExtentExPoint(aDc, begin, len, RECT_WIDTH(rect), &fit, NULL, &size))
					break;
				if (size.cy > RECT_HEIGHT(rect))
					break;
				if (!ExtTextOut(aDc, rect.left, rect.top, 0, &rect, begin, fit, NULL))
					break;
				begin += fit;
				len -= fit;
				rect.top += size.cy;
				rect.top += LINE_SPACE;
			}
			while (len > 0);

			//���û���꣬˵���ѻ�������Ļ�ף����˳�ѭ����
			if (len > 0)
			{
				if (begin[len] == _T('\r'))
					len++;
				if (begin[len] == _T('\n'))
					len++;

				iContent->RoolBack(len);
				break;
			}
		}
	}

	//�ָ�dc.
	SelectObject(aDc, oldFont);
	SetTextColor(aDc, oldTextColor);
}

void CContentCompnent::SetForeColor(COLORREF aColor)
{
	iForeColor = aColor;
}

void CContentCompnent::SetFontHeight(int aFontHeight)
{
	iFontHeight = aFontHeight;
}

void CContentCompnent::SetContent(CContent* aContent)
{
	iContent = aContent;
}
