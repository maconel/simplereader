
#pragma once

#include "resource.h"
#include <mzfc_inc.h>
#include "ContentWnd.h"

class CSimpleReaderApp : public CMzApp
{
public:
	BOOL Init();
protected:
	CContentWnd iContentWnd;
};
