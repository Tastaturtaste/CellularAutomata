#pragma once

#include <unordered_map>
#include <SFML\Graphics.hpp>
#include "Cell.h"
#include "Config.h"
#include "Base_Gamerules.h"
#include <memory>


class engine {
public:
	using uint = unsigned int;
	using point = sf::Vector2i;
	using Color_lookup = std::vector<sf::Color>;
	const Color_lookup m_color_lookup;

private:
	std::unique_ptr<Base_gamerules> m_gamerules;
	Config m_config;
	sf::RectangleShape m_FieldShape;
	
	std::vector<std::vector<Cell>> m_Cells; // rowmajor, m_Cells[y][x]
	std::vector<std::vector<uint>> m_next_status;
	sf::RectangleShape m_cell_shape;
	uint m_cell_size = m_config.cell_size;
	sf::RenderWindow m_window;
	Cell border_cell;
	
	bool is_paused = true;
	sf::Clock timer;

	void update_cells();
	void Update();
	Cell& mousepos_to_cell(sf::Vector2f mouse_pos);
	const sf::RectangleShape& getShape() const;
	void ClearAll();
	void handle_events();
	void switch_pause();

public:
	engine(std::unique_ptr<Base_gamerules> gamerules, const Config& config = Config());
	
	void Run();

};