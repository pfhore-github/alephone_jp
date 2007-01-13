#ifndef _HPL_LIB_MFC_
#define _HPL_LIB_MFC_

#include "stdafx.h"
#include "HPLLibCommon.h"

//CString->char
void strToChar(CString& src, char* dest);

//WCHAR->char
void wcharToChar(WCHAR* src, char* dest);

//char->WCHAR
void charToWChar(char* src, WCHAR* dest);

//split
void splitString(CString& src, const char* sep, CStringArray &array);



void setIntegerNum(int num, CEdit* edit);
int getIntegerNum(CEdit* edit);
void setComboAndIntegerNum(int num, int max,
                           CEdit* edit, CComboBox* cmb);
void setCombo(int num, int max, CComboBox* cmb);

/**disable*/
void worldToWindow(int worldX, int worldY, POINT* point);

void setStatusBar(int index, CString str);

void copySurfaceToBitmap(CDC* cdc, CBitmap* dest, SDL_Surface* surface, SDL_Color* palette);
#endif