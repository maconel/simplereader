
#ifndef __STLTOOLBOX_H__
#define __STLTOOLBOX_H__

#include <string>
#include <algorithm>
#include <malloc.h>

using namespace std;

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000

#define TCHAR_ARG   TCHAR
#define WCHAR_ARG   WCHAR
#define CHAR_ARG    char
#define DOUBLE_ARG  double

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

namespace ToolBox
{
inline void FormatString(std::tstring& output, LPCTSTR format, va_list argList)
{
	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR lpsz = format; *lpsz != _T('\0'); lpsz = _tcsinc(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != _T('%') || *(lpsz = _tcsinc(lpsz)) == _T('%'))
		{
			nMaxLen += (int)_tcslen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != _T('\0'); lpsz = _tcsinc(lpsz))
		{
			// check for valid flags
			if (*lpsz == _T('#'))
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == _T('*'))
				nWidth = va_arg(argList, int);
			else if (*lpsz == _T('-') || *lpsz == _T('+') || *lpsz == _T('0') ||
				*lpsz == _T(' '))
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(lpsz);
			for (; *lpsz != _T('\0') && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
				;
		}

		int nPrecision = 0;
		if (*lpsz == _T('.'))
		{
			// skip past '.' separator (width.precision)
			lpsz = _tcsinc(lpsz);

			// get precision and skip it
			if (*lpsz == _T('*'))
			{
				nPrecision = va_arg(argList, int);
				lpsz = _tcsinc(lpsz);
			}
			else
			{
				nPrecision = _ttoi(lpsz);
				for (; *lpsz != _T('\0') && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
					;
			}
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_tcsncmp(lpsz, _T("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case _T('h'):
				nModifier = FORCE_ANSI;
				lpsz = _tcsinc(lpsz);
				break;
			case _T('l'):
				nModifier = FORCE_UNICODE;
				lpsz = _tcsinc(lpsz);
				break;

			// modifiers that do not affect size
			case _T('F'):
			case _T('N'):
			case _T('L'):
				lpsz = _tcsinc(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case _T('c'):
		case _T('C'):
			nItemLen = 2;
			va_arg(argList, TCHAR_ARG);
			break;
		case _T('c')|FORCE_ANSI:
		case _T('C')|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG);
			break;
		case _T('c')|FORCE_UNICODE:
		case _T('C')|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;

		// strings
		case _T('s'):
			{
				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = lstrlen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case _T('S'):
			{
#ifndef _UNICODE
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = (int)wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
#else
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
#endif
			}
			break;

		case _T('s')|FORCE_ANSI:
		case _T('S')|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case _T('s')|FORCE_UNICODE:
		case _T('S')|FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = (int)wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case _T('d'):
			case _T('i'):
			case _T('u'):
			case _T('x'):
			case _T('X'):
			case _T('o'):
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case _T('e'):
			case _T('g'):
			case _T('G'):
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case _T('f'):
				{
					double f;
					LPTSTR pszTemp;

					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312+nPrecision+6));

					f = va_arg(argList, double);
					_stprintf( pszTemp, _T( "%*.*f" ), nWidth, nPrecision+6, f );
					nItemLen = (int)_tcslen(pszTemp);
				}
				break;

			case _T('p'):
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case _T('n'):
				va_arg(argList, int*);
				break;

			default:
				break;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	output.assign(nMaxLen + 1, _T('\0'));
	_vstprintf(&output[0], format, argListSave);
	output.erase(output.find(_T('\0'), 0));

	va_end(argListSave);
}

inline void FormatString(std::tstring& output, LPCTSTR format, ...)
{
	va_list argList;
	va_start(argList, format);
	FormatString(output, format, argList);
	va_end(argList);
}

inline void LowerString(std::tstring& aString)
{
	transform(aString.begin(), aString.end(), aString.begin(), ttolower);
}

inline void UpperString(std::tstring& aString)
{
	transform(aString.begin(), aString.end(), aString.begin(), ttoupper);
}

inline int TrimLeft(std::tstring& aString, std::tstring aTrim, bool aIgnoreCase)
{
	int ret = 0;
	std::tstring localString = aString;
	std::tstring::size_type pos = std::tstring::npos;

	if (aIgnoreCase)
	{
		LowerString(localString);
		LowerString(aTrim);
	}
	pos = localString.find_first_not_of(aTrim);

	if (pos != std::tstring::npos)
	{
		ret = pos;
		aString.erase(0, pos);
	}

	return ret;
}

inline int TrimRight(std::tstring& aString, std::tstring aTrim, bool aIgnoreCase)
{
	int ret = 0;
	std::tstring localString = aString;
	std::tstring::size_type pos = std::tstring::npos;

	if (aIgnoreCase)
	{
		LowerString(localString);
		LowerString(aTrim);
	}
	pos = localString.find_last_not_of(aTrim);

	if (pos != std::tstring::npos)
	{
		pos++;
		ret = aString.size() - pos;
		aString.erase(pos);
	}

	return ret;
}

inline int Trim(std::tstring& aString, std::tstring aTrim, bool aIgnoreCase)
{
	int ret = 0;

	ret += TrimRight(aString, aTrim, aIgnoreCase);
	ret += TrimLeft(aString, aTrim, aIgnoreCase);

	return ret;
}

inline string::size_type FindStringCaseSensitive(std::tstring aString, std::tstring aFind, string::size_type pos)
{
	LowerString(aString);
	LowerString(aFind);
	return aString.find(aFind, pos);
}

inline void StringReplace(std::tstring& strBig, const std::tstring& strFrom, const std::tstring& strTo)
{
	std::tstring::size_type pos = 0;
	std::tstring::size_type srclen = strFrom.size();
	std::tstring::size_type dstlen = strTo.size();
	while ((pos = FindStringCaseSensitive(strBig, strFrom, pos)) != std::tstring::npos)
	{
		strBig.replace(pos, srclen, strTo);
		pos += dstlen;
	}
}
}

#endif //__STLTOOLBOX_H__
