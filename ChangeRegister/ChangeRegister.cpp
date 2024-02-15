#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cctype>
#include "windows.h"
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cwctype>

using namespace std;

// ������� ��� ����������� ������ � ����� ������
std::wstring toLowerCase(const std::wstring& input) {
    std::wstring result = input;
    //std::transform(result.begin(), result.end(), result.begin(), ::towlower);
    // ��������� ����� ������� ����� � ������ ������� ���� �������
    bool capitalizeNext = true;

    for (wchar_t& c : result) {
        if (std::iswalpha(c)) {
            // ˳���� � ����
            if (capitalizeNext) {
                // ����� ����� �����
                c = std::towupper(c);
                capitalizeNext = false; // �������� ����� �� ������� �������
            }
            else {
                // ����� ���� �����
                c = std::towlower(c);
            }
        }
        else if (c == '.' || c == '!' || c == '?') {
            // ʳ���� �������, �������� ����� �� ���� �������
            capitalizeNext = true;
        }
    }
    return result;
}

int main() {
    // ������������ ������ ��� �������� Unicode
    std::locale::global(std::locale(""));

    // ³�������� ����� �����
    if (!OpenClipboard(NULL)) {
        std::cerr << "������� �������� ������ �����" << std::endl;
        return 1;
    }

    // �������� ���������� ���������� ��'���� � ����� �����
    HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
    if (hClipboardData == NULL) {
        std::cerr << "������� ��������� ����� � ������ �����" << std::endl;
        CloseClipboard();
        return 1;
    }

    // ���������� ���������� � �������� �� ���
    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hClipboardData));

    // ����������, �� �������� �����
    if (pszText != NULL) {
        // ���������� ����� � ����� ������
        std::wstring lowerCaseText = toLowerCase(pszText);

        // ������ ����� ����� �� ������ �����
        if (OpenClipboard(NULL)) 
        {
            EmptyClipboard();
            size_t len = lowerCaseText.size();
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t));
            if (hMem != NULL) 
            {
                wchar_t* pszNewText = static_cast<wchar_t*>(GlobalLock(hMem));
                if (pszNewText != NULL) 
                {
                    wcscpy_s(pszNewText, len + 1, lowerCaseText.c_str());
                    GlobalUnlock(hMem);
                    SetClipboardData(CF_UNICODETEXT, hMem);
                }
                else 
                {
                    std::cerr << "������� ���������� ������ ��� ������ �����" << std::endl;
                }
            }
            else 
            {
                std::cerr << "������� �������� ���'�� ��� ������" << std::endl;
            }
            CloseClipboard();
        }
        else 
        {
            std::cerr << "������� �������� ������ ����� ��� ������ ������" << std::endl;
        }

        // ��������� ���'���
        GlobalUnlock(hClipboardData);
    }
    else 
    {
        std::cerr << "������� ��������� ������ � ������ �����" << std::endl;
    }

    // ��������� ����� �����
    CloseClipboard();

    return 0;
}