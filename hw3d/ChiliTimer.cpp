#include "ChiliTimer.h"

using namespace std::chrono;

ChiliTimer::ChiliTimer() noexcept
{
	last = steady_clock::now();
}

float ChiliTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return 0.0f;
}

float ChiliTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
 