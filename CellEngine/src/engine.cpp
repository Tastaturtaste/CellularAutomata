#include "engine.h"

engine::engine(const Config& config)
	: m_config(config), m_cell_shape({m_config.cell_size, m_config.cell_size})
{
	m_FieldShape = sf::RectangleShape(sf::Vector2f(m_config.botright.y - m_config.topleft.y, m_config.botright.x - m_config.topleft.x));
	m_FieldShape.setFillColor(m_config.background_color);
	m_color_lookup.insert({ 0,m_config.background_color });
	m_window.create(sf::VideoMode(m_FieldShape.getSize().x, m_FieldShape.getSize().y), m_config.title, sf::Style::Default ^ sf::Style::Resize);
	m_window.setFramerateLimit(m_config.framerate_limit);
	m_window.setPosition(m_config.topleft);
	
	m_cell_shape.setOutlineThickness(-m_config.cellboarder_percentage * m_config.cell_size);
	m_cell_shape.setOutlineColor(m_config.cellboarder_color);
	border_cell.set_status(0);
	m_Cells.reserve(m_FieldShape.getSize().y / m_config.cell_size);
	for (uint y = 0; y < m_FieldShape.getSize().y / m_config.cell_size; y++)
	{
		m_Cells.emplace_back(std::vector<Cell>());
		m_Cells[y].reserve(m_FieldShape.getSize().x / m_config.cell_size);
		for (uint x = 0; x < m_FieldShape.getSize().x / m_config.cell_size; x++)
		{
			m_Cells[y].emplace_back({ x, y }, 0);
			m_Cells[y][x].m_body = m_cell_shape.setPosition({ x*m_config.cell_size, y*m_config.cell_size });
		}
	}
	/*
	connect cells so they know what their neighbors are
	0 1 2
	3	4
	5 6 7
	*/
	for (uint y = 0; y < m_Cells.size(); y++)
	{
		for (uint x = 0; x < m_Cells[y].size(); x++)
		{
			auto& cell = m_Cells[y][x];

			y > 0 && x > 0 ? cell.m_neighbors[0] = &m_Cells[y - 1][x - 1]) : cell.m_neighbors[0] = &border_cell);
			y > 0 ? cell.m_neighbors[1] = &m_Cells[y - 1][x]) : cell.m_neighbors[1] = &border_cell);
			y > 0 && x < m_Cells[y].size() - 1 ? cell.m_neighbors[2] = &m_Cells[y - 1][x + 1]) : cell.m_neighbors[2] = &border_cell);
			x > 0 ? cell.m_neighbors[3] = &m_Cells[y][x - 1]) : cell.m_neighbors[3] = &border_cell);
			x < m_Cells[y].size() - 1 ? cell.m_neighbors[4] = &m_Cells[y][x + 1]) : cell.m_neighbors[4] = &border_cell);
			y < m_Cells.size() - 1 && x > 0 ? cell.m_neighbors[5] = &m_Cells[y + 1][x - 1]) : cell.m_neighbors[5] = &border_cell);
			y < m_Cells.size() - 1 ? cell.m_neighbors[6] = &m_Cells[y + 1][x]) : cell.m_neighbors[6] = &border_cell);
			y < m_Cells.size() - 1 && x < m_Cells[y].size() - 1 ? cell.m_neighbors[7] = &m_Cells[y + 1][x + 1]) : cell.m_neighbors[7] = &border_cell);

		}
	}
}

const sf::RectangleShape& engine::getShape() const
{
	return m_FieldShape;
}

Cell & engine::mousepos_to_cell(sf::Vector2f mouse_pos)
{
	return m_Cells[static_cast<uint>(mousePos.y / m_CellSize)][static_cast<uint>(int)(mousePos.x / m_CellSize)];
}

void engine::ClearAll()
{
	for (auto& vec : m_Cells)
		for (auto& cell : vec)
			cell.set_status(0, color_lookup[0]);
}

void engine::Run()
{
	sf::Clock timer;
	sf::Time time = timer.restart();
	sf::Time deltaTime;

	bool isPaused = true;

	while (m_window.isOpen())
	{
		if (!isPaused)
		{
			deltaTime = time - timer.getElapsedTime();
			if (deltaTime < m_config.epoch_time)
			{
				sf::sleep(m_config.epoch_time - deltaTime);
				time = timer.getElapsedTime();
			}
			Update();
		}
		sf::Event evnt;
		while (m_window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed: m_window.close(); break;
			case sf::Event::MouseButtonPressed: if (evnt.mouseButton.button == sf::Mouse::Left) switchCell(sf::Vector2f(evnt.mouseButton.x, evnt.mouseButton.y)); break;
			case sf::Event::KeyPressed:
				switch (evnt.key.code)
				{
				case sf::Keyboard::Space: isPaused = !isPaused; time = timer.getElapsedTime(); break;
				case sf::Keyboard::BackSpace: ClearAll(); break;
				}
			default:
				break;
			}

		}
		m_window.clear(sf::Color::White);
		m_window.draw(m_FieldShape);
		for (auto& row : m_Cells)
			for (auto& cell : row)
				m_window.draw(cell.get_shape());
		m_window.display();
	}
}