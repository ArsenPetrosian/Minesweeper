#pragma once
#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <stack>

class Map
{
private:
	size_t mRow_size;
	size_t mColumn_size;
	mutable std::vector<std::vector<unsigned int>> mMap;        // map with mines: 0 - empty, 11 - mine, [1, 8] - number of near mines

	static const std::vector<unsigned int> complexity_coefs;    // percent of mines on the map: 15, 25 or 37
	size_t mComplexity;                                         // index of complexity_coefs

	unsigned int num_of_mines;

	const unsigned int EMPTY_CELL = 0;
	const unsigned int MINE = 11;

	bool cell_without_mine(const int cell_x, const int cell_y, const int row_without_mines, const int column_without_mines, const int row_pos, const int column_pos) const; // return true if the cell is near the cell of the first click
public:
	Map();
	Map(size_t row_size, size_t column_size, size_t complexity);

	std::vector<std::vector<unsigned int>> Get_map() const;

	bool out_of_map(size_t cell_x, size_t cell_y) const; // if the cell is out of the map it returns true

	void set_mines_numbers(const int cell_x, const int cell_y);           // set mines and numbers on the map with island on the clicked cells
};

#endif