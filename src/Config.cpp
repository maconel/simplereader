
#include "StdAfx.h"
#include "Config.h"
#include <wchar.h>
#include <ReadWriteIni.h>
#include "toolbox.h"
#include "global.h"

CConfig::CConfig()
{
	iTextColor = RGB(0x7F, 0x7F, 0x7F);
	iBackColor = RGB(0x00, 0x00, 0x00);
	iButtonColor = RGB(0x30, 0x30, 0x30);
	iContentFontHeight = 28;
	iStatusBarFontHeight = 22;
	MAKE_RECT(iPositionButtonRect, 20, 20, 100, 100);
	MAKE_RECT(iSelectFileButtonRect, (480-100)/2, 20, 100, 100);
	MAKE_RECT(iPrevPageButtonRect, (480-100)/2, 152, 100, 100);
	MAKE_RECT(iNextPageButtonRect, (480-150)/2, (720-150)/2, 150, 150);
	MAKE_RECT(iExitButtonRect, 480-20-100, 20, 100, 100);
	iDefaultDirectory = _T("\\disk\\ebook");
	iScreenAlwaysOn = false;
	iDisablekey_home = false;
	iDisablekey_power = false;
	iDisablekey_play = true;
	iDisablekey_volumeup = true;
	iDisablekey_volumedown = true;
}

CConfig::~CConfig()
{
}

void CConfig::Load()
{
	TCHAR* strValue = NULL;
	DWORD intValue = 0;
#ifdef _DEBUG
	CMzStringW filename = _T("\\Storage Card\\") CONFIG_FILENAME;
#else
	CMzStringW filename = GetAppPath() + CONFIG_FILENAME;
#endif

	if (IniReadString(_T("common"), _T("lastfilename"), &strValue, filename) && strValue != NULL)
		iLastFilename = strValue;

	if (IniReadString(_T("ui"), _T("textcolor"), &strValue, filename) && strValue != NULL)
	{
		if (_stscanf(strValue, _T("%X"), &intValue) != 0)
			iTextColor = intValue;
	}
	if (IniReadString(_T("ui"), _T("backcolor"), &strValue, filename) && strValue != NULL)
	{
		if (_stscanf(strValue, _T("%X"), &intValue) != 0)
			iBackColor = intValue;
	}
	if (IniReadString(_T("ui"), _T("buttoncolor"), &strValue, filename) && strValue != NULL)
	{
		if (_stscanf(strValue, _T("%X"), &intValue) != 0)
			iButtonColor = intValue;
	}

	if (IniReadInt(_T("ui"), _T("contentfontheight"), &intValue, filename))
		iContentFontHeight = intValue;
	if (IniReadInt(_T("ui"), _T("statusbarfontheight"), &intValue, filename))
		iStatusBarFontHeight = intValue;

	if (IniReadInt(_T("ui"), _T("positionbuttonrect_left"), &intValue, filename))
		iPositionButtonRect.left = intValue;
	if (IniReadInt(_T("ui"), _T("positionbuttonrect_top"), &intValue, filename))
		iPositionButtonRect.top = intValue;
	if (IniReadInt(_T("ui"), _T("positionbuttonrect_right"), &intValue, filename))
		iPositionButtonRect.right = intValue;
	if (IniReadInt(_T("ui"), _T("positionbuttonrect_bottom"), &intValue, filename))
		iPositionButtonRect.bottom = intValue;

	if (IniReadInt(_T("ui"), _T("selectfilebuttonrect_left"), &intValue, filename))
		iSelectFileButtonRect.left = intValue;
	if (IniReadInt(_T("ui"), _T("selectfilebuttonrect_top"), &intValue, filename))
		iSelectFileButtonRect.top = intValue;
	if (IniReadInt(_T("ui"), _T("selectfilebuttonrect_right"), &intValue, filename))
		iSelectFileButtonRect.right = intValue;
	if (IniReadInt(_T("ui"), _T("selectfilebuttonrect_bottom"), &intValue, filename))
		iSelectFileButtonRect.bottom = intValue;

	if (IniReadInt(_T("ui"), _T("prevpagebuttonrect_left"), &intValue, filename))
		iPrevPageButtonRect.left = intValue;
	if (IniReadInt(_T("ui"), _T("prevpagebuttonrect_top"), &intValue, filename))
		iPrevPageButtonRect.top = intValue;
	if (IniReadInt(_T("ui"), _T("prevpagebuttonrect_right"), &intValue, filename))
		iPrevPageButtonRect.right = intValue;
	if (IniReadInt(_T("ui"), _T("prevpagebuttonrect_bottom"), &intValue, filename))
		iPrevPageButtonRect.bottom = intValue;

	if (IniReadInt(_T("ui"), _T("nextpagebuttonrect_left"), &intValue, filename))
		iNextPageButtonRect.left = intValue;
	if (IniReadInt(_T("ui"), _T("nextpagebuttonrect_top"), &intValue, filename))
		iNextPageButtonRect.top = intValue;
	if (IniReadInt(_T("ui"), _T("nextpagebuttonrect_right"), &intValue, filename))
		iNextPageButtonRect.right = intValue;
	if (IniReadInt(_T("ui"), _T("nextpagebuttonrect_bottom"), &intValue, filename))
		iNextPageButtonRect.bottom = intValue;

	if (IniReadInt(_T("ui"), _T("exitbuttonrect_left"), &intValue, filename))
		iExitButtonRect.left = intValue;
	if (IniReadInt(_T("ui"), _T("exitbuttonrect_top"), &intValue, filename))
		iExitButtonRect.top = intValue;
	if (IniReadInt(_T("ui"), _T("exitbuttonrect_right"), &intValue, filename))
		iExitButtonRect.right = intValue;
	if (IniReadInt(_T("ui"), _T("exitbuttonrect_bottom"), &intValue, filename))
		iExitButtonRect.bottom = intValue;

	if (IniReadString(_T("ui"), _T("defaultdirectory"), &strValue, filename) && strValue != NULL)
		iDefaultDirectory = strValue;

	if (IniReadInt(_T("ui"), _T("screenalwayson"), &intValue, filename))
		iScreenAlwaysOn = !!intValue;

	if (IniReadInt(_T("ui"), _T("disablekey_home"), &intValue, filename))
		iDisablekey_home = !!intValue;
	if (IniReadInt(_T("ui"), _T("disablekey_power"), &intValue, filename))
		iDisablekey_power = !!intValue;
	if (IniReadInt(_T("ui"), _T("disablekey_play"), &intValue, filename))
		iDisablekey_play = !!intValue;
	if (IniReadInt(_T("ui"), _T("disablekey_volumeup"), &intValue, filename))
		iDisablekey_volumeup = !!intValue;
	if (IniReadInt(_T("ui"), _T("disablekey_volumedown"), &intValue, filename))
		iDisablekey_volumedown = !!intValue;
}

void CConfig::Save()
{
	CMzStringW filename = GetAppPath() + CONFIG_FILENAME;

	if (!FileExists(filename))
	{
		IniCreateFile(filename);
	}

	IniWriteString(_T("common"), _T("lastfilename"), iLastFilename, filename);
}
