#include "game.h"

const std::vector<unsigned int> Game::complexity_coefs({ 15, 25, 37 });

std::vector<std::pair<int, int>> combinations_of_near_cells({ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} });

Game::Game(size_t row_size, size_t column_size, size_t complexity) : mRow_size(row_size), mColumn_size(column_size),
mMask(std::vector<std::vector<unsigned int>>(row_size, std::vector<unsigned int>(column_size, 0))),
click_function_flag(true), num_of_opened_cells(0), num_of_setted_flags(0), mComplexity(complexity), lose_flag(false)
{
	max_num_of_flags = (mRow_size * mColumn_size * complexity_coefs[complexity]) / 100;
}

char Game::return_symbol(size_t cell_x, size_t cell_y, const std::vector<std::vector<unsigned int>>& map) const
{
	if (mMask[cell_x][cell_y] == 0)
		return '.';

	if (mMask[cell_x][cell_y] == FLAG)
		return 'F';

	if (map[cell_x][cell_y] == EMPTY_CELL)
		return ' ';

	if (map[cell_x][cell_y] == MINE)
		return '*';

	return '0' + map[cell_x][cell_y];
}

bool Game::out_of_map(size_t cell_x, size_t cell_y) const
{
	if (cell_x < 0 || cell_x >= mColumn_size || cell_y < 0 || cell_y >= mRow_size)
		return true;

	return false;
}

void Game::change_lose_flag()
{
	lose_flag = true;
}

bool Game::win_check() const
{
	if (num_of_opened_cells == mColumn_size * mRow_size)
		return true;

	return false;
}

void Game::change_func()
{
	if (click_function_flag)
	{
		click_function_flag = false;
		return;
	}

	click_function_flag = true;
}

bool Game::get_func() const
{
	return click_function_flag;
}

unsigned int Game::open_cell(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map)
{
	if (mMask[cell_y][cell_x] == FLAG)
		return FLAG;

	mMask[cell_y][cell_x] = 1;
	++num_of_opened_cells;
	return map[cell_y][cell_x];
}

void Game::fill_empty_cells(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map)
{
	std::stack<int> stk;

	stk.push(cell_x);
	stk.push(cell_y);

	int x{}, y{};
	while (true)
	{
		y = stk.top();
		stk.pop();
		x = stk.top();
		stk.pop();

		for (std::pair<int, int> pair : combinations_of_near_cells) {
			int real_x = x + pair.first, real_y = y + pair.second;

			if (out_of_map(real_x, real_y))
				continue;

			if (map[real_y][real_x] == EMPTY_CELL && mMask[real_y][real_x] == 0)
			{
				stk.push(real_x);
				stk.push(real_y);
			}
			if (mMask[real_y][real_x] == 0)
			{
				++num_of_opened_cells;
				mMask[real_y][real_x] = 1;
			}
		}
		if (stk.empty())
			break;
	}
	//show(map);
}

void Game::set_unset_flag(const int cell_x, const int cell_y)
{
	if (mMask[cell_y][cell_x] == FLAG)
	{
		--num_of_setted_flags;
		--num_of_opened_cells;
		mMask[cell_y][cell_x] = 0;
		return;
	}

	if (mMask[cell_y][cell_x] == 0 && num_of_setted_flags < max_num_of_flags)
	{
		++num_of_setted_flags;
		++num_of_opened_cells;
		mMask[cell_y][cell_x] = FLAG;
	}
}

bool Game::is_opened(const int cell_x, const int cell_y) const
{
	if (mMask[cell_y][cell_x] != 0)
		return true;

	return false;
}

bool Game::is_flag(const int cell_x, const int cell_y) const
{
	if (mMask[cell_y][cell_x] == FLAG)
		return true;

	return false;
}

bool Game::is_number(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map) const
{
	unsigned int cell = map[cell_y][cell_x];
	if (cell >= 1 && cell <= 8)
		return true;

	return false;
}

unsigned int Game::near_flags(const int cell_x, const int cell_y) const
{
	unsigned int res{};

	for (std::pair<int, int> pair : combinations_of_near_cells) {
		int x = cell_x + pair.first;
		int y = cell_y + pair.second;

		if (out_of_map(x, y))
			continue;

		if (mMask[y][x] == FLAG)
			++res;
	}
	return res;
}

int Game::check_setted_flags(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map) const
{
	if (near_flags(cell_x, cell_y) < map[cell_y][cell_x])
		return 0;

	for (std::pair<int, int> pair : combinations_of_near_cells) {
		int x = cell_x + pair.first;
		int y = cell_y + pair.second;

		if (out_of_map(x, y))
			continue;

		if (map[y][x] == MINE && mMask[y][x] != FLAG)
			return 1;
	}

	return 2;
}

void Game::open_flag_cells(const int cell_x, const int cell_y, const std::vector<std::vector<unsigned int>>& map)
{
	for (std::pair<int, int> pair : combinations_of_near_cells) {
		int x = cell_x + pair.first;
		int y = cell_y + pair.second;

		if (out_of_map(x, y))
			continue;

		if (mMask[y][x] == 0)
		{
			open_cell(cell_x, cell_y, map);
			if (map[y][x] == EMPTY_CELL)
				fill_empty_cells(cell_x, cell_y, map);
		}
	}
}

/*void Game::run()
{
	Map map(mRow_size, mColumn_size, mComplexity);

	std::vector<std::vector<unsigned int>> game_map;

	show(game_map);

	Keyboard key;
	Cursor cur;

	cur.move();

	bool set_mines = true;
	bool cond = false;
	while (!cond)
	{
		key.wait_key();

		cur.save();

		switch (key.Get_Key()) {
		case 77: cur.inc_X(); break;
		case 75: cur.dec_X(); break;
		case 80: cur.inc_Y(); break;
		case 72: cur.dec_Y(); break;
		case 32: // backspace
		{
			change_func();
			show(game_map);
			break;
		}
		case 13: // enter
			if (get_func()) // open
			{
				if (set_mines)
				{
					map.set_mines_numbers(cur);
					game_map = map.Get_map();
					set_mines = false;
				}

				if (is_opened(cur) && is_number(cur, game_map))
				{
					int res = check_setted_flags(cur, game_map);
					if (res == 0)
						break;

					if (res == 1)
					{
						change_lose_flag();
						cond = true;
						break;
					}

					open_flag_cells(cur, game_map);
				}

				int result = open_cell(cur, game_map);
				if (result == MINE)
				{
					change_lose_flag();
					cond = true;
					break;
				}
				if (result == FLAG)
					break;

				if (result == EMPTY_CELL)
					fill_empty_cells(cur, game_map);
			}
			else // flag
			{
				if (is_opened(cur) && !is_flag(cur))
				{
					if (is_number(cur, game_map))
					{
						int res = check_setted_flags(cur, game_map);
						if (res == 0)
							break;

						if (res == 1)
						{
							change_lose_flag();
							cond = true;
							break;
						}

						open_flag_cells(cur, game_map);
					}
				}
				else
					set_unset_flag(cur);
			}
			show(game_map);
			break;
		}

		if (out_of_map(cur.Get_X() - 1, cur.Get_Y() - 2))
			cur.reset();

		cur.move();

		if (win_check())
			cond = true;
	}
	show(game_map);
}*/