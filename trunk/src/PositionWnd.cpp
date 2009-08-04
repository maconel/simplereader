
#include "StdAfx.h"
#include "PositionWnd.h"
#include <wchar.h>

MZ_IMPLEMENT_DYNAMIC(CPositionWnd)

CPositionWnd::CPositionWnd()
{
}

CPositionWnd::~CPositionWnd()
{
}

float CPositionWnd::GetPosition()
{
	return iPosition;
}

BOOL CPositionWnd::OnInitDialog()
{
	if (!CMzWndEx::OnInitDialog())
		return FALSE;

	iEdit.SetPos(MZM_MARGIN_MAX, MZM_MARGIN_MAX, GetWidth() - MZM_MARGIN_MAX * 2, 70);
	iEdit.SetID(IDC_POSITIONTEXT);
	iEdit.SetTip(_T("xx%"));
	AddUiWin(&iEdit);

	iToolBar.SetPos(0, GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);
	iToolBar.SetButton(0, true, true, _T("确定"));
	iToolBar.SetButton(2, true, true, _T("取消"));
	iToolBar.SetID(IDC_POSITIONTOOLBAR);
	AddUiWin(&iToolBar);

	return TRUE;
}

void CPositionWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	switch (id)
	{
	case IDC_POSITIONTOOLBAR:
		{
		if (lParam == 0)
		{
			if (_stscanf(iEdit.GetText(), _T("%f"), &iPosition) == 0)
			{
				MzMessageBoxEx(m_hWnd, _T("请输入一个数字。"), _T("提示"), MZ_OK);
			}
			else
			{
				iPosition /= 100;
				EndModal(ID_OK);
			}
		}
		else if (lParam == 2)
		{
			EndModal(ID_CANCEL);
		}
		}
		break;
	}
}
