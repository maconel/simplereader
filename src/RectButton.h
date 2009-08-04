#pragma once

#include "Compnent.h"

class CRectButton;
class IRectButtonObserver
{
public:
	virtual void OnClick(CRectButton& aRectButton) = 0;
};

class CRectButton : public CCompnent
{
public:
	CRectButton();
	virtual ~CRectButton();

	bool ButtonDown(int aX, int aY);
	void SetObserver(IRectButtonObserver* aObserver);
protected:
	IRectButtonObserver* iObserver;
};
