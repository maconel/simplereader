
#pragma once

#include "Compnent.h"

class CStatusBarCompnent : public CCompnent
{
public:
	CStatusBarCompnent();
	virtual ~CStatusBarCompnent();

	void Draw(HDC aDc);
	void SetForeColor(COLORREF aColor);
	void SetFontHeight(int aFontHeight);
	void SetProgress(float aProgress);
	int MeasureHeight();
protected:
	COLORREF iForeColor;
	HPEN iThinPen;
	HPEN iWidePen;
	HFONT iFont;
	float iProgress;
	int iFontHeight;
};
