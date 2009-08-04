
#pragma once

#include <mzfc_inc.h>
#include <Mzfc/MemoryDC.h>
#include "global.h"
#include "resource.h"
#include "BookMemo.h"
#include "RectButton.h"
#include "Content.h"
#include "ContentCompnent.h"
#include "StatusBarCompnent.h"
#include "BackCompnent.h"
#include "global.h"

class CContentWnd : public CMzWndEx,
                    public IRectButtonObserver
{
	MZ_DECLARE_DYNAMIC(CContentWnd);
public:
	CContentWnd();
	virtual ~CContentWnd();
protected:
	CContent iContent;
	CContentCompnent iContentCompnent;
	CStatusBarCompnent iStatusBarCompnent;
	CBackCompnent iBackCompnent;
	CRectButton iSelectFileButton;
	CRectButton iPositionButton;
	CRectButton iPrevPageButton;
	CRectButton iNextPageButton;
	CBookMemo iBookMemo;
	MemoryDC iMemoryDC;

	BOOL OnInitDialog();
	void PaintWin(HDC hdc, RECT* prcUpdate = NULL);
	void OnLButtonUp(UINT fwKeys, int xPos, int yPos);
	int OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam);

	bool SelectFile(CMzStringW& aFilename);
	void OpenFile(LPCTSTR aFilename);
	void Draw(HDC aDc);

	void OnSelectFileButtonClick();
	void OnPositionButtonClick();
	void OnPrevPageButtonClick();
	void OnNextPageButtonClick();

	//From IRectButtonObserver.
	void OnClick(CRectButton& aRectButton);
};
