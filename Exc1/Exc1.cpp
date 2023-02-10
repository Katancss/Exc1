#include <chrono>
#include <iostream>

template<typename T>
class Timer {
public:
	Timer() : start_point(std::chrono::steady_clock::now()), time(0), running(true) {}

	~Timer() {
		update_clock();
		std::cout << time.count() << std::endl;
	}

	void stop() {
		update_clock();
		running = false;
	}

	void restart() {
		update_clock();
		running = true;
	}

	template<typename T>
	friend std::ostream& operator<< (std::ostream& out, Timer<T>& timer);

private:
	std::chrono::steady_clock::time_point start_point;
	T time;
	bool running;

	void update_clock() {
		if (running)
			time += std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - start_point);
		start_point = std::chrono::steady_clock::now();
	}
};


template<typename T>
std::ostream& operator<<(std::ostream& out, Timer<T>& timer)
{
	timer.update_clock();
	out << timer.time.count() << std::endl;
	return out;
}

void test() {
	double count = 0.0;
	for (auto i = 0; i < 1'000'000; i++) {
		count += std::sin(i) + std::cos(i);
	}
	std::cout << "Test is over" << std::endl;
};

int main() {
	{
		std::cout << "First test:" << std::endl;
		Timer<std::chrono::microseconds> t;
		test();
		std::cout << "Result:" << std::endl;
		std::cout << t;

		t.stop();
		std::cout << "Stop timer" << std::endl;
		test();
		std::cout << t;

		std::cout << "Restart timer and one more test:" << std::endl;
		t.restart();
		test();
	}

	{
		std::cout << "Second test: " << std::endl;
		Timer<std::chrono::microseconds> t;
		t.stop();
		test();
		std::cout << t;

		t.stop();
		test();
		std::cout << t;

		t.stop();
		test();
	}
	
	{
		std::cout << "Third test:" << std::endl;
		Timer<std::chrono::milliseconds> t;
		test();
	}
}
