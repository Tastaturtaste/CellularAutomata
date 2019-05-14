#include "engine.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#define LOG(x) std::cout << x << "\n"

engine::engine(std::unique_ptr<Base_game> game, const Config& config)
	: m_color_lookup(game->get_color_lookup()), m_game(std::move(game)), m_config(config), m_cell_shape(sf::Vector2f(static_cast<float>(config.cell_size), static_cast<float>(config.cell_size))), border_cell(Cell({ 0u, 0u }, m_cell_shape, 0u, m_color_lookup))
{
	if (m_config.fullscreen)
	{
		sf::VideoMode videomode = *(sf::VideoMode::getFullscreenModes().begin());
		m_FieldShape = sf::RectangleShape(sf::Vector2f(static_cast<float>(videomode.width), static_cast<float>(videomode.height)));
		m_FieldShape.setPosition(sf::Vector2f(0.0f,0.0f));

		m_window.create(videomode, m_game->get_title(), sf::Style::Fullscreen);
		m_window.setPosition(m_config.topleft);

	}
	else
	{
		m_FieldShape = sf::RectangleShape(sf::Vector2f(static_cast<float>(m_config.botright.x - m_config.topleft.x), static_cast<float>(m_config.botright.y - m_config.topleft.y)));
		m_FieldShape.setPosition(static_cast<sf::Vector2f>(m_config.topleft));

		m_window.create(sf::VideoMode(static_cast<uint>(m_FieldShape.getSize().x), static_cast<uint>(m_FieldShape.getSize().y), sf::VideoMode::getDesktopMode().bitsPerPixel), m_game->get_title(), sf::Style::Default ^ sf::Style::Resize);
		m_window.setPosition(m_config.topleft);
	}
	m_FieldShape.setFillColor(m_config.background_color);
	m_window.setFramerateLimit(m_config.framerate_limit);
	
	m_cell_shape.setOutlineThickness(-m_config.cellboarder_percentage * m_config.cell_size);
	m_cell_shape.setOutlineColor(m_config.cellboarder_color);
	
	m_Cells.reserve(static_cast<size_t>(m_FieldShape.getSize().y / m_config.cell_size));
	for (uint y = 0; y < m_FieldShape.getSize().y / m_config.cell_size; y++)
	{
		m_Cells.emplace_back(std::vector<Cell>());
		m_Cells[y].reserve(static_cast<size_t>(m_FieldShape.getSize().x / m_config.cell_size));
		for (uint x = 0; x < m_FieldShape.getSize().x / m_config.cell_size; x++)
		{
			m_cell_shape.setPosition(sf::Vector2f(static_cast<float>(x * m_cell_size), static_cast<float>(y * m_cell_size)));
			m_Cells[y].emplace_back(sf::Vector2u( x, y ), m_cell_shape, 0u, m_color_lookup);
		}
	}
	m_next_status.resize(m_Cells.size());
	std::for_each(std::begin(m_next_status), std::end(m_next_status), [&](auto& vec) {vec.resize(m_Cells[0].size()); });
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

			y > 0 && x > 0 ? cell.m_neighbors[0] = &m_Cells[y - 1][x - 1] : cell.m_neighbors[0] = &border_cell;
			y > 0 ? cell.m_neighbors[1] = &m_Cells[y - 1][x] : cell.m_neighbors[1] = &border_cell;
			y > 0 && x < m_Cells[y].size() - 1 ? cell.m_neighbors[2] = &m_Cells[y - 1][x + 1] : cell.m_neighbors[2] = &border_cell;
			x > 0 ? cell.m_neighbors[3] = &m_Cells[y][x - 1] : cell.m_neighbors[3] = &border_cell;
			x < m_Cells[y].size() - 1 ? cell.m_neighbors[4] = &m_Cells[y][x + 1] : cell.m_neighbors[4] = &border_cell;
			y < m_Cells.size() - 1 && x > 0 ? cell.m_neighbors[5] = &m_Cells[y + 1][x - 1] : cell.m_neighbors[5] = &border_cell;
			y < m_Cells.size() - 1 ? cell.m_neighbors[6] = &m_Cells[y + 1][x] : cell.m_neighbors[6] = &border_cell;
			y < m_Cells.size() - 1 && x < m_Cells[y].size() - 1 ? cell.m_neighbors[7] = &m_Cells[y + 1][x + 1] : cell.m_neighbors[7] = &border_cell;

		}
	}
}

const sf::RectangleShape& engine::getShape() const
{
	return m_FieldShape;
}

Cell & engine::mousepos_to_cell(sf::Vector2i mouse_pos)
{
	return m_Cells[mouse_pos.y / m_cell_size][mouse_pos.x / m_cell_size];
}

void engine::ClearAll()
{
	for (auto& vec : m_Cells)
		for (auto& cell : vec)
			cell.set_status(0);
}

void engine::handle_events()
{
	sf::Event evnt;
	while (m_window.pollEvent(evnt))
	{
		switch (evnt.type)
		{
		case sf::Event::Closed: m_window.close(); break;
		case sf::Event::MouseButtonPressed: 
			if (evnt.mouseButton.button == sf::Mouse::Left) mouse_input(); break;
		case sf::Event::KeyPressed:
			switch (evnt.key.code)
			{
			case sf::Keyboard::Escape: m_window.close(); break;
			case sf::Keyboard::Space: switch_pause(); break;
			case sf::Keyboard::BackSpace: ClearAll(); break;
			case sf::Keyboard::Right: m_config.epoch_time -= m_config.delta_gamespeed; break;
			case sf::Keyboard::Left: m_config.epoch_time += m_config.delta_gamespeed; break;
			}
		default:
			break;
		}
	}
}

void engine::mouse_input()
{
	static std::unordered_set<const Cell*> cells;
	static Cell* current_cell;
	while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		current_cell = &mousepos_to_cell(sf::Mouse::getPosition(m_window));
		if (cells.find(current_cell) == cells.end())
		{
			current_cell->set_status(m_game->on_click_cell(*current_cell));
			cells.insert(current_cell);
		}
	}
	cells.clear();
	current_cell = nullptr;
}

void engine::switch_pause()
{
	is_paused = !is_paused;
}

void engine::update_cells()
{
	for (size_t y = 0; y < m_Cells.size(); y++)
	{
		for (size_t x = 0; x < m_Cells[y].size(); x++)
		{
			m_Cells[y][x].set_status(m_next_status[y][x]);
		}
	}
	return;
}

void engine::Update()
{
	for(uint y = 0; y < m_Cells.size(); y++)
		for(uint x = 0; x < m_Cells[y].size(); x++)
			m_next_status[y][x] = m_game->calc_cell_update(m_Cells[y][x]);
	update_cells();
}

void engine::Run()
{
	sf::Time time = timer.restart();
	sf::Time deltaTime;

	while (m_window.isOpen())
	{
		if (!is_paused)
		{
			deltaTime = timer.getElapsedTime();
			if (deltaTime > m_config.epoch_time)
			{
				Update();
				timer.restart();
			}
		}
		
		handle_events();
		m_window.clear(sf::Color::White);
		m_window.draw(m_FieldShape);
		for (auto& row : m_Cells)
			for (auto& cell : row)
				m_window.draw(cell.get_shape());
		m_window.display();
	}
}