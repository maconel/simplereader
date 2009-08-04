#pragma once

#include "Config.h"

#ifdef _UNICODE
#ifndef tstring
	#define tstring wstring
#endif
#ifndef ttoupper
	#define ttoupper towupper
#endif
#ifndef ttolower
	#define ttolower towlower
#endif
#else
#ifndef tstring
	#define tstring string
#endif
#ifndef ttoupper
	#define ttoupper toupper
#endif
#ifndef ttolower
	#define ttolower tolower
#endif
#endif

#define MAKE_RECT(r, l, t, w, h) (r).left=(l);(r).top=(t);(r).right=(r).left+(w);(r).bottom=(r).top+(h)

extern CConfig gConfig;
