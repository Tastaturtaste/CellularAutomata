#include "Wireworld.h"
#include "Cell.h"

Wireworld::Wireworld()
{
	const auto default_background = sf::Color({ 125,100,50,255 });
	set_title("Wireworld"); 
	set_color_lookup({ default_background,  sf::Color::Yellow, sf::Color::Red, sf::Color::Blue });
}

Base_game::uint Wireworld::calc_cell_update(const Cell & cell) const
{
	short step = diagonal_neighbors ? 1 : 2;
	uint8_t electron_count = 0;
	switch (cell.get_status())
	{
	case Cell_status::isolator: 
		return Cell_status::isolator;
		break;
	case Cell_status::conductor: 
		for (auto iter = std::begin(cell.m_neighbors); iter < std::end(cell.m_neighbors); iter += step)
		{
			if ((*iter)->get_status() == Cell_status::electron_head)
			{
				++electron_count;
			}
		}
		if (electron_count > 0 && electron_count <= electron_switch) return Cell_status::electron_head;
		else return Cell_status::conductor;
		break;  
	case Cell_status::electron_head:
		return Cell_status::electron_tail;
		break;
	case Cell_status::electron_tail:
		return Cell_status::conductor;
		break;
	}
	static_assert(true, "Cellstatus not handled!\n");
}

Base_game::uint Wireworld::on_click_cell(const Cell & cell) const
{
	switch (cell.get_status())
	{
	case Cell_status::isolator: 
		return Cell_status::conductor;
		break;
	case Cell_status::conductor:
		return Cell_status::electron_head;
		break;
	case Cell_status::electron_head:
		return Cell_status::isolator;
		break;
	case Cell_status::electron_tail:
		return Cell_status::conductor;
		break;
	}
	static_assert(true, "Cellstatus not handled!\n");
}
