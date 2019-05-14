#pragma once
#include "Base_Gamerules.h"

class GoL : public Base_gamerules{
private:
	using uint = unsigned int;
	uint max_sustain_neighbors = 3;
	uint min_sustain_neighbors = 2;
	uint max_reproduce_neighbors = 3;
	uint min_reproduce_neighbors = 3;

	uint check_sustain(const Cell& cell);		
	uint check_reproduce(const Cell& cell);

public:
	GoL() = default;
	virtual ~GoL() final = default;
	virtual uint calc_cell_update(const Cell& cell) final;
	virtual uint on_click_cell(const Cell& cell) final;
};