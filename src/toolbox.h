#pragma once
#include <mzfc_inc.h>

bool FileExists(const TCHAR* filename);
CMzStringW GetAppPath();
bool a2u(LPCSTR src, int& srcsize, LPWSTR dst, int& dstsize, DWORD flags);
bool u2a(LPWSTR src, int& srcsize, LPSTR dst, int& dstsize, DWORD flags);
void utf8_to_unicode(LPCSTR src, int& srcsize, LPWSTR dst, int& dstsize);
void unicode_to_utf8(LPCWSTR src, int& srcsize, LPSTR dst, int& dstsize);
void RotationPosition(int& x, int& y, int width, int height, int mode);
void RotationRect(RECT& rect, int width, int height, int mode);
bool RotationBlt(HDC hdcdst, int xdst, int ydst, int width, int height, HDC hdcsrc, int xsrc, int ysrc, int mode);
