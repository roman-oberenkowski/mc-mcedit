// midnightCommander.h
#include <curses.h>
#include "okienko.h"
//#include "lista.h"
#include <deque>
#include <filesystem>
#include <fstream>
#include "../src/BaseTool.h"
#include <unordered_map>

using namespace std;

class midnightCommander final : public BaseTool
{
private:
	//from base tool:
	std::unordered_map<std::string, std::string> entries;
	std::string getEntry(const std::string&) const override;
	void setEntry(const std::string&, const std::string&) override;
	void assignWindow(WINDOW*) override;
	//mine:
	bool constructed;
	bool debug;
	WINDOW* tool;
	okienko* left;
	okienko* right;
	okienko* get_active();
	okienko* get_inactive();
	bool batch_mode;
	bool active_left;
	bool is_right_active();
	bool is_left_active();
	void cosmetic_refresh(bool);
	enum operations
	{
		blank_id,
		copy_id,
		move_id,
		create_file_id,
		create_directory_id,
		remove_id
	};
	struct operation
	{
		string in;
		string out;
		int operation_id;
	};
	deque <operation> operation_buffer;
	bool copy_move_execute(string in, string out, bool move = false);
	void remove_execute(string pathname);
	void copy_move(bool move = false);
	void enque_operation(string in, string out, int op_id);
	void create_empty_file_execute(string pathname);
	void create_directory_execute(string pathname);
	void search_next();
	void search_previous();
public:
	midnightCommander();
	~midnightCommander();
	void navigate();

	void toggle_active_win();
	void up();
	void down();
	void choose();
	void stop_searching();


	void copy();
	void move();
	void remove();
	
	void abort_batch();
	void commit_batch();
	void enable_batch_mode();

	void search();
	void create_empty_file();
	void create_directory();
	
	
	
};