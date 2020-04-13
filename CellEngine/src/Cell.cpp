#include "Cell.h"
#include <SFML\Graphics.hpp>

#include <numeric>

Cell::Cell(sf::Vector2u idx, uint cell_size, float border_percentage, uint status, const std::vector<sf::Color>& color_lookup)
	: m_status(status), r_color_lookup(color_lookup), m_idx(idx)
{
	const float pixelX = idx.x * cell_size + cell_size * (border_percentage / 2);
	const float pixelY = idx.y * cell_size + cell_size * (border_percentage / 2);
	const float core_size = cell_size * (1.f - border_percentage);
	m_vertices[0].position = { pixelX				,	pixelY };
	m_vertices[1].position = { pixelX + core_size	,	pixelY };
	m_vertices[2].position = { pixelX + core_size	,	pixelY + core_size };
	m_vertices[3].position = { pixelX				,	pixelY + core_size };
	set_color(m_status);
}

void Cell::set_status(const uint status) noexcept
{
	m_status = status;
	set_color(status);
}

const Cell::uint Cell::get_status() const noexcept
{
	return m_status;
}

