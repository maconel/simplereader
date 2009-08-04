
#pragma once

#include "Compnent.h"
#include "Content.h"

class CContentCompnent : public CCompnent
{
public:
	CContentCompnent();
	virtual ~CContentCompnent();

	void Draw(HDC aDc);
	void SetForeColor(COLORREF aColor);
	void SetFontHeight(int aFontHeight);
	void SetContent(CContent* aContent);
protected:
	COLORREF iForeColor;
	HFONT iFont;
	int iFontHeight;
	CContent* iContent;
};
