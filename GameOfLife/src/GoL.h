#pragma once
#include "Base_game.h"


class GoL : public Base_game{
private:
	using uint = unsigned int;
	uint max_sustain_neighbors = 3;
	uint min_sustain_neighbors = 2;
	uint max_reproduce_neighbors = 3;
	uint min_reproduce_neighbors = 3;
	
	uint const check_sustain(const Cell& cell) const;		
	uint const check_reproduce(const Cell& cell) const;

public:

	GoL() { Base_game::title = "Game of Life"; Base_game::color_lookup = { {0,70,0,255}, sf::Color::Yellow }; };
	virtual ~GoL() final = default;
	
	virtual const std::vector<sf::Color>& get_color_lookup() const final;
	virtual const std::string& get_title() const final;
	virtual uint calc_cell_update(const Cell& cell) const final;
	virtual uint on_click_cell(const Cell& cell) const final;
};