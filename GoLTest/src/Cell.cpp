#include "Cell.h"

#include <numeric>

Cell::Cell(sf::Vector2f position, uint CellSize, sf::Color ActiveColor, sf::Color InactiveColor)
	: m_ActiveCellColor(ActiveColor), m_InactiveCellColor(InactiveColor), m_CellSize(CellSize)
{
	m_CellShape = sf::RectangleShape(sf::Vector2f(m_CellSize*(1.0f-0.05f), m_CellSize*(1.0f-0.05f)));
	m_CellShape.setFillColor(m_Active ? m_ActiveCellColor : m_InactiveCellColor);
	m_CellShape.setOutlineThickness(0.05f * CellSize);
	m_CellShape.setOutlineColor(sf::Color::Transparent);
}

void Cell::setCellSize(uint CellSize)
{
	m_CellSize = CellSize;
	m_CellShape.setSize(sf::Vector2f(m_CellSize*(1.0f - 0.05f), m_CellSize*(1.0f - 0.05f)));
}

void Cell::Reset()
{
	m_Active = false;
	m_nextActivation = false;
}

void Cell::Activate()
{
	m_Active = true;
	m_nextActivation = false;
}

void Cell::ResolveActivation()
{
	m_Active = m_nextActivation;
}

void Cell::setNeighbors(const std::array<Cell*, 8>& neighbors)
{
	std::copy(neighbors.begin(), neighbors.end(), m_Neighbors.begin());
}

void Cell::ResolveNextActivation()
{
	uint activation = std::accumulate(m_Neighbors.begin(), m_Neighbors.end(), 0, [](uint a, const Cell* cell) {return a + static_cast<uint>(cell->m_Active); });
	if (m_Active)
		m_nextActivation = activation <= maxSustainNeighbors && activation >= minSustainNeighbors;
	else
		m_nextActivation = activation <= maxRebirthNeighbors && activation >= minRebirthNeighbors;
}

void Cell::setPosition(const sf::Vector2f& position)
{
	m_Position = position;
	m_CellShape.setPosition(position);
}

sf::Vector2f Cell::getPostion() const
{
	return m_Position;
}

const sf::RectangleShape& Cell::getShape()
{
	UpdateShape();
	return m_CellShape;
}

void Cell::switchActiv()
{
	m_Active = !m_Active;
}

void Cell::UpdateShape()
{
	sf::Color fillColor;
	m_Active ? fillColor = m_ActiveCellColor : fillColor = m_InactiveCellColor;
	m_CellShape.setFillColor(fillColor);
}
