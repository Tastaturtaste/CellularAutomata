#pragma once

#include <unordered_map>
#include <memory>
#include <ratio>

#include "Cell.h"
#include "Config.h"
#include "Base_game.h"



class engine {
public:
	using uint = unsigned int;
	using point = sf::Vector2i;
	using Color_lookup = std::vector<sf::Color>;
	const Color_lookup m_color_lookup;

private:
	std::unique_ptr<Base_game> m_game;
	Config m_config;
	//sf::RectangleShape m_FieldShape;
	
	std::vector<std::vector<Cell>> m_Cells; // rowmajor, m_Cells[y][x]
	std::vector<std::vector<uint>> m_next_status;
	uint m_cell_size = m_config.cell_size;
	sf::RenderWindow m_window;
	Cell border_cell;
	
	bool is_paused = true;
	bool game_running = true;
	const float max_fps = static_cast<float>(m_config.framerate_limit);


	void connect_cells();
	void update_cells();
	void Update();
	Cell& mousepos_to_cell(sf::Vector2i mouse_pos);
	void ClearAll();
	void handle_events();
	void mouse_input();
	void switch_pause();
	void draw();
	

public:
	engine(std::unique_ptr<Base_game> game, const Config& config = Config());
	
	void Run();

};


//Period has to be of type std::ratio
template<class Period>
constexpr const uint64_t get_time_per_instance(int i)
{
	return Period::den / (Period::num * i);
}
