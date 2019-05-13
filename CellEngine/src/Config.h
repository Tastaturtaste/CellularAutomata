#pragma once
#include <SFML\Graphics.hpp>
#include <string>

struct Config {
	using point = sf::Vector2i;
	using uint = unsigned int;

	std::string title = "Game of Life";
	sf::Color background_color = sf::Color::Black;
	sf::Color cellboarder_color = sf::Color::Transparent;
	float cellboarder_percentage = 0.05f;
	uint cell_size = 20;
	sf::Time epoch_time = sf::milliseconds(500);
	point topleft = { 0,0 };
	point botright = { 600,400 };
	uint framerate_limit = 60;
};