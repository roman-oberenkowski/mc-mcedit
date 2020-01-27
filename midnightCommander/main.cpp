#include <curses.h>
#include "midnightCommander.h"
#include "./../src/MceditBackend.h"


using namespace std;

//int main3() {
//	initscr();
//	start_color();
//	int h, w;
//	getmaxyx(stdscr, h, w);
//	wprintw(stdscr, "%d %d", h, w);
//	wrefresh(stdscr);
//	WINDOW* mainWin = subwin(stdscr, 28, 120, 1, 0);
//	midnightCommander editor = midnightCommander();
//	keypad(stdscr, TRUE);
//	curs_set(0);
//	noecho();
//	//wgetch(stdscr);
//	//WINDOW* newwin = subwin(stdscr, 20, 100, 1, 0);
//	//editor.resize_handler(newwin);
//	//delwin(mainWin);
//
//
//	while (1) {
//		switch (wgetch(stdscr)) {
//		case KEY_UP:
//			editor.up();
//			break;
//		case KEY_DOWN:
//			editor.down();
//			break;
//		case 10:
//			editor.choose();
//			break;
//		case '\t':
//			editor.toggle_active_win();
//			break;
//		case 'c':
//			//editor.copy_move("copied_file");
//			break;
//		case 'm':
//			//editor.copy_move("moved_file", true);
//			break;
//		case 'e':
//			editor.create_empty_file("wrtrevolution.txt");
//			break;
//		case 'd':
//			editor.create_directory("falafel");
//			break;
//		case 'r':
//			editor.remove();
//			break;
//		case 's':
//			editor.search("wtf");
//			break;
//		case 'x':
//			editor.search_next();
//			break;
//		case 'z':
//			editor.search_previous();
//			break;
//		case 'b':
//			editor.enable_batch_mode();
//			break;
//			//create prompt in the editor
//			//std::filesystem::exists("helloworld.tx");
//			//chceck if resize handler works
//			//check batch mode
//			//check creation of files
//		}
//	}
//	return 0;
//}


int main()
{
	midnightCommander* tool = new midnightCommander();
	BaseBackend* backend = new MceditBackend(tool);
	//all
	backend->bind("<EDITION>", [&tool]() { tool->navigate(); }, "Edition mode - basic navigation");
	//mcedit
	backend->bind("#mcedit#.File.Copy${Target filename (default=source filename):|filename}", [&tool]() {tool->copy(); }, "Copies file");
	backend->bind("#mcedit#.File.Move${Target filename (default=source filename):|filename}", [&tool]() {tool->move(); }, "Moves file");
	backend->bind("#mcedit#.File.Remove", [&tool]() {tool->remove(); }, "removes file");
	backend->bind("#mcedit#.Batch.Enable", [&tool]() {tool->enable_batch_mode(); }, "Enables batch mode");
	backend->bind("#mcedit#.Batch.Abort", [&tool]() {tool->abort_batch(); }, "Enables batch mode");
	backend->bind("#mcedit#.Batch.Commit", [&tool]() {tool->commit_batch(); }, "Commits batch mode");
	backend->bind("#mcedit#.Search.SearchFor${Name of file to search for:|search}", [&tool]() {tool->search(); }, "Searches for file in current directory, empty exits search mode");
	backend->bind("#mcedit#.Create.EmptyFile${New file's name:|filename}", [&tool]() {tool->create_empty_file(); }, "");
	backend->bind("#mcedit#.Create.Directory${New directory's name:|filename}", [&tool]() {tool->create_directory(); }, "");
	//additional mcedit
	backend->bind("#mcedit#.Search.Stop", [&tool]() {tool->stop_searching(); }, "stops searching - same as search empty filename");
	backend->bind("#mcedit#.Navigate.Up", [&tool]() {tool->up(); }, "Moves UP");
	backend->bind("#mcedit#.Navigate.Down", [&tool]() {tool->down(); }, "Moved DOWN");
	backend->bind("#mcedit#.Navigate.Choose", [&tool]() {tool->choose(); }, "Chooses item");
	backend->bind("#mcedit#.Navigate.ToggleActiveWindow", [&tool]() {tool->toggle_active_win(); }, "Toogles active WINDOW");
	//nice
	backend->bind("#nice#.File.Copy${Target filename (default=source filename):|filename}", [&tool]() {tool->copy(); }, "Copies file");
	backend->bind("#nice#.File.Move${Target filename (default=source filename):|filename}", [&tool]() {tool->move(); }, "Moves file");
	backend->bind("#nice#.File.Remove", [&tool]() {tool->remove(); }, "removes file");
	backend->bind("#nice#.Batch.Enable", [&tool]() {tool->enable_batch_mode(); }, "Enables batch mode");
	backend->bind("#nice#.Batch.Abort", [&tool]() {tool->abort_batch(); }, "Enables batch mode");
	backend->bind("#nice#.Batch.Commit", [&tool]() {tool->commit_batch(); }, "Commits batch mode");
	backend->bind("#nice#.Search.SearchFor${Name of file to search for:|search}", [&tool]() {tool->search(); }, "Searches for file in current directory, empty exits search mode");
	backend->bind("#nice#.Create.EmptyFile${New file's name:|filename}", [&tool]() {tool->create_empty_file(); }, "");
	backend->bind("#nice#.Create.Directory${New directory's name:|filename}", [&tool]() {tool->create_directory(); }, "");
	//additional nice
	backend->bind("#nice#.Search.Stop", [&tool]() {tool->stop_searching(); }, "stops searching - same as search empty filename");
	backend->bind("#nice#.Navigate.Up", [&tool]() {tool->up(); }, "Moves UP");
	backend->bind("#nice#.Navigate.Down", [&tool]() {tool->down(); }, "Moved DOWN");
	backend->bind("#nice#.Navigate.Choose", [&tool]() {tool->choose(); }, "Chooses item");
	backend->bind("#nice#.Navigate.ToggleActiveWindow", [&tool]() {tool->toggle_active_win(); }, "Toogles active WINDOW");
	//nano
	backend->bind("#nano#<CTRL>K%Copy!Target filename (default=source filename):${filename}", [&tool]() {tool->copy(); }, "Copies file");
	backend->bind("#nano#<CTRL>M%Move!Target filename (default=source filename):${filename}", [&tool]() {tool->move(); }, "Moves file");
	backend->bind("#nano#<CTRL>R%Remove", [&tool]() {tool->remove(); }, "Removes file");
	backend->bind("#nano#<CTRL>B%EnableBatch", [&tool]() {tool->enable_batch_mode(); }, "Enables batch mode");
	backend->bind("#nano#<CTRL>G%AbortBatch", [&tool]() {tool->abort_batch(); }, "aborts batch");
	backend->bind("#nano#<CTRL>H%CommitBatch", [&tool]() {tool->commit_batch(); }, "commits batch");
	backend->bind("#nano#<CTRL>F%Search!Name of file to search for:${search}", [&tool]() {tool->search(); }, "Searches for file in current directory, empty exits search mode");
	backend->bind("#nano#<CTRL>N%EmptyFIle!New file's name:${filename}", [&tool]() {tool->create_empty_file(); }, "Creates empty file");
	backend->bind("#nano#<CTRL>M%NewDir!New directory's name:${filename}", [&tool]() {tool->create_directory(); }, "Creates Directory");
	//vim
	backend->bind("#vim#:copy ${filename}<ENTER>!EDIT", [&tool]() {tool->copy(); }, "Copies file");
	backend->bind("#vim#:move ${filename}<ENTER>!EDIT", [&tool]() {tool->move(); }, "Moves file");
	backend->bind("#vim#:remove<ENTER>!EDIT", [&tool]() {tool->remove(); }, "Removes file");
	backend->bind("#vim#:enablebatch<ENTER>!EDIT", [&tool]() {tool->enable_batch_mode(); }, "Enables batch mode");
	backend->bind("#vim#:abortbatch<ENTER>!EDIT", [&tool]() {tool->abort_batch(); }, "aborts batch");
	backend->bind("#vim#:commitbatch<ENTER>!EDIT", [&tool]() {tool->commit_batch(); }, "commits batch");
	backend->bind("#vim#:search ${search}<ENTER>!EDIT", [&tool]() {tool->search(); }, "Searches for file in current directory, empty exits search mode");
	backend->bind("#vim#:touch ${filename}<ENTER>!EDIT", [&tool]() {tool->create_empty_file(); }, "Creates empty file");
	backend->bind("#vim#:mkdir ${filename}<ENTER>!EDIT", [&tool]() {tool->create_directory(); }, "Creates Directory");
	


	backend->start();
	delete backend;
	delete tool;
	return 0;
}