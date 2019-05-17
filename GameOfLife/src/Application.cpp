#include "GoL.h"
#include "engine.h"
#include "Config.h"
#include "util\Timer.h"
#include <thread>
#include <memory>

int main()
{
	/*engine game_instance(std::make_unique<GoL>());
	game_instance.Run();*/

	using namespace std::chrono_literals;
	int64_t dur;
	{
		Timer<std::chrono::microseconds> timer(&dur);
		std::this_thread::sleep_for(1s);
	}
	std::cout << dur << " should be 1000000\n";

	return 0;
}