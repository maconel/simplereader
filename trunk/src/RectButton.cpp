#include "StdAfx.h"
#include "RectButton.h"

CRectButton::CRectButton()
{
	iObserver = NULL;
}

CRectButton::~CRectButton()
{
}

bool CRectButton::ButtonDown(int aX, int aY)
{
	bool ret = false;
	POINT point = {aX, aY};

	if (PtInRect(&iRect, point))
	{
		ret = true;
		if (iObserver != NULL)
			iObserver->OnClick(*this);
	}

	return ret;
}

void CRectButton::SetObserver(IRectButtonObserver* aObserver)
{
	iObserver = aObserver;
}
