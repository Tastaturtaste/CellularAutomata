#pragma once

#include <array>

#include "SFML/Graphics.hpp"


/*
	Cells know their current status, their shape, their neighbors, and their position
*/
class Cell
{
private:
	using uint = unsigned int;

	sf::RectangleShape m_body;
	uint m_status = 0;
	const std::vector<sf::Color>& r_color_lookup;

public:
	const sf::Vector2u m_idx = { 0,0 }; //{x,y}
	std::array<const Cell*, 8> m_neighbors{ nullptr };

	Cell(sf::Vector2u idx, const sf::RectangleShape& cell_shape, uint status, const std::vector<sf::Color>& color_lookup);
	inline const sf::RectangleShape& get_shape() const { return m_body; };
	//void set_position(sf::Vector2i pos); // pos counted from top left corner
	void set_status(uint status);
	const Cell::uint get_status() const;
};