#include "window.h"
bool FastQuery::Window::Initialize(std::function<std::string(const char*, FastQuery::RecordType)> cb)
{
    WNDCLASSEXA wclsx = {};
    wclsx.cbSize = sizeof(WNDCLASSEXA);
    wclsx.hbrBackground = reinterpret_cast<HBRUSH>(5);
    wclsx.hCursor = LoadCursor(0, IDC_ARROW);
    wclsx.hIcon = wclsx.hIconSm = LoadIcon(0, IDI_APPLICATION);
    wclsx.hInstance = GetModuleHandle(0);
    wclsx.lpszClassName = WIN_NAME;
    wclsx.style = CS_HREDRAW | CS_VREDRAW; // even though it's unresizble, whatever lol
    wclsx.lpfnWndProc = Win32Events;

    if (!RegisterClassExA(&wclsx))
        return false;

    m_win32_winHandle = CreateWindowExA(0, WIN_NAME, WIN_NAME, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT, 0, 0, wclsx.hInstance, 0);
    if (!m_win32_winHandle)
        return false;

    m_win32_textboxInput = CreateWindowExA(0, "Edit", "Domain/IP", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
        TEXTBOX_INPUT_X, TEXTBOX_INPUT_Y, TEXTBOX_INPUT_W, TEXTBOX_INPUT_H, m_win32_winHandle, (HMENU)TEXTBOX_INPUT_ID, wclsx.hInstance, 0);
    if (!m_win32_textboxInput)
        return false;

    m_win32_recordtypes = CreateWindowExA(0, "ComboBox", "", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
        DROPDOWN_X, DROPDOWN_Y, DROPDOWN_W, DROPDOWN_H, m_win32_winHandle, (HMENU)DROPDOWN_ID, wclsx.hInstance, 0);
    if (!m_win32_recordtypes)
        return false;

    m_win32_btn = CreateWindowExA(0, "Button", "GO", WS_VISIBLE | WS_CHILD,
        BTN_X, BTN_Y, BTN_W, BTN_H, m_win32_winHandle, (HMENU)BTN_ID, wclsx.hInstance, 0);
    if (!m_win32_btn)
        return false;

    m_win32_textboxRes = CreateWindowExA(0, "Edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_READONLY,
        TEXTBOX_RES_X, TEXTBOX_RES_Y, TEXTBOX_RES_W, TEXTBOX_RES_H, m_win32_winHandle, (HMENU)TEXTBOX_RES_ID, wclsx.hInstance, 0);
    if (!m_win32_textboxRes)
        return false;

    for(size_t i = 0; i < ARRAYSIZE(FastQuery::RTStrings); i++)
        SendMessageA(m_win32_recordtypes, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(FastQuery::RTStrings[i]));
    SendMessageA(m_win32_recordtypes, CB_SETCURSEL, 0, 0);

    SetWindowLongPtrA(m_win32_winHandle, GWLP_USERDATA, (LONG)this);
    f_cb_dns = cb;

    return true;
}

void FastQuery::Window::Show()
{
    if (!m_win32_winHandle)
        return;
    ShowWindow(m_win32_winHandle, SW_SHOW);
    UpdateWindow(m_win32_winHandle);
}

void FastQuery::Window::Run()
{
    while (GetMessageA(&m_win32_msg, 0, 0, 0))
    {
        TranslateMessage(&m_win32_msg);
        DispatchMessageA(&m_win32_msg);
    }
}

LONG_PTR __stdcall FastQuery::Window::Win32Events(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_LBUTTONUP:
        SetFocus(hwnd);
        return 0;
    case WM_COMMAND:
    {
        if (LOWORD(wparam) == BTN_ID)
        {
            FastQuery::Window* window = reinterpret_cast<FastQuery::Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            std::function<std::string(const char*, FastQuery::RecordType)> callback = window->f_cb_dns;
            if (callback)
            {
                char buffer[512];
                FastQuery::RecordType type;
                if (GetWindowTextA(window->m_win32_textboxInput, buffer, 512) && (type = (FastQuery::RecordType)SendMessageA(window->m_win32_recordtypes, CB_GETCURSEL, 0, 0)) != CB_ERR)
                    SetWindowTextA(window->m_win32_textboxRes, callback(buffer, type).c_str());
            }
        }
        //else if (LOWORD(wparam) == TEXTBOX_INPUT_ID)

        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd,msg,wparam,lparam);
}
