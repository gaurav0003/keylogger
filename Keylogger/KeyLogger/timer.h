#ifndef _TIMER_H_
#define _TIMER_H_

#include <thread>
#include <chrono>

class Timer
{
	std::thread Thread;
	bool alive = false;
	long callNumber = -1L;
	long repeat_count = -1L;
	std::chrono::milliseconds interval = std::chrono::milliseconds(0);
	std::function<void(void)> funct = nullptr;

	void sleepAndRun()
	{
		std::this_thread::sleep_for(interval);

		if (alive)
		{
			Function() ();
		}
	}

	void threadFunc()
	{
		if (callNumber == Infinite)
		{
			while (alive)
			{
				sleepAndRun();
			}
		}
		else
		{
			while (repeat_count--)
			{
				sleepAndRun();
			}
		}
	}

public:
	static const long Infinite = -1L;

	Timer() {}

	Timer(const std::function<void(void)> &f) : funct(f) {}

	Timer(const std::function<void(void)> &f,
		const unsigned long &i, const long repeat = Timer::Infinite) : funct(f),
		interval(std::chrono::milliseconds(i)), callNumber(repeat) {}

	void start(bool Async = true)
	{
		alive = true;
		repeat_count = callNumber;

		if (Async)
		{
			Thread = std::thread(&Timer::threadFunc, this);
		}
		else
		{
			this->threadFunc();
		}
	}

	void stop()
	{
		alive = false;
		Thread.join();
	}

	void setFunction(const std::function<void(void)> &f)
	{
		funct = f;
	}

	bool isAlive() const { return alive; }

	void repeatCount(const long r)
	{
		if (alive)
		{
			return;
		}

		callNumber = r;
	}

	long getLeftCount() const { return repeat_count; }

	long repeatCount() const { return callNumber; }

	void setInterval(const unsigned long &i)
	{
		if (alive)
		{
			return;
		}

		interval = std::chrono::milliseconds(i);
	}

	unsigned long Interval() const { return interval.count(); }

	const std::function<void(void)> &Function() const
	{
		return funct;
	}
};
#endif // _TIMER_H_