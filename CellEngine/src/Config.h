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
	std::vector<sf::Color> color_lookup = { sf::Color::Transparent, sf::Color::Yellow }; // 0 == dead, 1 == alive
	float cellboarder_percentage = 0.05f;
	uint cell_size = 20;
	sf::Time epoch_time = sf::milliseconds(500);
	point topleft = { 0,0 };
	point botright = { 600,400 };
	uint framerate_limit = 60;
	const sf::Time delta_gamespeed = sf::milliseconds(10);
};