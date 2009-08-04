
#include "StdAfx.h"
#include "toolbox.h"
int GetDIBits(HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage);

bool FileExists(const TCHAR* filename)
{
	bool ret = false;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(filename, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		ret = false;
	}
	else
	{
		FindClose(hFind);
		ret = true;
	}

	return ret;
}

CMzStringW GetAppPath()
{
	CMzStringW ret;
	TCHAR path[MAX_PATH];
	HMODULE handle = GetModuleHandle(NULL);
	DWORD dwRet = GetModuleFileName(handle, path, MAX_PATH);

	if (dwRet != 0)
	{
		TCHAR* p = path;

		//找到字符串尾。
		while (*p)
			++p;

		//找到最后一个'\\'.
		while (_T('\\') != *p)
			--p;
		++p;

		*p = _T('\0');
		ret = path; 
	}

	return ret;
}

bool a2u(LPCSTR src, int& srcsize, LPWSTR dst, int& dstsize, DWORD flags)
{
	int rst = 0;

	if (srcsize > dstsize)
		srcsize = dstsize;

	//转换。
	rst = MultiByteToWideChar(CP_ACP, flags, src, srcsize, dst, dstsize);
	if (rst == 0)
	{
		srcsize--;
		rst = MultiByteToWideChar(CP_ACP, flags, src, srcsize, dst, dstsize);
		if (rst == 0)
			return false;
	}
	dstsize = rst;

	//src转了多少字节。
	rst = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, dst, rst, NULL, 0, NULL, NULL);
	if (rst == 0)
		return false;
	srcsize = rst;

	return true;
}

bool u2a(LPWSTR src, int& srcsize, LPSTR dst, int& dstsize, DWORD flags)
{
	int rst = 0;

	//转换。
	rst = WideCharToMultiByte(CP_ACP, flags, src, srcsize, dst, dstsize, NULL, NULL);
	if (rst == 0)
		return false;
	dstsize = rst;

	//src转了多少字节。
	rst = MultiByteToWideChar(CP_ACP, 0, dst, rst, NULL, 0);
	if (rst == 0)
		return false;
	srcsize = rst;

	return true;
}

bool unicode_to_utf8_calusize(int& utf8size, int& unicodesize, int need_utf8size, int need_unicodesize)
{
	if (utf8size < need_utf8size || unicodesize < need_unicodesize)
	{
		utf8size = need_utf8size;
		unicodesize = need_unicodesize;
		return false;
	}
	utf8size = need_utf8size;
	unicodesize = need_unicodesize;
	return true;
}

bool utf8_to_unicode_calusize(int& utf8size, int need_utf8size)
{
	if (utf8size < need_utf8size)
	{
		utf8size = need_utf8size;
		return false;
	}
	utf8size = need_utf8size;
	return true;
}

bool utf8_to_unicode_char(LPCSTR utf8, int& utf8size, LPWSTR unicode, int& unicodesize)
{
	/*
	U-00000000 - U-0000007F: 	0xxxxxxx
	U-00000080 - U-000007FF: 	110xxxxx 10xxxxxx
	U-00000800 - U-0000FFFF: 	1110xxxx 10xxxxxx 10xxxxxx
	U-00010000 - U-001FFFFF: 	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-00200000 - U-03FFFFFF: 	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-04000000 - U-7FFFFFFF: 	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/

	char h;
	char l;

	if (utf8 == NULL || utf8size == 0)
	{
		utf8size = 0;
		unicodesize = 0;
		return false;
	}

	if ((utf8[0] & 0x80) == 0x00)		//0xxxxxxx
	{
		if (!unicode_to_utf8_calusize(utf8size, unicodesize, 1, 1))
			return false;

		if (unicode != NULL)
		{
			h = 0;
			l = utf8[0];
			unicode[0] = MAKEWORD(l, h);
		}
	}
	else if ((utf8[0] & 0xE0) == 0xC0)	//110xxxxx
	{
		if (!unicode_to_utf8_calusize(utf8size, unicodesize, 2, 1))
			return false;

		if (unicode != NULL)
		{
			h = (utf8[0] >> 2) & 0x07;
			l = ((utf8[0] << 6) & 0xC0) | ((utf8[1]) & 0x3F);
			unicode[0] = MAKEWORD(l, h);
		}
	}
	else if ((utf8[0] & 0xF0) == 0xE0)	//1110xxxx
	{
		if (!unicode_to_utf8_calusize(utf8size, unicodesize, 3, 1))
			return false;

		if (unicode != NULL)
		{
			h = ((utf8[0] << 4) & 0xF0) | ((utf8[1] >> 2) & 0x0F);
			l = ((utf8[1] << 6) & 0xC0) | ((utf8[2]) & 0x3F);
			unicode[0] = MAKEWORD(l, h);
		}
	}
	else if ((utf8[0] & 0xF8) == 0xF0)	//11110xxx
	{
		if (!unicode_to_utf8_calusize(utf8size, unicodesize, 4, 2))
			return false;

		h = 0;
		l = ((utf8[0] << 2) & 0x1C) | ((utf8[1] >> 6) & 0x03);
		unicode[0] = MAKEWORD(l, h);

		if (unicode != NULL)
		{
			h = ((utf8[1] << 4) & 0xF0) | ((utf8[2] >> 2) & 0x0F);
			l = ((utf8[2] << 6) & 0xC0) | ((utf8[2]) & 0x3F);
			unicode[1] = MAKEWORD(l, h);
		}
	}
	else if ((utf8[0] & 0xFC) == 0xF8)	//111110xx
	{
		if (!unicode_to_utf8_calusize(utf8size, unicodesize, 5, 2))
			return false;

		if (unicode != NULL)
		{
			h = ((utf8[0]) & 0x00);
			l = ((utf8[1] << 2) & 0xFC) | ((utf8[2] >> 4) & 0x03);
			unicode[0] = MAKEWORD(l, h);

			h = ((utf8[2] << 4) & 0xF0) | ((utf8[3] >> 2) & 0x0F);
			l = ((utf8[3] << 6) & 0xC0) | ((utf8[4]) & 0x3F);
			unicode[1] = MAKEWORD(l, h);
		}
	}
	else if ((utf8[0] & 0xFE) == 0xFC)	//1111110x
	{
		if (!unicode_to_utf8_calusize(utf8size, unicodesize, 6, 2))
			return false;

		if (unicode != NULL)
		{
			h = ((utf8[0] << 6) & 0x40) | ((utf8[1]) & 0x3F);
			l = ((utf8[2] << 2) & 0xFC) | ((utf8[2] >> 4) & 0x03);
			unicode[0] = MAKEWORD(l, h);

			h = ((utf8[3] << 4) & 0xF0) | ((utf8[4] >> 2) & 0x0F);
			l = ((utf8[4] << 6) & 0xC0) | ((utf8[5]) & 0x3F);
			unicode[1] = MAKEWORD(l, h);
		}
	}
	else
	{
		utf8size = 0;
		unicodesize = 0;
		return false;
	}

	return true;
}

bool unicode_to_utf8_char(WCHAR unicode, LPSTR utf8, int& utf8size)
{
	/*
	U-00000000 - U-0000007F: 	0xxxxxxx
	U-00000080 - U-000007FF: 	110xxxxx 10xxxxxx
	U-00000800 - U-0000FFFF: 	1110xxxx 10xxxxxx 10xxxxxx
	U-00010000 - U-001FFFFF: 	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-00200000 - U-03FFFFFF: 	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-04000000 - U-7FFFFFFF: 	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/

	char h = HIBYTE(unicode);
	char l = LOBYTE(unicode);

	if ((unsigned short)unicode <= 0x007F)
	{
		utf8_to_unicode_calusize(utf8size, 1);

		if (utf8 != NULL)
		{
			utf8[0] = l;
		}
	}
	else if ((unsigned short)unicode <= 0x07FF)
	{
		utf8_to_unicode_calusize(utf8size, 2);

		if (utf8 != NULL)
		{
			utf8[0] = 0xC0 | ((h << 2) & 0x1C) | ((l >> 6) & 0x03);
			utf8[1] = 0x80 | ((l) & 0x3F);
		}
	}
	else
	{
		utf8_to_unicode_calusize(utf8size, 3);

		if (utf8 != NULL)
		{
			utf8[0] = 0xE0 | ((h >> 4) & 0x0F);
			utf8[1] = 0x80 | ((h << 2) & 0x3C) | ((l >> 6) & 0x03);
			utf8[2] = 0x80 | ((l) & 0x3F);
		}
	}

	return true;
}

void utf8_to_unicode(LPCSTR src, int& srcsize, LPWSTR dst, int& dstsize)
{
	int remain_ssize = srcsize;
	int remain_dsize = dstsize;

	while (remain_ssize > 0)
	{
		int ssize = remain_ssize;
		int dsize = remain_dsize;

		if (!utf8_to_unicode_char(src, ssize, dst, dsize))
		{
			break;
		}
		src += ssize;
		if (dst != NULL)
			dst += dsize;
		remain_ssize -= ssize;
		remain_dsize -= dsize;
	}

	srcsize -= remain_ssize;
	dstsize -= remain_dsize;
}

void unicode_to_utf8(LPCWSTR src, int& srcsize, LPSTR dst, int& dstsize)
{
	int remain_ssize = srcsize;
	int remain_dsize = dstsize;

	while (remain_ssize > 0)
	{
		int dsize = remain_dsize;

		if (!unicode_to_utf8_char(*src, dst, dsize))
		{
			break;
		}
		src++;
		if (dst != NULL)
			dst += dsize;
		remain_ssize--;
		remain_dsize -= dsize;
	}

	srcsize -= remain_ssize;
	dstsize -= remain_dsize;
}

//mode: 0为不旋转，1为顺时针90度，2为顺时针180度，3为顺时针270度。
void RotationPosition(int& x, int& y, int width, int height, int mode)
{
	int tmpx = x;

	switch (mode)
	{
	case 1:		//90
		x = height - y;
		y = tmpx;
		break;
	case 2:		//180
		x = width - x;
		y = height - y;
		break;
	case 3:		//270
		x = y;
		y = width - tmpx;
		break;
	case 0:
	default:
		break;
	}
}

void RotationRect(RECT& rect, int width, int height, int mode)
{
	RECT tmpRect = rect;
	switch (mode)
	{
	case 1:
		rect.left = tmpRect.top;
		rect.top = width - tmpRect.right;
		rect.right = tmpRect.bottom;
		rect.bottom = width - tmpRect.left;
		break;
	case 2:
		rect.left = width - tmpRect.right;
		rect.top = height - tmpRect.bottom;
		rect.right = width - tmpRect.left;
		rect.bottom = height - tmpRect.top;
		break;
	case 3:
		rect.left = height - tmpRect.bottom;
		rect.top = tmpRect.left;
		rect.right = height - tmpRect.top;
		rect.bottom = tmpRect.right;
	case 0:
	default:
		break;
	}
}

//mode: 0为不旋转，1为顺时针90度，2为顺时针180度，3为顺时针270度。
bool RotationBlt(HDC hdcdst, int xdst, int ydst, int width, int height, HDC hdcsrc, int xsrc, int ysrc, int mode)
{
	switch (mode)
	{
	case 1:		//90
		{
		/*
		int x = 0;
		int y = 0;
		int dx = x;
		int dy = x;
		int dy0 = y;

		RotationPosition(dx, dy, height, width, mode);
		dy0 = dy;
		for (y=0; y<width; y++, dx--)
		{
			for (x=0, dy=dy0; x<height; x++, dy++)
			{
				COLORREF color = GetPixel(hdcsrc, x + xsrc, y + ysrc);
				SetPixel(hdcdst, dx + xdst, dy + ydst, color);
			}
		}
		*/
		int sx = 0;
		int sy = 0;
		int dx = sx;
		int dy = sy;
		int dy0 = 0;
		HBITMAP sbmp = NULL;
		HBITMAP dbmp = NULL;
		BITMAP sbm;
		BITMAP dbm;
		BITMAPINFO sbmi;
		BITMAPINFO dbmi;
		BYTE* sbits = NULL;
		BYTE* dbits = NULL;

		sbmp = (HBITMAP)GetCurrentObject(hdcsrc, OBJ_BITMAP);
		if (sbmp == NULL)
			return false;
		dbmp = (HBITMAP)GetCurrentObject(hdcdst, OBJ_BITMAP);
		if (dbmp == NULL)
			return false;
		if (0 == GetObject(sbmp, sizeof(sbm), &sbm))
			return false;
		if (0 == GetObject(dbmp, sizeof(dbm), &dbm))
			return false;
		sbits = new BYTE[sbm.bmWidthBytes * sbm.bmHeight];
		dbits = new BYTE[dbm.bmWidthBytes * dbm.bmHeight];

		if (sbits != NULL && dbits != NULL && GetDIBits(hdcsrc, sbmp, 0, sbm.bmHeight, sbits, &sbmi, DIB_RGB_COLORS) > 0)
		{
			RotationPosition(dx, dy, height, width, mode);
			dy0 = dy;
			for (sy=0; sy<width; sy++, dx--)
			{
				for (sx=0, dy=dy0; sx<height; sx++, dy++)
				{
					memcpy(
						&dbits[(dy+ydst)*dbm.bmWidthBytes + (dx+xdst)*(dbm.bmBitsPixel/8)],
						&sbits[(sy+ysrc)*sbm.bmWidthBytes + (sx+xsrc)*(sbm.bmBitsPixel/8)],
						dbm.bmBitsPixel/8);
				}
			}

			dbmi = sbmi;
			dbmi.bmiHeader.biWidth = width;
			dbmi.bmiHeader.biHeight = height;
			StretchDIBits(hdcdst, xdst, ydst, width, height, xdst, ydst, width, height, dbits, &dbmi, DIB_RGB_COLORS, SRCCOPY);
		}

		if (sbits != NULL)
		{
			delete[] sbits;
			sbits = NULL;
		}
		if (dbits != NULL)
		{
			delete[] dbits;
			dbits = NULL;
		}
		}
		break;
	case 3:		//270
		{
		int x = 0;
		int y = 0;
		int dx = x;
		int dy = x;
		int dy0 = y;

		RotationPosition(dx, dy, height, width, mode);
		dy0 = dy;
		for (y=0; y<width; y++, dx++)
		{
			for (x=0, dy=dy0; x<height; x++, dy--)
			{
				COLORREF color = GetPixel(hdcsrc, x + xsrc, y + ysrc);
				SetPixel(hdcdst, dx + xdst, dy + ydst, color);
			}
		}
		}
		break;
	case 2:		//180
		StretchBlt(hdcdst, xdst, ydst, width, height, hdcsrc, xsrc+width, ysrc+height, -width, -height, SRCCOPY);
		break;
	case 0:		//0
	default:
		BitBlt(hdcdst, xdst, ydst, width, height, hdcsrc, xsrc, ysrc, SRCCOPY);
		break;
	}

	return true;
}

int BytesPerLine(int nWidth, int nBitsPerPixel)
{
	return ( (nWidth * nBitsPerPixel + 31) & (~31) ) / 8;
}

int NumColorEntries(int nBitsPerPixel, int nCompression, DWORD biClrUsed)
{
	int nColors = 0;

	switch (nBitsPerPixel)
	{
	case 1:
		nColors = 2;
		break;
	case 2:
		nColors = 4;
		break; // winCE only
	case 4:
		nColors = 16;
		break;
	case 8:
		nColors = 256;
		break;
	case 24:
		nColors = 0;
		break;
	case 16:
	case 32:
		nColors = 3; // I've found that PocketPCs need this regardless of BI_RGB or BI_BITFIELDS
		break;

	default:
		ASSERT(FALSE);
	}

	// If biClrUsed is provided, and it is a legal value, use it
	if (biClrUsed > 0 && biClrUsed <= (DWORD)nColors)
		return biClrUsed;

	return nColors;
}

int GetDIBits(HDC hdc, HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage)
{
	HBITMAP hBitmap;
	VOID* ppvBits = NULL;
	UINT iColorTableSize = 0;

	if (!hbmp || !lpbi)
		return 0;

	// Get dimensions of bitmap
	BITMAP bm;
	if (!::GetObject(hbmp, sizeof(bm),(LPVOID)&bm))
		return 0;

	bm.bmHeight = abs(bm.bmHeight);

	// Initialize the BITMAPINFOHEADER in m_DIBinfo
	BITMAPINFOHEADER& bih = (*lpbi).bmiHeader;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = bm.bmWidth;
	bih.biHeight = bm.bmHeight;
	bih.biPlanes = 1; // Must always be 1 according to docs
	bih.biBitCount = bm.bmBitsPixel;
	if (bm.bmBitsPixel == 16 || bm.bmBitsPixel == 32)
		bih.biCompression = BI_BITFIELDS;
	else
		bih.biCompression = BI_RGB;
//	bih.biSizeImage = BytesPerLine(bm.bmWidth, bm.bmBitsPixel) * bm.bmHeight;
	bih.biSizeImage = bm.bmWidthBytes * bm.bmHeight;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	// Create it!
	hBitmap = CreateDIBSection(hdc, lpbi, uUsage, &ppvBits, NULL, 0);
	if (!hBitmap)
		return 0;

	HDC tmphdc = CreateCompatibleDC(hdc);
	HBITMAP oldBmp = (HBITMAP)SelectObject(tmphdc, hBitmap);
	BitBlt(tmphdc, 0, 0, bm.bmWidth, bm.bmHeight, hdc, 0, 0, SRCCOPY);
	SelectObject(tmphdc, oldBmp);
	DeleteObject(tmphdc);
	tmphdc = NULL;

	if (lpvBits != NULL)
	{
		memcpy(lpvBits, (BYTE*)ppvBits+bm.bmWidthBytes*uStartScan, bm.bmWidthBytes*cScanLines);
	}

	return bm.bmWidthBytes * cScanLines;
}
