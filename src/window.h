#ifndef KOTERET_WINDOW
#define KOTERET_WINDOW

// Hardcoded values for Window
// No reason to not harcode all this stuff, not an F'ing library :D
#define WIN_NAME "FastQuery"
#define WIN_WIDTH 300
#define WIN_HEIGHT 150

/* CONTROLS */
#define TEXTBOX_INPUT_X 5
#define TEXTBOX_INPUT_Y 5
#define TEXTBOX_INPUT_W 160
#define TEXTBOX_INPUT_H 24
#define TEXTBOX_INPUT_ID 0x6901

#define DROPDOWN_X TEXTBOX_INPUT_X + TEXTBOX_INPUT_W + 5
#define DROPDOWN_Y TEXTBOX_INPUT_Y
#define DROPDOWN_W 60
#define DROPDOWN_H TEXTBOX_INPUT_H - 4
#define DROPDOWN_ID 0x6902

#define BTN_X DROPDOWN_X + DROPDOWN_W + 5
#define BTN_Y 5
#define BTN_W 40
#define BTN_H 24
#define BTN_ID 0x6903

#define TEXTBOX_RES_X 5
#define TEXTBOX_RES_Y TEXTBOX_INPUT_Y + TEXTBOX_INPUT_H + 5
#define TEXTBOX_RES_W BTN_X + BTN_W - TEXTBOX_RES_X
#define TEXTBOX_RES_H 74
#define TEXTBOX_RES_ID 0x6904
/*====================*/

#if defined(_WIN32) || defined(_WIN64)
#ifdef _MSC_VER //MSVC only. G++ on Windows wont understand this directive
#pragma comment(linker,"\"/manifestdependency:type='win32' \ name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \ processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#include <Windows.h>
#define SHOWMESSAGE(x) MessageBoxA(0, x, WIN_NAME, 0)
#endif

#include <functional>
#include "utils.h"


namespace FastQuery
{
	class Window 
	{
	public:
		bool Initialize(std::function<std::string(const char*,FastQuery::RecordType)>); // No constructor :D
		void Show();

		void Run();
	private:
		static LONG_PTR __stdcall Win32Events(HWND, UINT, WPARAM, LPARAM);

		HWND m_win32_winHandle = 0;
		HWND m_win32_textboxInput = 0;
		HWND m_win32_recordtypes = 0;
		HWND m_win32_btn = 0;
		HWND m_win32_textboxRes = 0;
		MSG m_win32_msg = {};

		std::function<std::string(const char*, FastQuery::RecordType)> f_cb_dns = 0;
	};
}

#endif