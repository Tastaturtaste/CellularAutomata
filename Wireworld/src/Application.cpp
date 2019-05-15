#include "Wireworld.h"
#include "engine.h"
#include "Config.h"
#include <memory>

int main()
{
	engine game_instance(std::make_unique<Wireworld>());
	game_instance.Run();

	return 0;
}