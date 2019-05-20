#include "engine.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <thread>


#define LOG(x) std::cout << x << "\n"

engine::engine(std::unique_ptr<Base_game> game, const Config& config)
	: m_color_lookup(game->get_color_lookup()), m_game(std::move(game)), m_config(config), border_cell(Cell({ 0u, 0u }, config.cell_size, config.cellboarder_percentage, 0u, m_color_lookup))
{
	if (m_config.fullscreen)
	{
		sf::VideoMode videomode = *(sf::VideoMode::getFullscreenModes().begin());

		m_window.create(videomode, m_game->get_title(), sf::Style::Fullscreen);
		m_window.setPosition(m_config.topleft);
	}
	else
	{
		m_window.create(sf::VideoMode(static_cast<uint>(m_config.botright.x), static_cast<uint>(m_config.botright.y), sf::VideoMode::getDesktopMode().bitsPerPixel), m_game->get_title(), sf::Style::Default ^ sf::Style::Resize);
		m_window.setPosition(m_config.topleft);
	}
	m_window.setFramerateLimit(0);
	
	m_Cells.reserve(static_cast<size_t>(m_window.getSize().y / m_config.cell_size));
	for (uint y = 0; y < m_window.getSize().y / m_config.cell_size; y++)
	{
		m_Cells.emplace_back(std::vector<Cell>());
		m_Cells[y].reserve(static_cast<size_t>(m_window.getSize().x / m_config.cell_size));
		for (uint x = 0; x < m_window.getSize().x / m_config.cell_size; x++)
		{
			m_Cells[y].emplace_back(sf::Vector2u( x, y ), m_cell_size, m_config.cellboarder_percentage, 0u, m_color_lookup);
		}
	}
	m_next_status.resize(m_Cells.size());
	std::for_each(std::begin(m_next_status), std::end(m_next_status), [&](auto& vec) { vec.resize(m_Cells[0].size()); });
	connect_cells();
}

void engine::connect_cells()
{
	/*
	connect cells so they know what their neighbors are
	0 1 2
	7	3
	6 5 4
	*/
	for (uint y = 0; y < m_Cells.size(); y++)
	{
		for (uint x = 0; x < m_Cells[y].size(); x++)
		{
			auto& cell = m_Cells[y][x];

			
			y > 0 ? cell.m_neighbors[0] = &m_Cells[y - 1][x] : cell.m_neighbors[0] = &border_cell;															//0
			y > 0 && x < m_Cells[y].size() - 1? cell.m_neighbors[1] = &m_Cells[y - 1][x + 1] : cell.m_neighbors[1] = &border_cell;							//1
			x < m_Cells[y].size() - 1 ? cell.m_neighbors[2] = &m_Cells[y][x + 1] : cell.m_neighbors[2] = &border_cell;										//2
			y < m_Cells.size() - 1 && x < m_Cells[y].size() - 1 ? cell.m_neighbors[3] = &m_Cells[y + 1][x + 1] : cell.m_neighbors[3] = &border_cell;		//3
			y < m_Cells.size() - 1 ? cell.m_neighbors[4] = &m_Cells[y + 1][x] : cell.m_neighbors[4] = &border_cell;											//4
			y < m_Cells.size() - 1 && x > 0 ? cell.m_neighbors[5] = &m_Cells[y + 1][x - 1] : cell.m_neighbors[5] = &border_cell;							//5
			x > 0 ? cell.m_neighbors[6] = &m_Cells[y][x - 1] : cell.m_neighbors[6] = &border_cell;															//6
			y > 0 && x > 0 ? cell.m_neighbors[7] = &m_Cells[y - 1][x - 1] : cell.m_neighbors[7] = &border_cell;												//7
		}
	}
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
	//sf::Lock lock(mut_window);
	while (m_window.pollEvent(evnt))
	{
		switch (evnt.type)
		{
		case sf::Event::Closed: game_running = false; break;
		case sf::Event::MouseButtonPressed: 
			if (evnt.mouseButton.button == sf::Mouse::Left) mouse_input(); break;
		case sf::Event::KeyPressed:
			switch (evnt.key.code)
			{
			case sf::Keyboard::Escape: game_running = false; break;
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

void engine::draw()
{
	std::chrono::nanoseconds frame_time = std::chrono::nanoseconds(get_time_per_instance<std::nano>(max_fps));
	std::chrono::high_resolution_clock draw_clock;
	std::chrono::time_point last_draw = draw_clock.now() - frame_time;
	

	std::vector<sf::Vertex> vertices(m_Cells.size() * m_Cells[0].size() * 4);
	auto va_it = vertices.begin(); // assigned new every loop in case vector resizes
	sf::VertexBuffer vertex_buffer(sf::Quads);
	vertex_buffer.create(vertices.size());

	m_window.setActive(true);
	while (game_running)
	{
		m_window.clear(m_config.background_color);
		va_it = vertices.begin();
		for (auto y = m_Cells.begin(); y < m_Cells.end(); ++y)
		{
			for (auto x = y->begin(); x < y->end(); ++x)
			{

				std::copy(x->get_vertices().begin(), x->get_vertices().end(), va_it);
				va_it += 4;
			}
		}
		vertex_buffer.update(vertices.data());
		m_window.draw(vertex_buffer);

		std::this_thread::sleep_until(last_draw + frame_time);

		m_window.display();
		last_draw = draw_clock.now();
	}
	m_window.setActive(false);
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
	

	m_window.setActive(false);
	std::thread draw_thread(&engine::draw, this);

	std::chrono::high_resolution_clock update_timer;
	std::chrono::time_point last_update = update_timer.now();
	std::chrono::time_point update_beginning(update_timer.now());
	std::chrono::nanoseconds min_update_time = std::chrono::nanoseconds(get_time_per_instance<std::nano>(m_config.min_ups));

	while (game_running)
	{
		update_beginning = update_timer.now();
		if (!is_paused)
		{
			if (update_timer.now() - last_update > m_config.epoch_time)
			{
				Update();
				last_update = update_timer.now();
			}
		}

		handle_events();
		if(min_update_time < m_config.epoch_time)
			std::this_thread::sleep_for(min_update_time - std::chrono::duration(update_timer.now() - update_beginning));
		else
			std::this_thread::sleep_for(m_config.epoch_time - std::chrono::duration(update_timer.now() - update_beginning));
	}
	draw_thread.join();
	m_window.close();
}




