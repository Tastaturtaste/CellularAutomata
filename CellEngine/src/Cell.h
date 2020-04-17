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

	inline void set_color(uint status) noexcept {
		for (auto& vertex : m_vertices) {
			vertex.color = r_color_lookup[status];
}
	}
	const sf::Vector2u m_idx = { 0,0 }; //{x,y}

public:
	/*
	0 1 2
	7	3
	6 5 4
	*/
	std::array<const Cell*, 8> m_neighbors{ nullptr };
	[[nodiscard]] inline auto get_index() const noexcept { return m_idx; }
	inline void set_index() = delete;
	Cell(sf::Vector2u idx, uint cell_size, float border_size, uint status, const std::vector<sf::Color>& color_lookup);
	[[nodiscard]] inline auto get_vertices() const noexcept -> const std::array<sf::Vertex,4>& { return m_vertices; };
	void set_status(uint status) noexcept;
	[[nodiscard]] auto get_status() const noexcept -> const Cell::uint;
};