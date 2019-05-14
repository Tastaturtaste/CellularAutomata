#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>

struct Config {
	using point = sf::Vector2i;
	using uint = unsigned int;

	std::string title = "title";
	sf::Color background_color = sf::Color::Black;
	sf::Color cellboarder_color = sf::Color::Red;
	float cellboarder_percentage = 0.05f;
	uint cell_size = 20;
	sf::Time epoch_time = sf::milliseconds(500);
	bool fullscreen = true;
	point topleft = { 0,0 };
	point botright = { 1200,800 };
	uint framerate_limit = 60;
	const sf::Time delta_gamespeed = sf::milliseconds(10);
};