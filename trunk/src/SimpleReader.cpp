// SimpleReader.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SimpleReader.h"

CSimpleReaderApp gApp;

BOOL CSimpleReaderApp::Init()
{
	CoInitializeEx(0, COINIT_MULTITHREADED);

	RECT workRect = MzGetWorkArea();
	workRect.top = 0;
	iContentWnd.Create(workRect.left, workRect.top, RECT_WIDTH(workRect), RECT_HEIGHT(workRect), 0, 0, 0);
	iContentWnd.Show();

	return TRUE;
}
