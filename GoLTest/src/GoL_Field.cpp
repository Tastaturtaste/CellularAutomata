#include "GoL_Field.h"

GameOfLife::GameOfLife(point topleft, point botright, uint CellSize, sf::Color backgroundColor, sf::Color lineColor, sf::Color ActiveCellColor)
	: m_FieldWidth(botright.x - topleft.x), m_FieldHeight(botright.y - topleft.y), m_CellSize(CellSize), m_FieldBackgroundColor(backgroundColor), m_FieldLineColor(lineColor),
	m_ActiveCellColor(ActiveCellColor)
{
	m_FieldShape = sf::RectangleShape(sf::Vector2f(m_FieldWidth, m_FieldHeight));
	m_FieldShape.setFillColor(m_FieldBackgroundColor);
	m_Cells.resize(m_FieldHeight / CellSize);
	for (uint i = 0; i < m_Cells.size(); i++)
	{
		m_Cells[i].resize(m_FieldWidth / CellSize);
	}
	for (uint i = 0; i < m_Cells.size(); i++)
		for (uint j = 0; j < m_Cells[i].size(); j++)
		{
			m_Cells[i][j].setCellSize(m_CellSize);
			m_Cells[i][j].setPosition(sf::Vector2f(j*CellSize, i*CellSize));
		}
	GameField.create(sf::VideoMode(m_FieldWidth, m_FieldHeight), "Game of Life", sf::Style::Default ^ sf::Style::Resize);
	GameField.setFramerateLimit(60);
	GameField.setPosition(static_cast<sf::Vector2i>(topleft));

	// connect cells so they know what their neighbors are
	for (uint i = 0; i < m_Cells.size(); i++)
	{
		for (uint j = 0; j < m_Cells[i].size(); j++)
		{
			auto& cell = m_Cells[i][j];

			i > 0 && j > 0 ? cell.setNeighbor(0, &m_Cells[i - 1][j - 1]) : cell.setNeighbor(0, &BorderCell);
			i > 0 ? cell.setNeighbor(1, &m_Cells[i - 1][j]) : cell.setNeighbor(1, &BorderCell);
			i > 0 && j < m_Cells[i].size() - 1 ? cell.setNeighbor(2, &m_Cells[i - 1][j + 1]) : cell.setNeighbor(2, &BorderCell);
			j > 0 ? cell.setNeighbor(3, &m_Cells[i][j - 1]) : cell.setNeighbor(3, &BorderCell);
			j < m_Cells[i].size() - 1 ? cell.setNeighbor(4, &m_Cells[i][j + 1]) : cell.setNeighbor(4, &BorderCell);
			i < m_Cells.size() - 1 && j > 0 ? cell.setNeighbor(5, &m_Cells[i + 1][j - 1]) : cell.setNeighbor(5, &BorderCell);
			i < m_Cells.size() - 1 ? cell.setNeighbor(6, &m_Cells[i + 1][j]) : cell.setNeighbor(6, &BorderCell);
			i < m_Cells.size() - 1 && j < m_Cells[i].size() - 1 ? cell.setNeighbor(7, &m_Cells[i + 1][j + 1]) : cell.setNeighbor(7, &BorderCell);

		}
	}
}

const sf::RectangleShape GameOfLife::getShape() const
{
	return m_FieldShape;
}

void GameOfLife::setBackground(sf::Color color)
{
	m_FieldBackgroundColor = color;
}

void GameOfLife::Update()
{
	m_FieldShape.setFillColor(m_FieldBackgroundColor);
	for (uint i = 0; i < m_Cells.size(); i++)
		for (uint j = 0; j < m_Cells[i].size(); j++)
			m_Cells[i][j].ResolveNextActivation();
	for (uint i = 0; i < m_Cells.size(); i++)
		for (uint j = 0; j < m_Cells[i].size(); j++)
			m_Cells[i][j].ResolveActivation();
}

void GameOfLife::switchCell(sf::Vector2f mousePos)
{
	m_Cells[(int)(mousePos.y / m_CellSize)][(int)(mousePos.x / m_CellSize)].switchActiv();
}

void GameOfLife::ClearAll() 
{
	for (uint i = 0; i < m_Cells.size(); i++)
		for (uint j = 0; j < m_Cells[i].size(); j++)
			m_Cells[i][j].Reset();
}

void GameOfLife::ActivateAll()
{
	for (uint i = 0; i < m_Cells.size(); i++)
		for (uint j = 0; j < m_Cells[i].size(); j++)
			m_Cells[i][j].Activate();
}


void GameOfLife::Run()
{
	sf::Clock timer;
	sf::Time time = timer.restart();
	sf::Time deltaTime;

	bool isPaused = true;

	while (GameField.isOpen())
	{
		if (!isPaused)
		{
			deltaTime = time - timer.getElapsedTime();
			if (deltaTime < m_GameloopDelta)
			{
				sf::sleep(m_GameloopDelta - deltaTime);
				time = timer.getElapsedTime();
			}
			Update();
		}
		sf::Event evnt;
		while (GameField.pollEvent(evnt))
		{
			switch (evnt.type)
			{
				case sf::Event::Closed: GameField.close(); break;
				case sf::Event::MouseButtonPressed: if(evnt.mouseButton.button == sf::Mouse::Left) switchCell(sf::Vector2f(evnt.mouseButton.x, evnt.mouseButton.y)); break;
				case sf::Event::KeyPressed: 
					switch (evnt.key.code)
					{
					case sf::Keyboard::Space: isPaused = !isPaused; time = timer.getElapsedTime(); break;
					case sf::Keyboard::BackSpace: ClearAll(); break;
					case sf::Keyboard::Enter: ActivateAll(); break;
					}
				default:
					break;
			}

		}
		GameField.clear(sf::Color::White);
		GameField.draw(m_FieldShape);
		for (auto row : m_Cells)
			for (auto cell : row)
				GameField.draw(cell.getShape());
		GameField.display();
	}
}
