#include "App.h"
#include <iomanip>

App::App()
	:wnd(800, 600, "The Donkey Far Box")
{

}

int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		DoFrame();
	}
} 

void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t ;
	wnd.SetTitle(oss.str());

	//增加这一行窗口变成黑色的了，不再是透明的了

	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c,c,1.0f);
	wnd.Gfx().DrawTestTriangle(timer.Peek());
	wnd.Gfx().EndFrame();
}
