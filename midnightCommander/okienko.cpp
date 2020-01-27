#include "okienko.h"

okienko::okienko(WINDOW* w, bool arg_active)
{
	active = false;
	resize_handler(w, arg_active);
}
void okienko::resize_handler(WINDOW* new_win, bool arg_active)
{
	win = new_win;
	wbkgd(win, COLOR_PAIR(COLOR_MC));
	set_active_state(arg_active);
	int height, width;
	getmaxyx(win, height, width);
	WINDOW* list_window = derwin(win, height - 2, width - 2, 1, 1);
	list_change_window(list_window);
	//list_refresh();
}
void okienko::set_active_state(bool state)
{
	active = state;
	if (active)
		wattron(win, A_DIM);
	else
		wattroff(win, A_DIM);
	border(active);
}
void okienko::border(bool dashed)
{
	attron(COLOR_PAIR(COLOR_MC));
	//dashed if (not)active - information
	if (dashed)
		wborder(win, '|', '|', '-', '-', '0', '*', '*', '*');
	else
		box(win, 0, 0);
	wrefresh(win);
}

