
#pragma once

#include <mzfc_inc.h>
#include "resource.h"

class CPositionWnd : public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CPositionWnd);
public:
	CPositionWnd();
	virtual ~CPositionWnd();

	float GetPosition();
protected:
	UiToolbar_Text iToolBar;
	UiSingleLineEdit iEdit;
	float iPosition;

	BOOL OnInitDialog();
	void OnMzCommand(WPARAM wParam, LPARAM lParam);
};
