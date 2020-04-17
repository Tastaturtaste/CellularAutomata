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

	[[nodiscard]] virtual auto get_color_lookup() const noexcept -> const std::vector<sf::Color>& { return color_lookup; }
	[[nodiscard]] virtual auto get_title() const noexcept -> const std::string& { return title; }
	[[nodiscard]] virtual auto calc_cell_update(const Cell& cell) const -> uint = 0;
	[[nodiscard]] virtual auto on_click_cell(const Cell& cell) const -> uint = 0;
};
