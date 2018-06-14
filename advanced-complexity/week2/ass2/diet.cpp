#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <cassert>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;

const double EPS = 1e-6;

#define IsZero(a) (fabs((a)) <= (EPS))
#define NotZero(a) (fabs((a)) > (EPS))
#define Equal(a,b) (fabs(a - b) <= (EPS + EPS * fabs(b)))

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

using std::string;
using std::stringstream;

struct Equation
{
	Equation(const Matrix &a, const Column &b) :
		a(a),
		b(b)
	{}

	Matrix a;
	Column b;
};

struct Position
{
	Position(int column, int row) :
		column(column),
		row(row)
	{}

	int column;
	int row;
};

Matrix ReadMatrix(std::istream &in, size_t rows, size_t cols)
{
	Matrix mat(rows, Column(cols));
	for (size_t r = 0; r < rows; r++)
	{
		for (size_t c = 0; c < cols; ++c)
			in >> mat[r][c];
	}
	return mat;
}

Column ReadColumn(std::istream &in, size_t length)
{
	Column col(length);
	for (size_t i = 0; i < length; ++i)
		in >> col[i];
	return col;
}

Position SelectPivotElement(
	const Matrix &a,
	std::vector <bool> &used_rows,
	std::vector <bool> &used_columns)
{
	int size = a.size();

	// This algorithm selects the first free element.
	// You'll need to improve it to pass the problem.
	Position pivot(0, 0);
	while (used_rows[pivot.row])
	{
		++pivot.row;
		if (pivot.row == size)
			break;
	}
	while (used_columns[pivot.column] || IsZero(a[pivot.row][pivot.column]))
	{
		++pivot.column;
		if (pivot.column == size)
			break;
	}
	return pivot;
}

void SwapLines(Matrix &a, Column &b, std::vector <bool> &used_rows, Position &pivot)
{
	std::swap(a[0], a[pivot.row]);
	std::swap(b[0], b[pivot.row]);
	std::swap(used_rows[0], used_rows[pivot.row]);
	pivot.row = 0;
}

void RescalePivotRow(Matrix &a, Column &b, const Position &pivot_element)
{
	double constant = a[pivot_element.row][pivot_element.column];
	assert(NotZero(constant));

	int size = a.size();
	for (int i = 0; i < size; ++i)
		a[pivot_element.row][i] /= constant;
	b[pivot_element.row] /= constant;
}

void SubtractLines(Matrix &a, Column &b, const Position &pivot_element)
{
	assert(pivot_element.row == 0);
	Row pivot_row = a[0];

	int size = a.size();
	for (int r = 1; r < size; ++r)
	{
		double factor = -(a[r][pivot_element.column]);

		for (int c = 0; c < size; ++c)
			a[r][c] += pivot_row[c] * factor;
		b[r] += b[0] * factor;
	}
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element)
{
	RescalePivotRow(a, b, pivot_element);
	SubtractLines(a, b, pivot_element);
}

void MarkPivotElementUsed(const Position &pivot_element, std::vector <bool> &used_rows, std::vector <bool> &used_columns)
{
	used_rows[pivot_element.row] = true;
	used_columns[pivot_element.column] = true;
}

Column SolveEquation(Equation equation)
{
	Matrix &a = equation.a;
	Column &b = equation.b;
	int size = a.size();

	std::vector <bool> used_columns(size, false);
	std::vector <bool> used_rows(size, false);

	for (int step = 0; step < size; ++step)
	{
		Position pivot = SelectPivotElement(a, used_rows, used_columns);
		if (pivot.row == size || pivot.column == size)
			continue;

		SwapLines(a, b, used_rows, pivot);
		ProcessPivotElement(a, b, pivot);
		MarkPivotElementUsed(pivot, used_rows, used_columns);
	}

	for (int dim = 0; dim < size; ++dim)
	{
		auto itr = std::find_if(begin(a), end(a), [&](const Row &row)
		{
			return Equal(row[dim], 1.0);
		});
		assert(itr != end(a));

		if (itr != end(a))
		{
			std::swap(a[dim], a[itr - begin(a)]);
			std::swap(b[dim], b[itr - begin(a)]);
		}
	}

	return b;
}

pair<int, vector<double>> solve_diet_problem(size_t n, size_t m, const Matrix &A, const Column &b, const Column &c)
{
	// Write your code here

	return {0, vector<double>(m, 0)};
}

int my_main(std::istream &in, std::ostream &out)
{
	size_t n; // the number of restrictions on your diet (between 1 and 8 inclusive)
	size_t m; // the number of all available dishes and drinks respectively (between 1 and 8 inclusive)
	in >> n >> m;

	// inequalities
	Matrix A = ReadMatrix(in, n, m);

	// amounts of each ingredient
	Column b = ReadColumn(in, n);

	// pleasure of consuming each dish
	Column c = ReadColumn(in, m);

	pair<int, vector<double>> ans = solve_diet_problem(n, m, A, b, c);

	switch (ans.first)
		{
		case -1:
			out << "No solution\n";
			break;
		case 0:
			out << "Bounded solution\n";
			out << std::fixed << std::setprecision(18);
			std::copy(begin(ans.second), end(ans.second), std::ostream_iterator<double>(out, " "));
			out << "\n";
			break;
		case 1:
			out << "Infinity\n";
			break;
		}
	return 0;
}

int main()
{
	//my_main(std::cin, std::cout);

	auto Test = [](string input, string expected)
		{
		stringstream in(input);
		stringstream out;
		my_main(in, out);
		string actual = out.str();
		assert(actual == expected);
		};

	Test("3 2 -1 -1 1 0 0 1 -1 2 2 -1 2", "Bounded solution\n0.000000000000000 2.000000000000000\n");
	Test("2 2 1 1 -1 -1 1 -2 1 1", "No solution\n");
	Test("1 3 0 0 1 3 1 1 1", "Infinity\n");

	return 0;
}