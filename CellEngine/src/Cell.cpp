#include "Cell.h"

#include <numeric>

Cell::Cell(sf::Vector2u idx, const sf::RectangleShape& cell_shape, uint status, const std::vector<sf::Color>& color_lookup)
	: m_body(cell_shape), m_status(status), r_color_lookup(color_lookup), m_idx(idx)
{
	m_body.setFillColor(r_color_lookup[status]);
}

void Cell::set_status(const uint status)
{
	m_status = status;
	m_body.setFillColor(r_color_lookup[status]);
}

const Cell::uint Cell::get_status() const
{
	return m_status;
}
