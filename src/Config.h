
#pragma once
#include <mzfc_inc.h>

#define CONFIG_FILENAME _T("cfg.ini")

class CConfig
{
public:
	CMzStringW iLastFilename;
	long iTextColor;
	long iBackColor;
	long iButtonColor;
	long iButtonBorderColor;
	int iContentFontHeight;
	int iStatusBarFontHeight;
	RECT iPositionButtonRect;
	RECT iSelectFileButtonRect;
	RECT iPrevPageButtonRect;
	RECT iNextPageButtonRect;
	RECT iExitButtonRect;
	CMzStringW iDefaultDirectory;
	bool iScreenAlwaysOn;
	bool iDisablekey_home;
	bool iDisablekey_power;
	bool iDisablekey_play;
	bool iDisablekey_volumeup;
	bool iDisablekey_volumedown;

	CConfig();
	virtual ~CConfig();

	void Load();
	void Save();
};
