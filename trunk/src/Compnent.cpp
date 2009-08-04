#include "StdAfx.h"
#include "Compnent.h"

CCompnent::CCompnent()
{
	memset(&iRect, 0, sizeof(iRect));
	iBackColor = 0;
	iBrush = NULL;
}

CCompnent::~CCompnent()
{
	if (iBrush != NULL)
	{
		DeleteObject(iBrush);
		iBrush = NULL;
	}
}

void CCompnent::SetRect(int aLeft, int aTop, int aWidth, int aHeight)
{
	iRect.left = aLeft;
	iRect.top = aTop;
	iRect.right = iRect.left + aWidth;
	iRect.bottom = iRect.top + aHeight;
}

void CCompnent::GetRect(RECT& aRect)
{
	aRect = iRect;
}

void CCompnent::SetBackColor(COLORREF aColor)
{
	iBackColor = aColor;

	if (iBrush != NULL)
	{
		DeleteObject(iBrush);
		iBrush = NULL;
	}
	iBrush = (HBRUSH)CreateSolidBrush(iBackColor);
}

COLORREF CCompnent::GetBackColor()
{
	return iBackColor;
}

void CCompnent::Draw(HDC aDc)
{
	if (iBrush != NULL)
	{
		FillRect(aDc, &iRect, iBrush);
	}
}

HFONT CCompnent::CreateFont(HDC aDc, int aFontHeight)
{
	LOGFONT logfont;
	HFONT curFont = (HFONT)GetCurrentObject(aDc, OBJ_FONT);
	GetObject(curFont, sizeof(logfont), &logfont);
	logfont.lfHeight = aFontHeight;
	return CreateFontIndirect(&logfont);
}
