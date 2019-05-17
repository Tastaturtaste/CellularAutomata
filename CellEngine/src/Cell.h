#pragma once

#include <array>
#include <SFML\Graphics.hpp>
#include <array>
#include "Config.h"


/*
	Cells know their current status, their shape, their neighbors, and their position
*/
class Cell
{
private:
	using uint = unsigned int;

	uint m_status = 0;
	const std::vector<sf::Color>& r_color_lookup;
	std::array<sf::Vertex,4> m_vertices;

	inline void set_color(uint status) {
		for (auto& vertex : m_vertices)
			vertex.color = r_color_lookup[status];
	}

public:
	const sf::Vector2u m_idx = { 0,0 }; //{x,y}
	/*
	0 1 2
	7	3
	6 5 4
	*/
	std::array<const Cell*, 8> m_neighbors{ nullptr };

	Cell(sf::Vector2u idx, uint cell_size, float border_size, uint status, const std::vector<sf::Color>& color_lookup);
	inline const std::array<sf::Vertex,4>& get_vertices() const { return m_vertices; };
	void set_status(const uint status);
	const Cell::uint get_status() const;
};