#include "GoL.h"
#include "Cell.h"

#include <numeric>

GoL::uint GoL::check_reproduce(const Cell & cell)
{
	uint neighbors = std::accumulate(std::begin(cell.m_neighbors), std::end(cell.m_neighbors), 0u, [](uint a, const Cell* p_cell) {return a + p_cell->get_status(); });
	return neighbors <= max_reproduce_neighbors && neighbors >= min_reproduce_neighbors ? 1u : 0u;
}

GoL::uint GoL::check_sustain(const Cell& cell)
{
	uint neighbors = std::accumulate(std::begin(cell.m_neighbors), std::end(cell.m_neighbors), 0u, [](uint a, const Cell const* p_cell) {return a + p_cell->get_status(); });
	return neighbors <= max_sustain_neighbors && neighbors >= min_sustain_neighbors ? 1u : 0u;
}

GoL::uint GoL::calc_cell_update(const Cell & cell)
{
	if(cell.get_status() == 0u) return check_reproduce(cell);
	if(cell.get_status() == 1u) return check_sustain(cell);
}

GoL::uint GoL::on_click_cell(const Cell & cell)
{
	return cell.get_status() == 0u ? 1u : 0u;
}
