#pragma once

#include "Base_game.h"

class Wireworld final : public Base_game {
private:
	enum Cell_status : uint {isolator = 0, conductor = 1, electron_head = 2, electron_tail = 3};
	uint8_t electron_switch = 2;
	const bool diagonal_neighbors = true;

public:
	Wireworld();
	~Wireworld() final = default;

	uint calc_cell_update(const Cell& cell) const final;
	uint on_click_cell(const Cell& cell) const final;
};

