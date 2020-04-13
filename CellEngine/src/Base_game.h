#pragma once
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>


class Cell;

class Base_game {
private:
	std::string title;
	std::vector<sf::Color> color_lookup;

protected:
	using uint = unsigned int;
	void set_color_lookup(std::vector<sf::Color> new_color_lookup) { color_lookup = new_color_lookup; }
	void set_title(std::string_view new_title) { title = new_title; }
public:
	
	virtual ~Base_game() = default;

	virtual const std::vector<sf::Color>& get_color_lookup() const noexcept { return color_lookup; }
	virtual const std::string& get_title() const noexcept { return title; }
	virtual uint calc_cell_update(const Cell& cell) const = 0;
	virtual uint on_click_cell(const Cell& cell) const = 0;
};
