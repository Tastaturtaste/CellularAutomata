#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "Cell.h"

class GameOfLife
{
	using uint = unsigned int;

	uint m_FieldHeight;
	uint m_FieldWidth;
	uint m_CellSize ;
	sf::Color m_FieldBackgroundColor;
	sf::Color m_FieldLineColor;
	sf::Color m_ActiveCellColor;
	sf::RectangleShape m_FieldShape;
	std::vector<std::vector<Cell>> m_Cells;
	sf::RenderWindow GameField;
	Cell BorderCell;
	sf::Time m_GameloopDelta = sf::milliseconds(500);

public:
	GameOfLife(uint height = 600, uint width = 600, uint CellSize = 20, sf::Color backgroundColor = sf::Color::Black, sf::Color lineColor = sf::Color::Red, sf::Color ActiveCellColor = sf::Color::Yellow);
	const sf::RectangleShape getShape() const;
	void setBackground(sf::Color color);
	void ClearAll();
	void ActivateAll();
	void Update();
	void switchCell(sf::Vector2f mousePos);
	void Initialize();
	void Run();

};

