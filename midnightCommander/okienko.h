#include <curses.h>
#include "lista.h"
#include "midnightCommanderDefines.h"

class okienko : public lista
{
public:
	okienko(WINDOW* w, bool arg_active);
	void resize_handler(WINDOW* new_win, bool arg_active);
	void set_active_state(bool state);
private:
	bool active;
	WINDOW* win;
	void border(bool dashed = false);
};

