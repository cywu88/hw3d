#include "App.h"
#include <iomanip>

App::App()
	:wnd(800, 600, "The Donkey Far Box")
{

}

int App::Go()
{
	MSG msg;
	BOOL gResult;

	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		DoFrame();
	}

	if (gResult == -1)
	{
		//throw CHWND_LAST_EXCEPT();
		return -1;
	}
	 
	return msg.wParam;
} 

void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t ;
	wnd.SetTitle(oss.str());
}
