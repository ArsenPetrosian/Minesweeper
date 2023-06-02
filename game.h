#pragma once
#ifndef GAME_H
#define GAME_H

#include "map.h"

class Game
{
private:
	size_t mRow_size;
	size_t mColumn_size;
	std::vector<std::vector<unsigned int>> mMask;      // map for gamer:  0 - closed, 1 - opened, 12 - flag

	bool click_function_flag;                          // open: true, flag: false, default: open

	unsigned int num_of_opened_cells;                  // number of opened cells

	static const std::vector<unsigned int> complexity_coefs;    // percent of mines on the map: 15, 25 or 37
	size_t mComplexity;

	unsigned int num_of_setted_flags;                  // number of setted flags
	unsigned int max_num_of_flags;                     // number of mines

	bool lose_flag;

	const unsigned int EMPTY_CELL = 0;
	const unsigned int MINE = 11;
	const unsigned int FLAG = 12;

	char return_symbol(size_t cell_x, size_t cell_y, const std::vector<std::vector<unsigned int>>& map) const;

	bool out_of_map(size_t cell_x, size_t cell_y) const; // if the cell is out of the map it returns true
public:
	Game(size_t row_size, size_t column_size, size_t complexity);

	void change_lose_flag();                             // game is over: click on the mine or enough amount of flags with mine without flag

	bool win_check() const;                              // check if it is a win or not at this moment

	void change_func();                                  // change function of a click

	bool get_func() const;                               // get function of a click

	unsigned int open_cell(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map);           // FLAG: it's a flag, number: which number it is

	void fill_empty_cells(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map);                  // fill near empty cells

	void set_unset_flag(const int cell_x, const int cell_y);              // set or unset flag on cell

	bool is_opened(const int cell_x, const int cell_y) const;

	bool is_flag(const int cell_x, const int cell_y) const;

	bool is_number(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map) const;

	unsigned int near_flags(const int cell_x, const int cell_y) const;    // number of near flags

	int check_setted_flags(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map) const;     // 0: not enough flags, 1: mine without flag, 2: more or equal flags

	void open_flag_cells(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map);             // flags near a number are on the right positions, open near not flagged cells

	//void run();                                          // run game with setted size of the map and difficulty
};

#endif