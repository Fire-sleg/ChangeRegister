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

using namespace std;

// Функція для переведення тексту в нижній регістр
std::wstring toLowerCase(const std::wstring& input) {
    std::wstring result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::towlower);
    return result;
}

int main() {
    // Встановлюємо локаль для підтримки Unicode
    std::locale::global(std::locale(""));

    // Відкриваємо буфер обміну
    if (!OpenClipboard(NULL)) {
        std::cerr << "Помилка відкриття буфера обміну" << std::endl;
        return 1;
    }

    // Отримуємо дескриптор текстового об'єкту у буфері обміну
    HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
    if (hClipboardData == NULL) {
        std::cerr << "Помилка отримання даних з буфера обміну" << std::endl;
        CloseClipboard();
        return 1;
    }

    // Конвертуємо дескриптор в вказівник на дані
    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hClipboardData));

    // Перевіряємо, чи отримано текст
    if (pszText != NULL) {
        // Виводимо отриманий текст на екран
        std::wcout << L"Текст з буфера обміну:" << std::endl;
        std::wcout << pszText << std::endl;

       

        // Переводимо текст у нижній регістр
        std::wstring lowerCaseText = toLowerCase(pszText);

        // Додаємо текст назад до буфера обміну
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
                    std::cerr << "Помилка блокування тексту для буфера обміну" << std::endl;
                }
            }
            else 
            {
                std::cerr << "Помилка виділення пам'яті для тексту" << std::endl;
            }
            CloseClipboard();
        }
        else 
        {
            std::cerr << "Помилка відкриття буфера обміну для запису тексту" << std::endl;
        }

        // Звільнюємо пам'ять
        GlobalUnlock(hClipboardData);
    }
    else 
    {
        std::cerr << "Помилка отримання тексту з буфера обміну" << std::endl;
    }

    // Закриваємо буфер обміну
    CloseClipboard();

    return 0;
}




