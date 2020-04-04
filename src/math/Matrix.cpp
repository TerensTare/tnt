// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <future>
#include <iostream>
#include "math/Matrix.hpp"

tnt::Matrix::Matrix(unsigned row, unsigned column, double init)
	: rows{row}, columns{column}
{
	data.resize(row);
	std::async(std::launch::deferred, [this, column, init] {
		for (unsigned i{0}; i < data.size(); ++i)
			data[i].resize(column, init);
	}).get();
}

tnt::Matrix::~Matrix()
{
	std::async(std::launch::deferred, [this] {
		data.clear();
		std::vector<std::vector<double>>{}.swap(data);
	}).get();
}

tnt::Matrix tnt::Matrix::operator+(Matrix &rhs)
{
	Matrix sum{columns, rows};
	unsigned i{}, j{};
	for (i = 0; i < rows; ++i)
		for (j = 0; j < columns; ++j)
			sum(i, j) = data[i][j] + rhs(i, j);
	return sum;
}

tnt::Matrix tnt::Matrix::operator-(Matrix &rhs)
{
	Matrix diff{columns, rows};
	unsigned i{}, j{};
	for (i = 0; i < rows; ++i)
		for (j = 0; j < columns; ++j)
			diff(i, j) = data[i][j] - rhs(i, j);
	return diff;
}

tnt::Matrix tnt::Matrix::operator*(Matrix &rhs)
{
	Matrix mult{rows, rhs.Columns()};

	if (columns == rhs.Rows())
	{
		unsigned i{}, j{}, k{};
		double tmp{0.0};
		for (i = 0; i < rows; ++i)
			for (j = 0; j < rhs.Columns(); ++j)
			{
				tmp = 0.0;
				for (k = 0; k < columns; ++k)
					tmp = tmp + data[i][k] * rhs(k, j);
				mult(i, j) = tmp;
			}
		return mult;
	}
	else
		return {0, 0};
}

tnt::Matrix tnt::Matrix::operator+(double num)
{
	Matrix res{rows, columns};
	unsigned i{}, j{};
	for (i = 0; i < rows; ++i)
		for (j = 0; j < columns; ++j)
			res(i, j) = data[i][j] + num;
	return res;
}

tnt::Matrix tnt::Matrix::operator-(double num)
{
	Matrix res{rows, columns};
	unsigned i{}, j{};
	for (i = 0; i < rows; ++i)
		for (j = 0; j < columns; ++j)
			res(i, j) = data[i][j] - num;
	return res;
}

tnt::Matrix tnt::Matrix::operator*(double num)
{
	Matrix res{rows, columns};
	unsigned i{}, j{};
	for (i = 0; i < rows; ++i)
		for (j = 0; j < columns; ++j)
			res(i, j) = data[i][j] * num;
	return res;
}

tnt::Matrix tnt::Matrix::operator/(double num)
{
	Matrix res{rows, columns};
	unsigned i{}, j{};
	for (i = 0; i < rows; ++i)
		for (j = 0; j < columns; ++j)
			res(i, j) = data[i][j] / num;
	return res;
}

double &tnt::Matrix::operator()(unsigned const &row, unsigned const &column)
{
	return data[row][column];
}

double tnt::Matrix::at(unsigned const &row, unsigned const &column)
{
	return data[row][column];
}

tnt::Matrix tnt::Matrix::transpose()
{
	Matrix res{columns, rows};
	for (unsigned i{0}, j{0}; i < columns, j < rows; ++i, ++j)
		res(i, j) = data[j][i];
	return res;
}

std::tuple<tnt::Matrix, double, int> tnt::Matrix::power_iterator(unsigned rowNum, double tolerance)
{
	Matrix x{rows, 1, 1.0};
	for (unsigned i{1}; i <= rowNum; ++i)
		x(i - 1, 0) = i;

	int errCode{0};
	double diff{1.0};
	unsigned j{0}, location{};
	std::vector<double> eigen{};
	double eigenVal{0.0};

	eigen.push_back(0.0);

	while (std::abs(diff) > tolerance)
	{
		++j;
		for (unsigned i{0}; i < rowNum; ++i)
		{
			eigenVal = x(0, 0);
			if (std::abs(x(i, 0)) >= std::abs(eigenVal))
			{
				eigenVal = x(i, 0);
				location = i;
			}
		}

		if (j >= 5e5)
		{
			std::cout << "Error: complex number encountered in matrix!\nLine: " << __LINE__ << std::endl;
			errCode = -1;
			return std::make_tuple(x, 0.0, errCode);
		}

		eigen.push_back(eigenVal);
		diff = eigen[j] - eigen[j - 1];

		x = (*this) * x;
	}

	x = x / eigenVal;
	return std::make_tuple(x, eigenVal, errCode);
}

// TODO: optimize this.
tnt::Matrix tnt::Matrix::deflation(Matrix &x, double &eigenVal)
{
	double denominator{eigenVal / (x.transpose() * x)(0, 0)};
	Matrix xtrans{x.transpose()};
	Matrix rhs{x * xtrans};
	Matrix rhs2{rhs * denominator};
	Matrix a2{*this - rhs2};
	return a2;
}

unsigned tnt::Matrix::Rows() const
{
	return rows;
}

unsigned tnt::Matrix::Columns() const
{
	return columns;
}
