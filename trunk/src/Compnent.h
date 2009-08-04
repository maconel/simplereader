#pragma once

class CCompnent
{
public:
	CCompnent();
	virtual ~CCompnent();

	virtual void SetRect(int aLeft, int aTop, int aWidth, int aHeight);
	void GetRect(RECT& aRect);
	virtual void SetBackColor(COLORREF aColor);
	COLORREF GetBackColor();
	virtual void Draw(HDC aDc);
protected:
	RECT iRect;
	COLORREF iBackColor;
	HBRUSH iBrush;

	static HFONT CreateFont(HDC aDc, int aFontHeight);
};
