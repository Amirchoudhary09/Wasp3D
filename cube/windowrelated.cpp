
#include "calculatevertexandib.h"
LRESULT CALLBACK Cube:: WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProc(
        hwnd,
        msg,
        wParam,
        lParam);
}
void Cube ::InitWindow(HINSTANCE hInstance)
{
    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;

    wc.hInstance = hInstance;

    wc.lpszClassName = L"DX12Window";

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        L"DX12Window",
        L"Realistic DX12 Cube",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        width,
        height,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    ShowWindow(hwnd, SW_SHOW);
}
