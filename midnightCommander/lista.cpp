// lista.cpp
#include "lista.h"
#include "midnightCommanderDefines.h"
using namespace std;

bool lista::is_directory(int id)
{
	return (folder_content_vect[id].attrib & _A_SUBDIR) == _A_SUBDIR;
}
string lista::get_editor()
{
	//https://stackoverflow.com/questions/15916695/can-anyone-give-me-example-code-of-dupenv-s/15916732
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "EDITOR") == 0 && buf != nullptr)
	{
		//need to restart VS to update env vars
		//if more than MAX_PATH then consider failure and use default
		if (sz < MAX_PATH) {
			string return_val = string(buf);
			free(buf);
			return return_val;
		}
	}
	return("C:\\Windows\\System32\\notepad.exe");
}
void lista::draw()
{	
	
	wattron(list_window,COLOR_PAIR(COLOR_MC));
	wclear(list_window);
	//debug();
	for (int i = 1; i <= min(list_entries, list_height); i++) {
		//if a directory
		if (is_directory(i + downshift))
			wattron(list_window, A_BLINK);
		else
			wattroff(list_window, A_BLINK);
		//if a choosen one
		if (i + downshift == choice)
			wattron(list_window, A_REVERSE);
		else
			wattroff(list_window, A_REVERSE);
		//.. na końcu zbyt długiego tekstu
		if (int(string(folder_content_vect[i + downshift].name).length()) > list_width)
			mvwprintw(list_window, i - 1, 0, "%.*s..", list_width - 2, folder_content_vect[i + downshift].name);
		else
			mvwprintw(list_window, i - 1, 0, "%s", folder_content_vect[i + downshift].name);
	}
	wrefresh(list_window);
}
string lista::windows_get_current_directory()
{
	//https://stackoverflow.com/questions/875249/how-to-get-current-directory
	// The directory path returned by native GetCurrentDirectory() no end backslash
	const unsigned long maxDir = 260;
	char currentDir[maxDir];
	GetCurrentDirectory(maxDir, currentDir);
	return string(currentDir);
}
int lista::get_current_selection()
{
	return choice;
}
lista::lista()
{
	init_pair(COLOR_MC, COLOR_WHITE, COLOR_BLUE);
	folder_content_vect.clear();
	list_entries = 0;
	list_height = 0;
	list_width = 0;
	choice = 0;
	downshift = 0;
	list_window = NULL;
	current_directory = "";
	searching = false;
}
void lista::list_change_window(WINDOW* win)
{
	list_window = win;
	init_window_dimensions();
	if (current_directory == "")current_directory = windows_get_current_directory(); //if used when no direcory is set (used default constructor)
	list_refresh();//can't simply draw - the downshift an choice could get messed up then
}
void lista::init_window_dimensions()
{
	getmaxyx(list_window, list_height, list_width);
}
void lista::list_refresh()
{
	//przy wczytywaniu '.' też się ładuje do vektora (na indexie 0)
	_finddata_t data;
	int ff = _findfirst((current_directory + "\\*.*").c_str(), &data);
	if (ff == -1) {
		//nie udało się otworzyć katalogu
		folder_up();
		//ff = _findfirst((current_directory + "\\*.*").c_str(), &data);
		mvwprintw(stdscr, 29, 0, "%s", "access denied");
		wrefresh(stdscr);
	}
	else {
		//zeruję, tylko gdy udało się wczytać nowy katalog
		folder_content_vect.clear();
		list_entries = 0;
		if (ff != -1)
		{
			int res = 0;
			while (res != -1) {
				folder_content_vect.push_back(data);
				list_entries++;
				res = _findnext(ff, &data);
			}
			_findclose(ff);
			//bo jakoś tak wychodzi o jedno za dużo ;)
			list_entries--;
			choice = 1;
			downshift = 0;
			draw();
		}
		else {
			mvwprintw(stdscr, 29, 0, "%s", "fix failed - error!");
			wrefresh(stdscr);
			getch();
		}
	}
}
string lista::get_current_selection_name()
{
	return folder_content_vect[get_current_selection()].name;
}
string lista::get_current_directory()
{
	return current_directory;
}
bool lista::is_selected_directory()
{
	return is_directory(get_current_selection());
}
void lista::up()
{
	if (downshift) {
		if (choice == downshift + 1) {
			choice--;
			downshift--;
		}
		else {
			choice--;
		}
	}
	else {
		choice--;
		if (choice < 1) {
			choice = list_entries;
			if (list_entries < list_height) {
				downshift = 0;
			}
			else {
				downshift = list_entries - list_height;
			}
		}
	}
	if (!searching)draw();
}
void lista::down()
{
	if (choice - downshift == list_height)downshift++;
	choice++;
	if (choice > list_entries) {
		choice = 1;
		downshift = 0;
	}
	if (!searching)draw();
}
void lista::folder_up()
{
	current_directory = current_directory.substr(0, current_directory.find_last_of("\\"));
}
void lista::choose()
{
	string selection = get_current_selection_name();
	if (is_directory(get_current_selection())) {
		//folder wyzej
		if (selection == "..") {
			folder_up();
			list_refresh();
		}
		//folder glebiej
		else {
			current_directory += "\\" + selection;
			list_refresh();

		}
	}
	else {
		//try to edit
		//system(("cd " + current_directory).c_str());
		string command = "\"" + get_editor() + "\" \"" + current_directory + "\\" + selection + "\"";
		command = "\"" + get_editor() + "\" " + current_directory + "\\" + selection;

		//system(command.c_str());
		//mvwprintw(stdscr, 29, 1, "%s", command.c_str());
		//wrefresh(stdscr);
		//getch();
		ShellExecute(NULL, "open", get_editor().c_str(), ("\"" + current_directory + "\\" + selection + "\"").c_str(), NULL, SW_SHOWDEFAULT);
	}
}
void lista::debug()
{
	move(29, 0);
	wclrtoeol(stdscr);
	mvwprintw(stdscr, 29, 0, "%d", downshift);
	mvwprintw(stdscr, 29, 6, "%d", choice);
	mvwprintw(stdscr, 29, 10, "%d", list_entries);
	wrefresh(stdscr);
}
void lista::start_searching(string name_to_search_for)
{
	searching = true;
	searching_for = name_to_search_for;
	search_next();
}
void lista::search_next(bool backwards)
{
	if (searching_for == "")return; //nie było start_searching
	//wykrywanie pętli (brak wyników);
	//searching after delete seem fine to me
	int starting_choice = choice;
	int loop_count = 0;
	searching = true;
	bool keep_searching = true;
	while (keep_searching) {
		if (!backwards)down();
		else up();
		//znalazłem
		if (get_current_selection_name().find(searching_for) != std::string::npos)keep_searching = false;
		else {//nie znalazłem - kręcę się dalej;
			if (choice == starting_choice)loop_count++;
			if (loop_count >= 2) {
				//przeiterowałem dwa razy
				keep_searching = false;
				//nie znalazłem
			}
		}
	}
	draw();
	searching = false;
}
