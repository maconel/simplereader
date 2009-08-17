
#include "StdAfx.h"
#include "StatusBarCompnent.h"
#include <mzfc_inc.h>

const int WIDEPEN_WIDTH = 3;
const int THINPEN_WIDTH = 1;

CStatusBarCompnent::CStatusBarCompnent()
{
	iForeColor = 0;
	iThinPen = NULL;
	iWidePen = NULL;
	iFont = NULL;
	iProgress = 0.0f;
	iFontHeight = 22;
}

CStatusBarCompnent::~CStatusBarCompnent()
{
	if (iThinPen != NULL)
	{
		DeleteObject(iThinPen);
		iThinPen = NULL;
	}
	if (iWidePen != NULL)
	{
		DeleteObject(iWidePen);
		iWidePen = NULL;
	}
	if (iFont != NULL)
	{
		DeleteObject(iFont);
		iFont = NULL;
	}
}

void CStatusBarCompnent::Draw(HDC aDc)
{
	COLORREF oldTextColor;
	HFONT oldFont = NULL;
	HPEN oldPen = NULL;
	TCHAR text[32];
	int textLength = 0;
	SYSTEM_POWER_STATUS_EX spse;
	SYSTEMTIME now;
	SIZE size;
	RECT textRect;
	RECT pogressRect;
	const int PROGRESS_MARGINX = 10;
	const int PROGRESS_HEIGHT = 13;

	//文字。
	GetSystemPowerStatusEx(&spse, TRUE);
	GetLocalTime(&now);
	textLength = _stprintf(text, _T("%02.1f%% [%d] %02d:%02d"), iProgress * 100, spse.BatteryLifePercent, now.wHour, now.wMinute);
	//背景。
	CCompnent::Draw(aDc);

	//字体。
	if (iFont == NULL)
		iFont = CreateFont(aDc, iFontHeight);
	if (iFont != NULL)
	{
		oldFont = (HFONT)SelectObject(aDc, iFont);
	}

	//颜色。
	oldTextColor = SetTextColor(aDc, iForeColor);
	if (iThinPen == NULL)
		iThinPen = (HPEN)CreatePen(PS_SOLID, THINPEN_WIDTH, iForeColor);
	if (iWidePen == NULL)
		iWidePen = (HPEN)CreatePen(PS_SOLID, WIDEPEN_WIDTH, iForeColor);

	//区分文字和进度条区域。
	GetTextExtentPoint32(aDc, text, textLength, &size);
	textRect = iRect;
	textRect.left = textRect.right - size.cx;
	textRect.top += 2;
	pogressRect = iRect;
	pogressRect.right -= size.cx;
	pogressRect.left += PROGRESS_MARGINX;
	pogressRect.right -= PROGRESS_MARGINX;
	pogressRect.top = pogressRect.top + (RECT_HEIGHT(pogressRect) - PROGRESS_HEIGHT) / 2 + 2;
	pogressRect.bottom = pogressRect.top + PROGRESS_HEIGHT;

	//画文字。
	ExtTextOut(aDc, textRect.left, textRect.top, 0, &textRect, text, textLength, NULL);

	//画进度条。
	int lineY = pogressRect.top + RECT_HEIGHT(pogressRect) / 2;

	oldPen = (HPEN)SelectObject(aDc, iThinPen);
	MoveToEx(aDc, pogressRect.left + WIDEPEN_WIDTH, lineY, NULL);
	LineTo(aDc, pogressRect.right - WIDEPEN_WIDTH, lineY);

	SelectObject(aDc, iWidePen);
	MoveToEx(aDc, pogressRect.left + WIDEPEN_WIDTH, lineY, NULL);
	LineTo(aDc, pogressRect.left + (RECT_WIDTH(pogressRect) - WIDEPEN_WIDTH * 2) * iProgress, lineY);

	MoveToEx(aDc, pogressRect.left, pogressRect.top, NULL);
	LineTo(aDc, pogressRect.left, pogressRect.bottom);

	MoveToEx(aDc, pogressRect.right - WIDEPEN_WIDTH, pogressRect.top, NULL);
	LineTo(aDc, pogressRect.right - WIDEPEN_WIDTH, pogressRect.bottom);

	SelectObject(aDc, oldPen);

	//恢复dc.
	SelectObject(aDc, oldFont);
	SetTextColor(aDc, oldTextColor);
}

void CStatusBarCompnent::SetForeColor(COLORREF aColor)
{
	iForeColor = aColor;
}

void CStatusBarCompnent::SetFontHeight(int aFontHeight)
{
	iFontHeight = aFontHeight;
}

void CStatusBarCompnent::SetProgress(float aProgress)
{
	iProgress = aProgress;
}

int CStatusBarCompnent::MeasureHeight()
{
	return iFontHeight + 1;
}
