#pragma once

class CCompnent
{
public:
	CCompnent();
	virtual ~CCompnent();

	virtual void SetRect(int aLeft, int aTop, int aWidth, int aHeight);
	void GetRect(RECT& aRect);
	virtual void SetBackColor(COLORREF aColor);
	virtual void SetBorderColor(COLORREF aColor);
	COLORREF GetBackColor();
	COLORREF GetBorderColor();
	virtual void Draw(HDC aDc);
	virtual void DrawBorder(HDC aDc);
protected:
	RECT iRect;
	COLORREF iBackColor;
	COLORREF iBorderColor;
	HBRUSH iBrush;
	HPEN iPen;

	static HFONT CreateFont(HDC aDc, int aFontHeight);
};
