// C++ program to demonstrate the declaration and use of
// wstring in C++.

#include <iostream>
#include <string>
using namespace std;
int main()
{
    // For wide character strings, use std::wstring (Unicode
    // support)
    wstring wString = L"Hello, GeeksForGeeks";

    wString = wString + L"SSSSS!";

    // Putting the long string on display

    wcout << wString << endl;

    // Reaching specific large characters

    for (wchar_t ch : wString) {
        wcout << ch << L" ";
    }

    wcout << "█" << endl;

    return 0;
}