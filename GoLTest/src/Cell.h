#pragma once

#include <array>

#include "SFML/Graphics.hpp"

class Cell
{
	using uint = unsigned int;
	sf::Color m_ActiveCellColor;
	sf::Color m_InactiveCellColor;
	uint m_CellSize;
	sf::RectangleShape m_CellShape;
	bool m_Active = false; // Set back to false
	bool m_nextActivation = false;
	std::array<const Cell*, 8> m_Neighbors{ nullptr };
	
	//Rules for the Game of Life
	uint minSustainNeighbors = 2;
	uint maxSustainNeighbors = 3;
	uint minRebirthNeighbors = 3;
	uint maxRebirthNeighbors = 3;
	
	sf::Vector2f m_Position{ 0,0 };

public:
	Cell(sf::Vector2f position = sf::Vector2f(0, 0), uint CellSize = 40, sf::Color ActiveColor = sf::Color::Yellow, sf::Color InactiveColor = sf::Color(0, 255, 0, 30));
	void setCellSize(uint CellSize);
	void Reset(); // Resets the activation of the cell
	void Activate(); // Activates the cell and resets nextActivation
	void ResolveActivation();
	void setNeighbors(const std::array<Cell*, 8>& neighbors);
	void ResolveNextActivation();
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPostion() const;
	const sf::RectangleShape& getShape();
	void switchActiv();
	void UpdateShape();
};