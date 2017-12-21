#include <iostream>
#include <chrono>

// Doesn't rum
// Referenced from https://ideone.com/JbUkPq

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
	template<typename F, typename ...Args>
	static typename TimeT::rep execution(F func, Args&&... args)
	{
		auto start = std::chrono::system_clock::now();

		// Now call the function with all the parameters you need.
		func(std::forward<Args>(args)...);

		auto duration = std::chrono::duration_cast< TimeT>
			(std::chrono::system_clock::now() - start);

		return duration.count();
	}
};

struct functor
{
	int state;
	functor(int state) : state(state) {}
	void operator()() const
	{
		std::cout << "In functor run for ";
	}
};

void func()
{
	std::cout << "In function, run for ";
}


int main()
{
	int dummy(3);

	std::cout << measure<std::chrono::nanoseconds>::execution([&dummy]()
	{
		dummy *= 2; // usage with lambdas
		std::cout << "In lambda, run for ";
	}) << std::endl;

	std::cout << measure<std::chrono::nanoseconds>::execution(functor(dummy)) << std::endl;

	std::cout << measure<std::chrono::nanoseconds>::execution(func);

	return 0;
}