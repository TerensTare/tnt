#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <tuple>
#include <vector>

#include "Config.hpp"

// TODO: add iterators for Matrix.
// TODO: add specializations for Matrix<T, 1,x> | Matrix<T, x, 1> | Matrix<T, 1, 1> so that the class wouldn't use two arrays.
// TODO: check if the length or the width of the Matrix is bigger than 0.
// TODO: add operator/(Matrix& rhs) and operator=;
// TODO(partial): custom printing matrix.
// TODO: add conversion operator for Vector, SDL_Point, SDL_FPoint, std::pair, etc.
namespace tnt
{
// implementation based on
// https://medium.com/@furkanicus/how-to-create-a-matrix-class-using-c-3641f37809c7
class Matrix
{
public:
	Matrix(unsigned row, unsigned column, double init = 0.0);
	Matrix(Matrix const &) = default;
	~Matrix();

	Matrix operator+(Matrix &rhs);
	Matrix operator-(Matrix &rhs);
	Matrix operator*(Matrix &rhs);

	Matrix operator+(double num);
	Matrix operator-(double num);
	Matrix operator*(double num);
	Matrix operator/(double num);

	double &operator()(unsigned const &row, unsigned const &column);
	double at(unsigned const &row, unsigned const &column);

	Matrix transpose(); // rows are columns and columns are rows.
	std::tuple<Matrix, double, int> power_iterator(unsigned rowNum, double tolerance);
	Matrix deflation(Matrix &x, double &eigenVal);

	//void print() const;

	unsigned Rows() const;
	unsigned Columns() const;

private:
	unsigned rows;
	unsigned columns;
	std::vector<std::vector<double>> data;
};

namespace experimental
{
template <typename T = void, std::size_t W = 1, std::size_t H = 1>
class Matrix
{
public:
	Matrix(Matrix const &) = default; // for now...
	Matrix &operator=(Matrix const &) = default;

	Matrix(Matrix &&) = default; // for now...
	Matrix &operator=(Matrix &&) = default;

private:
	std::array<std::array<T, H>, W> data;
};

template <typename T, std::size_t W>
class Matrix<T, W, 1>
{
private:
	std::array<T, W> data;
};

template <typename T>
class Matrix<T, 1, 1>
{
private:
	T data;
};

template <std::size_t W, std::size_t H>
class Matrix<void, W, H>
{
	TNT_NON_COPYABLE(Matrix)
	TNT_NON_MOVABLE(Matrix)

	Matrix()
	{
		debug std::cout << "Creating a Matrix<void>";
	}
};
} // namespace experimental
} // namespace tnt

#endif //!MATRIX_HPP