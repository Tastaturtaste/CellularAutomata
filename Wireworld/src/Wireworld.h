#pragma once

#include "Base_game.h"

class Wireworld : public Base_game {
private:
	enum Cell_status : uint {isolator = 0, conductor = 1, electron_head = 2, electron_tail = 3};
	uint8_t electron_switch = 2;
	const bool diagonal_neighbors = true;

public:
	Wireworld();
	virtual ~Wireworld() final = default;

	virtual const std::vector<sf::Color>& get_color_lookup() const final { return Base_game::color_lookup; };
	virtual const std::string& get_title() const final { return Base_game::title; };
	virtual uint calc_cell_update(const Cell& cell) const final;
	virtual uint on_click_cell(const Cell& cell) const final;
};
