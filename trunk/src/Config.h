
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
	int iContentFontHeight;
	int iStatusBarFontHeight;
	RECT iPositionButtonRect;
	RECT iSelectFileButtonRect;
	RECT iPrevPageButtonRect;
	RECT iNextPageButtonRect;
	CMzStringW iDefaultDirectory;
	bool iScreenAlwaysOn;

	CConfig();
	virtual ~CConfig();

	void Load();
	void Save();
};
