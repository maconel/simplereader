
#include "StdAfx.h"
#include "ContentWnd.h"
#include <windows.h>
#include <IFileBrowser.h>
#include <IFileBrowser_GUID.h>
#include <IMzUnknown.h>
#include <IMzUnknown_IID.h>
#include <ShellNotifyMsg.h>
#include "PositionWnd.h"
#include "toolbox.h"

MZ_IMPLEMENT_DYNAMIC(CContentWnd)

CContentWnd::CContentWnd()
{
}

CContentWnd::~CContentWnd()
{
	iMemoryDC.Unload();

	if (gConfig.iScreenAlwaysOn)
	{
		SetScrennAutoOff();
	}
	ShowMzTopBar();
}

BOOL CContentWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
		return FALSE;

	//读入配置。
	gConfig.Load();

	//创建内存dc.
	iMemoryDC.Create(GetWidth(), GetHeight());
	SetBkMode(iMemoryDC.GetDC(), TRANSPARENT);

	//隐藏标题栏。
	HideMzTopBar();

	//背景灯。
	if (gConfig.iScreenAlwaysOn)
	{
		SetScreenAlwaysOn(m_hWnd);
	}

	//字体。
	iStatusBarCompnent.SetFontHeight(gConfig.iStatusBarFontHeight);

	//组件和按钮位置颜色初始化。
	RECT rect = MzGetWindowRect();
	int statusBarHeight = iStatusBarCompnent.MeasureHeight();

	iBackCompnent.SetBackColor(gConfig.iBackColor);
	iBackCompnent.SetRect(rect.left, rect.top, RECT_WIDTH(rect), RECT_HEIGHT(rect));

	iContentCompnent.SetForeColor(gConfig.iTextColor);
	iContentCompnent.SetFontHeight(gConfig.iContentFontHeight);
	iContentCompnent.SetContent(&iContent);
	iContentCompnent.SetRect(rect.left, rect.top, RECT_WIDTH(rect), RECT_HEIGHT(rect) - statusBarHeight);

	iStatusBarCompnent.SetForeColor(gConfig.iTextColor);
	iStatusBarCompnent.SetRect(rect.left, rect.bottom - statusBarHeight, RECT_WIDTH(rect), statusBarHeight);

	iPositionButton.SetObserver(this);
	iSelectFileButton.SetObserver(this);
	iPrevPageButton.SetObserver(this);
	iNextPageButton.SetObserver(this);

	iPositionButton.SetRect(gConfig.iPositionButtonRect.left, gConfig.iPositionButtonRect.top, RECT_WIDTH(gConfig.iPositionButtonRect), RECT_HEIGHT(gConfig.iPositionButtonRect));
	iSelectFileButton.SetRect(gConfig.iSelectFileButtonRect.left, gConfig.iSelectFileButtonRect.top, RECT_WIDTH(gConfig.iSelectFileButtonRect), RECT_HEIGHT(gConfig.iSelectFileButtonRect));
	iPrevPageButton.SetRect(gConfig.iPrevPageButtonRect.left, gConfig.iPrevPageButtonRect.top, RECT_WIDTH(gConfig.iPrevPageButtonRect), RECT_HEIGHT(gConfig.iPrevPageButtonRect));
	iNextPageButton.SetRect(gConfig.iNextPageButtonRect.left, gConfig.iNextPageButtonRect.top, RECT_WIDTH(gConfig.iNextPageButtonRect), RECT_HEIGHT(gConfig.iNextPageButtonRect));

	iPositionButton.SetBackColor(gConfig.iButtonColor);
	iSelectFileButton.SetBackColor(gConfig.iButtonColor);
	iPrevPageButton.SetBackColor(gConfig.iButtonColor);
	iNextPageButton.SetBackColor(gConfig.iButtonColor);

	//打开上次的文件。
	if (!gConfig.iLastFilename.IsEmpty())
	{
		OpenFile(gConfig.iLastFilename);
	}

	Draw(iMemoryDC.GetDC());

	return TRUE;
}

void CContentWnd::PaintWin(HDC hdc, RECT* prcUpdate)
{
	BitBlt(hdc, 0, 0, GetWidth(), GetHeight(), iMemoryDC.GetDC(), 0, 0, SRCCOPY);
}

void CContentWnd::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
	iSelectFileButton.ButtonDown(xPos, yPos);
	iPositionButton.ButtonDown(xPos, yPos);
	iPrevPageButton.ButtonDown(xPos, yPos);
	iNextPageButton.ButtonDown(xPos, yPos);
}

int CContentWnd::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	return SHK_RET_APPEXIT_SHELLTOP;
}

bool CContentWnd::SelectFile(CMzStringW& aFilename)
{
	bool ret = false;
	IFileBrowser* fileBrowser = NULL;
	IMzSelect* selector = NULL;

	if (SUCCEEDED(CoCreateInstance(CLSID_FileBrowser, NULL, CLSCTX_INPROC_SERVER, IID_MZ_FileBrowser, (void**)&fileBrowser)))
	{
		if (SUCCEEDED(fileBrowser->QueryInterface(IID_MZ_Select, (void**)&selector)))
		{
			fileBrowser->SetParentWnd(m_hWnd);
			fileBrowser->SetOpenDocumentType(DOCUMENT_SELECT_SINGLE_FILE);
			fileBrowser->SetExtFilter(_T("*.txt"));
			fileBrowser->SetOpenDirectoryPath(gConfig.iDefaultDirectory);
			if (selector->Invoke())
			{
				aFilename = fileBrowser->GetSelectedFileName();
				ret = true;
			}
		}
	}
	if (selector != NULL)
	{
		selector->Release();
		selector = NULL;
	}
	if (fileBrowser != NULL)
	{
		fileBrowser->Release();
		fileBrowser = NULL;
	}

	return ret;
}

void CContentWnd::OpenFile(LPCTSTR aFilename)
{
	iContent.OpenFile(aFilename);
	iBookMemo.SetFilename(iContent.GetFilename());
	iContent.Seek(iBookMemo.GetOffset());
	iContent.Read();
}

void CContentWnd::Draw(HDC aDc)
{
	//背景。
	iBackCompnent.Draw(aDc);

	//按钮。
	iSelectFileButton.Draw(aDc);
	iPositionButton.Draw(aDc);
	iPrevPageButton.Draw(aDc);
	iNextPageButton.Draw(aDc);

	//内容。
	iContentCompnent.Draw(aDc);

	//状态栏。
	iStatusBarCompnent.SetProgress(iContent.GetFileSize() == 0 ? 0.0f : (float)iContent.GetOffset() / iContent.GetFileSize());
	iStatusBarCompnent.Draw(aDc);
}

void CContentWnd::OnSelectFileButtonClick()
{
	CMzStringW filename;

	ShowMzTopBar();
	if (SelectFile(filename))
	{
		OpenFile(filename);
		gConfig.iLastFilename = iContent.GetFilename();
		gConfig.Save();

		Draw(iMemoryDC.GetDC());
	}
	HideMzTopBar();
}

void CContentWnd::OnPositionButtonClick()
{
	RECT rect = MzGetWindowRect();
	CPositionWnd positionWnd;
	positionWnd.Create(rect.left, rect.top, RECT_WIDTH(rect), RECT_HEIGHT(rect), m_hWnd, 0, WS_POPUP);
	if (positionWnd.DoModal() == ID_OK)
	{
		iContent.Seek(positionWnd.GetPosition() * iContent.GetFileSize());
		iContent.Read();
		iBookMemo.SetOffset(iContent.GetOffset());

		Draw(iMemoryDC.GetDC());
	}
}

void CContentWnd::OnPrevPageButtonClick()
{
	iContent.ReadBack();
	iBookMemo.SetOffset(iContent.GetOffset());

	if (iContent.GetContentLength() > 0)
	{
		Draw(iMemoryDC.GetDC());
		Invalidate();
	}
}

void CContentWnd::OnNextPageButtonClick()
{
	iContent.Read();
	iBookMemo.SetOffset(iContent.GetOffset());

	if (iContent.GetContentLength() > 0)
	{
		Draw(iMemoryDC.GetDC());
		Invalidate();
	}
}

void CContentWnd::OnClick(CRectButton& aRectButton)
{
	if (&aRectButton == &iSelectFileButton)
	{
		OnSelectFileButtonClick();
	}
	else if (&aRectButton == &iPositionButton)
	{
		OnPositionButtonClick();
	}
	else if (&aRectButton == &iPrevPageButton)
	{
		OnPrevPageButtonClick();
	}
	else if (&aRectButton == &iNextPageButton)
	{
		OnNextPageButtonClick();
	}
}
