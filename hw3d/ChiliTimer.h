#pragma once

#include <chrono>

class ChiliTimer
{
public:
	ChiliTimer() noexcept;
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};