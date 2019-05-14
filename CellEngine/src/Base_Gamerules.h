#pragma once

class Cell;

class Base_gamerules {
	using uint = unsigned int;
public:
	Base_gamerules() = default;
	virtual ~Base_gamerules() = default;
	virtual uint calc_cell_update(const Cell& cell) = 0;
	virtual uint on_click_cell(const Cell& cell) = 0;
};