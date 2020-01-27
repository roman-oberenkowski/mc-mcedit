#include "midnightCommander.h"

using namespace std;


std::string midnightCommander::getEntry(const std::string& key) const
{
	try {
		return entries.at(key);
	}
	catch (std::out_of_range&) {
		throw BaseTool::UndefinedEntry();
	}
}

void midnightCommander::setEntry(const std::string& key, const std::string& value)
{
	entries[key] = value;
}

okienko* midnightCommander::get_active()
{
	if (is_left_active()) {
		return left;
	}
	else {
		return right;
	}
}

void midnightCommander::navigate() {
	string key=getEntry("KEY");
	if (key == "<ENTER>")choose();
	if (key == "<TAB>")toggle_active_win();
	if (key == "<DARROW>")down();
	if (key == "<UARROW>")up();
}

okienko* midnightCommander::get_inactive()
{
	if (!is_left_active()) {
		return left;
	}
	else {
		return right;
	}
}
void midnightCommander::cosmetic_refresh(bool alternate=false) {
	if (left->get_current_directory() == right->get_current_directory()) {
		if (!alternate) {
			get_inactive()->list_refresh();
		}
		else {
			get_active()->list_refresh();
		}
	}
}

bool midnightCommander::is_right_active()
{
	return !active_left;
}

bool midnightCommander::is_left_active()
{
	return active_left;
}

bool midnightCommander::copy_move_execute(string in, string out, bool move)
{
	try {
		if (move) {
			std::filesystem::rename(in.c_str(), out.c_str());
			if (!batch_mode) {
				left->list_refresh();
				right->list_refresh();
			}
		}
		else {
			//CopyFile(in.c_str(), out.c_str(), TRUE);
			std::filesystem::copy(in.c_str(), out.c_str(), std::filesystem::copy_options::recursive);
			if (!batch_mode) {
				get_inactive()->list_refresh();
				//special cosmetic refresh
				cosmetic_refresh(true);
			}
		}
		return 0;
	}
	catch (std::filesystem::filesystem_error) {
		//mvwprintw(stdscr, 29, 0, "%s", "NIE UDALO SIE SKOPOIWAC IDT");
		//wrefresh(stdscr);
		return 1;
	}
}

void midnightCommander::remove_execute(string pathname)
{
	//using remove instead of remove execute to make it less destructive if accident happens
	try {
		std::filesystem::remove(pathname);
	}
	catch (std::filesystem::filesystem_error) {
		//nie udalo sie usunac (niepustego katalog lub brak uprawnien)
	}
	if (!batch_mode)get_active()->list_refresh();
}

midnightCommander::midnightCommander()
{
	debug = false;
	constructed = false;
	active_left = true;
	batch_mode = false;
	left = NULL;
	right = NULL;
	tool = NULL;
	operation_buffer.clear();
	init_pair(COLOR_MC, COLOR_WHITE, COLOR_BLUE);
	setEntry("searching_mode", "NO");
}

midnightCommander::~midnightCommander()
{
	delete left;
	delete right;
}

void midnightCommander::assignWindow(WINDOW* newwin)
{
	tool = newwin;
	int parent_height, parent_width;
	getmaxyx(tool, parent_height, parent_width);
	WINDOW* left_window;
	WINDOW* right_window;
	left_window = derwin(tool, parent_height - 1, parent_width / 2, 0, 0);
	right_window = derwin(tool, parent_height - 1, parent_width / 2, 0, parent_width / 2);
	if (!constructed) {
		left = new okienko(left_window, true);
		right = new okienko(right_window, false);
		constructed = true;
	}
	else {
		left->resize_handler(left_window, is_left_active());
		right->resize_handler(right_window, is_right_active());
	}
	
}

void midnightCommander::up()
{
	if (getEntry("searching_mode") == "YES") {
		search_previous();
	}
	else {
		get_active()->up();
	}
}

void midnightCommander::down()
{
	if (getEntry("searching_mode") == "YES") {
		search_next();
	}
	else {
		get_active()->down();
	}
}



void midnightCommander::choose()
{
	get_active()->choose();
}

void midnightCommander::copy() {
	copy_move(false);
}
void midnightCommander::move() {
	copy_move(true);
}

void midnightCommander::copy_move(bool move)
{
	string filename = getEntry("filename");
	string source_dir = get_active()->get_current_directory();
	string target_dir = get_inactive()->get_current_directory();
	string source_sel = get_active()->get_current_selection_name();
	if (source_sel == "..") return;
	if (filename == "")filename = source_sel;
	if (source_dir == target_dir) {}//TODO
	string in = source_dir + "\\" + source_sel;
	string out = target_dir + "\\" + filename;
	if (debug) {
		mvwprintw(stdscr, 28, 0, "%s", in.c_str());
		mvwprintw(stdscr, 29, 0, "%s", out.c_str());
		wrefresh(stdscr);
		getch();
	}
	if (in == out)return; //need to check if moving file (renaming) in same dirs is possible
	if (!batch_mode) {
		copy_move_execute(in, out, move);
		cosmetic_refresh();
	}
	else {
		//enque operation
		enque_operation(in, out, (move) ? int(move_id) : int(copy_id));
	}
}

void midnightCommander::remove()
{
	string source_dir = get_active()->get_current_directory();
	string source_sel = get_active()->get_current_selection_name();
	if (source_sel == "..") {
		//nie można usuwać '..' bo to może powodować problemyyyyy...
		return;
	}
	string pathname = source_dir + "\\" + source_sel;
	if (!batch_mode) {
		remove_execute(pathname);
		cosmetic_refresh();
	}
	else {
		//enque operation
		enque_operation(pathname, "", int(remove_id));
	}
}

void midnightCommander::enque_operation(string in, string out, int op_id)
{
	operation temp;
	temp.in = in;
	temp.out = out;
	temp.operation_id = op_id;
	operation_buffer.push_back(temp);
}

void midnightCommander::abort_batch()
{
	operation_buffer.clear();
	batch_mode = false;
	setEntry("IS_SAVED", "YES");
}

void midnightCommander::commit_batch()
{
	operation temp;
	//print wykonywanie operation_buffer.size() operacji
	while (operation_buffer.size() > 0) {
		temp = operation_buffer.front();
		operation_buffer.pop_front();
		switch (temp.operation_id) {
		case copy_id:
			copy_move_execute(temp.in, temp.out, false);
			break;
		case move_id:
			copy_move_execute(temp.in, temp.out, true);
			break;
		case create_file_id:
			create_empty_file_execute(temp.in);
			break;
		case create_directory_id:
			//maybe...
			break;
		case remove_id:
			remove_execute(temp.in);
			break;
		}
	}
	setEntry("IS_SAVED", "YES");
	batch_mode = false;
	left->list_refresh();
	right->list_refresh();
}

void midnightCommander::create_empty_file()
{
	string name = getEntry("filename");
	if (name == "")name = "emptyfile.txt";
	string pathname = get_active()->get_current_directory() + "\\" + name;
	if (batch_mode) {
		enque_operation(pathname, "", int(create_file_id));
	}
	else {
		create_empty_file_execute(pathname);
		cosmetic_refresh();
	}
}

void midnightCommander::create_empty_file_execute(string pathname)
{
	ofstream filee;
	filee.open(pathname, ios::out);
	filee.close();
	get_active()->list_refresh();
}

void midnightCommander::create_directory()
{
	string name = getEntry("filename");
	if (name == "")name = "new_directory";
	string pathname = get_active()->get_current_directory() + "\\" + name;
	if (batch_mode) {
		enque_operation(pathname, "", int(create_directory_id));
	}
	else {
		create_directory_execute(pathname);
		cosmetic_refresh();
	}
}

void midnightCommander::create_directory_execute(string pathname)
{
	try {
		std::filesystem::create_directory(pathname);
	}
	catch (std::filesystem::filesystem_error) {
		//nie udalo sie utowrzyc (brak uprawnien lub cos innego)
	}
	if (!batch_mode)get_active()->list_refresh();
}

void midnightCommander::toggle_active_win()
{
	left->set_active_state(!active_left);
	right->set_active_state(active_left);
	active_left = !active_left;
}

void midnightCommander::enable_batch_mode()
{
	setEntry("IS_SAVED", "NO");
	batch_mode = true;
}

void midnightCommander::search()
{
	string name = getEntry("search");
	if (name != "") {
		setEntry("searching_mode", "YES");
		get_active()->start_searching(name);
	}
	else {
		stop_searching();
	}
}

void midnightCommander::search_next()
{
	get_active()->search_next();
}

void midnightCommander::search_previous()
{
	get_active()->search_next(true);
}

void midnightCommander::stop_searching() {
	setEntry("searching_mode", "NO");
}