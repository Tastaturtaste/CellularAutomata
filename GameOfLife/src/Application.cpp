#include "GoL.h"
#include "engine.h"
#include "Config.h"
#include "util\Timer.h"
#include <thread>
#include <memory>

int main()
{
	engine game_instance(std::make_unique<GoL>());
	game_instance.Run();

	return 0;
}