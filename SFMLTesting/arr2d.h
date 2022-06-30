#pragma once
#ifndef __ARR_2D__
#define __ARR_2D__
#include <vector>
#include <cassert>

template <typename T>
class arr2d {
private:
	std::vector<std::vector<T>>vec;
public:
	arr2d(const int rows, const int columns) : rows(rows), columns(columns)
	{
		this->vec = std::vector<std::vector<T>>(rows, std::vector<T>(columns, 0.0f));
	}

	std::vector<T>& operator[](int index) {
		assert(index > -1 && index < rows);
		return this->vec[index];
	}

	T& get_value(int row, int column)
	{
		return this->vec[row][column];
	}

	void set_value(int row, int column, T value)
	{
		this->vec[row][column] = value;
	}

public:
	int rows;
	int columns;
};


#endif