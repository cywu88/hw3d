#pragma once
#include "ChiliWin.h"

class Window
{
private:
	
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetIns() noexcept;
 

	private:
		WindowClass() noexcept;
		~WindowClass();

		WindowClass(const WindowClass&) = delete;
		WindowClass& operator =(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Chili Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(int width,int height,const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};