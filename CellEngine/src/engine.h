#pragma once

#include <unordered_map>
#include <SFML\Graphics.hpp>
#include "Cell.h"
#include "Config.h"

class engine {
private:
	using uint = unsigned int;
	using point = sf::Vector2i;
	using color_lookup = std::unordered_map<uint, sf::Color>;

	Config m_config;
	color_lookup m_color_lookup;
	sf::RectangleShape m_FieldShape;
	
	std::vector<std::vector<Cell>> m_Cells; // rowmajor, m_Cells[y][x]
	sf::RectangleShape m_cell_shape = sf::RectangleShape(sf::Vector2f(m_cell_size*(1.0f - m_cellboarder_percentage), m_CellSize*(1.0f - m_cellboarder_percentage)));
	sf::RenderWindow m_window;
	Cell border_cell;

public:
	engine(const Config& config);
	const sf::RectangleShape& getShape() const;
	Cell& mousepos_to_cell(sf::Vector2f mouse_pos);
	void ClearAll();
	virtual void Update() = 0;
	void Run();

};