#include "map.h"
#include <ctime>

const std::vector<unsigned int> Map::complexity_coefs({ 15, 25, 37 });

std::vector<std::pair<int, int>> combinations({ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} });

std::vector<std::vector<unsigned int>> Map::Get_map() const
{
	return mMap;
}

Map::Map() : mRow_size(0), mColumn_size(0), mMap({}), mComplexity(0), num_of_mines(0)
{}
Map::Map(size_t row_size, size_t column_size, size_t complexity) : mRow_size(row_size), mColumn_size(column_size),
mMap(std::vector<std::vector<unsigned int>>(row_size, std::vector<unsigned int>(column_size, 0))), mComplexity(complexity)
{
	num_of_mines = (mRow_size * mColumn_size * complexity_coefs[complexity]) / 100;
}

bool Map::out_of_map(size_t cell_x, size_t cell_y) const
{
	if (cell_x < 0 || cell_x >= mColumn_size || cell_y < 0 || cell_y >= mRow_size)
		return true;

	return false;
}

bool Map::cell_without_mine(const int cell_x, const int cell_y, const int row_without_mines, const int column_without_mines, const int row_pos, const int column_pos) const
{
	for (int i = 0; i <= row_without_mines; ++i)
	{
		for (int j = 0; j <= column_without_mines; ++j)
		{
			if (column_pos == cell_x - i && row_pos == cell_y - j)
				return true;

			if (column_pos == cell_x + i && row_pos == cell_y + j)
				return true;
		}
	}
	return false;
}

void Map::set_mines_numbers(const int cell_x, const int cell_y)
{
	const int row_without_mines = rand() % 2 + 1;
	const int column_without_mines = rand() % 2 + 1;

	for (size_t i = 0; i < num_of_mines; ++i)
	{
		int row_pos{}, column_pos{};
		do {
			row_pos = rand() % mRow_size;
			column_pos = rand() % mColumn_size;
		} while (mMap[row_pos][column_pos] == MINE || cell_without_mine(cell_x, cell_y, row_without_mines, column_without_mines, row_pos, column_pos));


		mMap[row_pos][column_pos] = MINE;

		for (std::pair<int, int> pair : combinations)
		{
			int row_near_pos = row_pos + pair.first, column_near_pos = column_pos + pair.second;
			if (out_of_map(row_near_pos, column_near_pos) || mMap[row_near_pos][column_near_pos] == MINE)
				continue;

			++mMap[row_near_pos][column_near_pos];
		}
	}
}