
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

	//背景。
	CCompnent::Draw(aDc);

	//颜色。
	oldTextColor = SetTextColor(aDc, iForeColor);

	//字体。
	if (iFont == NULL)
		iFont = CreateFont(aDc, iFontHeight);
	if (iFont != NULL)
	{
		oldFont = (HFONT)SelectObject(aDc, iFont);
	}

	//文字。
	if (iContent != NULL)
	{
		for (;;)
		{
			//读一段内容。
			TCHAR* begin = line;
			len = iContent->FatchLine(line);
			if (len == 0)
				break;
			if (line[len - 1] == _T('\n'))
				len--;
			if (line[len - 1] == _T('\r'))
				len--;

			//将内容一行行画到dc. 直到内容全画完或画到了屏幕底。
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

			//如果没画完，说明已画到了屏幕底，就退出循环。
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

	//恢复dc.
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
