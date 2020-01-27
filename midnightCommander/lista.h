// lista.h

#include <curses.h> //grafika
#include <string>   //nazwy i sciezki
#include <vector>   //wektor plików
#include <io.h>     //finddata_t
#include <windows.h>



using namespace std;

class lista
{
private:
	bool is_directory(int id);
	WINDOW* list_window;
	string get_editor();
	int list_entries;
	int list_height;
	int list_width;
	int choice;
	int downshift;
	bool searching;
	string searching_for;
	string current_directory;
	vector <_finddata_t> folder_content_vect;
	void draw();
	string windows_get_current_directory();
	int get_current_selection();
public:
	lista();
	void list_change_window(WINDOW* win);
	void init_window_dimensions();
	void list_refresh();
	string get_current_selection_name();
	string get_current_directory();
	bool is_selected_directory();
	void up();
	void down();
	void folder_up();
	void choose();
	void debug();
	void start_searching(string name_to_search_for);
	void search_next(bool backwards = false);
};

