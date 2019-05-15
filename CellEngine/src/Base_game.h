#pragma once
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>


class Cell;

class Base_game {
protected:
	using uint = unsigned int;

	std::string title;
	std::vector<sf::Color> color_lookup;

public:

	Base_game() = default;
	virtual ~Base_game() = default;
	
	virtual const std::vector<sf::Color>& get_color_lookup() const = 0;
	virtual const std::string& get_title() const = 0;
	virtual uint calc_cell_update(const Cell& cell) const = 0;
	virtual uint on_click_cell(const Cell& cell) const = 0;
};