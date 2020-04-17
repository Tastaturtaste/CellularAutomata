#pragma once
#include "Base_game.h"


class GoL final : public Base_game{
private:
	using uint = unsigned int;
	uint max_sustain_neighbors = 3;
	uint min_sustain_neighbors = 2;
	uint max_reproduce_neighbors = 3;
	uint min_reproduce_neighbors = 3;
	
	uint const check_sustain(const Cell& cell) const;		
	uint const check_reproduce(const Cell& cell) const;

public:

	GoL() ;
	~GoL() final = default;
	
	uint calc_cell_update(const Cell& cell) const final;
	uint on_click_cell(const Cell& cell) const final;
};