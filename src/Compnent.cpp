#include "StdAfx.h"
#include "Compnent.h"

CCompnent::CCompnent()
{
	memset(&iRect, 0, sizeof(iRect));
	iBackColor = 0;
	iBrush = NULL;
	iPen = NULL;
}

CCompnent::~CCompnent()
{
	if (iBrush != NULL)
	{
		DeleteObject(iBrush);
		iBrush = NULL;
	}

	if (iPen != NULL)
	{
		DeleteObject(iPen);
		iPen = NULL;
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

void CCompnent::SetBorderColor(COLORREF aColor)
{
	iBorderColor = aColor;
	
	if (iPen != NULL)
	{
		DeleteObject(iPen);
		iPen = NULL;
	}
	iPen = CreatePen(PS_SOLID, 1, iBorderColor);
}

COLORREF CCompnent::GetBackColor()
{
	return iBackColor;
}

COLORREF CCompnent::GetBorderColor()
{
	return iBorderColor;
}

void CCompnent::Draw(HDC aDc)
{
	if (iBrush != NULL)
	{
		FillRect(aDc, &iRect, iBrush);
	}
}

void CCompnent::DrawBorder(HDC aDc)
{
	if (iPen != NULL)
	{
		POINT points[5];
		HPEN oldPen = (HPEN)SelectObject(aDc, iPen);

		points[0].x = iRect.left;
		points[0].y = iRect.top;
		points[1].x = iRect.right;
		points[1].y = iRect.top;
		points[2].x = iRect.right;
		points[2].y = iRect.bottom;
		points[3].x = iRect.left;
		points[3].y = iRect.bottom;
		points[4] = points[0];
		Polyline(aDc, points, 5);

		SelectObject(aDc, oldPen);
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
