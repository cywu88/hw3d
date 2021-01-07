#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include "WindowsThrowMacros.h"
#include <optional>
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>

class Window
{
public:
	class Exception : public ChiliException
	{
		using ChiliException::ChiliException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept; 
	};

	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
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
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;
	Mouse mouse; 
private:
	int width;
	int height;
	HWND hWnd;

	//这是一个unique_ptr指针，当窗口销毁时也会自动销毁这个图形对象
	std::unique_ptr<Graphics> pGfx;

};