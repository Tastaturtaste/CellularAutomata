#include "Cell.h"

#include <numeric>

Cell::Cell(sf::Vector2f position, uint status)
	: m_position(position), m_status(status), m_body()
{
}

void Cell::set_position(sf::Vector2i pos)
{
	m_position = pos;
	m_body.setPosition({ pos.x * m_body.getSize(), pos.y * m_body.getSize() });
}

void Cell::set_status(uint status, sf::Color color)
{
	m_status = status;
	m_body.setFillColor(color);
}

Cell::uint Cell::get_status()
{
	return m_status;
}
