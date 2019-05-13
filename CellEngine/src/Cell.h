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

	const sf::Vector2u m_position = { 0,0 }; //{x,y}
	uint m_status = 0;
	sf::RectangleShape m_body;
	std::array<const Cell*, 8> m_neighbors{ nullptr };

public:
	Cell(sf::Vector2f position, uint status);
	inline void set_shape(sf::RectangleShape shape) { m_body = shape; };
	inline const sf::RectangleShape& get_shape() { return m_body; };
	void set_position(sf::Vector2i pos); // pos counted from top left corner
	void set_status(uint status, sf::Color color);
	Cell::uint get_status();
};