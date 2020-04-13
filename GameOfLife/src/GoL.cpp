#include "GoL.h"
#include "Cell.h"

#include <numeric>

GoL::GoL() { 
	set_title("Game of Life");
	set_color_lookup({ {0,70,0,255}, sf::Color::Yellow });
}

GoL::uint const GoL::check_reproduce(const Cell & cell) const
{
	uint neighbors = std::accumulate(std::begin(cell.m_neighbors), std::end(cell.m_neighbors), 0u, [](uint a, const Cell* const p_cell) {return a + p_cell->get_status(); });
	return neighbors <= max_reproduce_neighbors && neighbors >= min_reproduce_neighbors ? 1u : 0u;
}

GoL::uint const GoL::check_sustain(const Cell& cell) const
{
	uint neighbors = std::accumulate(std::begin(cell.m_neighbors), std::end(cell.m_neighbors), 0u, [](uint a, const Cell* const p_cell) {return a + p_cell->get_status(); });
	return neighbors <= max_sustain_neighbors && neighbors >= min_sustain_neighbors ? 1u : 0u;
}

GoL::uint GoL::calc_cell_update(const Cell & cell) const
{
	if(cell.get_status() == 0u) return check_reproduce(cell);
	if(cell.get_status() == 1u) return check_sustain(cell);
	static_assert(true, "Current Cellstatus doesn't get checked!\n");
	return cell.get_status();
}

GoL::uint GoL::on_click_cell(const Cell & cell) const
{
	return cell.get_status() == 0u ? 1u : 0u;
}
