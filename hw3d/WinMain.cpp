/* 第一版

#include <windows.h>
#include <string>
#include <sstream>
using namespace std;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(WM_CLOSE);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd,"Respects");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "Dangerfield");
		}
		break;
	case WM_CHAR:
		{
			static string title;
			title.push_back((char)wParam);
			SetWindowText(hWnd, title.c_str());
		}
		break;
	case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			ostringstream oss;
			oss << "(" << pt.x << "," << pt.y << ")";
			SetWindowText(hWnd, oss.str().c_str());

		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)

{
	const auto pClassName = "hw3dbutts";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);



	HWND hWnd = CreateWindowEx(0, pClassName,
		"Happy Hard Window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200,200,640,480,
		nullptr,nullptr,hInstance,nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}

	return 0;
}

*/


/* 第二版
#include "Window.h"
int CALLBACK WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, 
	int nCmdShow)

{

	Window wnd(800, 300, "Donkey Fart Box");

	//Window wnd2(300, 800, "Donkey Fart Box 2");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}
	return msg.wParam;
}
*/

#include "App.h"
int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try 
	{
		//MessageBox(nullptr, "what's the fuck why ", "Chili Exception", MB_OK | MB_ICONEXCLAMATION);

		return App{}.Go();
	}
	catch (const ChiliException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
