#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <iomanip>

const double EPS = 1e-6;
const int PRECISION = 6;

#define IsZero(a) (fabs((a)) <= (EPS))
#define NotZero(a) (fabs((a)) > (EPS))

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

using std::string;
using std::stringstream;

struct Equation
{
	Equation(const Matrix &a, const Column &b):
		a(a),
		b(b)
	{}

	Matrix a;
	Column b;
};

struct Position
{
	Position(int column, int row):
		column(column),
		row(row)
	{}

	int column;
	int row;
};

Equation ReadEquation(std::istream &in)
{
	int size;
	in >> size;
	Matrix a(size, std::vector <double> (size, 0.0));
	Column b(size, 0.0);
	for (int row = 0; row < size; ++row)
		{
		for (int column = 0; column < size; ++column)
			in >> a[row][column];
		in >> b[row];
		}
	return Equation(a, b);
}

Position SelectPivotElement(
	const Matrix &a,
	std::vector <bool> &used_rows,
	std::vector <bool> &used_columns)
{
	// This algorithm selects the first free element.
	// You'll need to improve it to pass the problem.
	Position pivot_element(0, 0);
	while (used_rows[pivot_element.row])
		++pivot_element.row;
	while (used_columns[pivot_element.column] && NotZero(a[pivot_element.row][pivot_element.column]))
		++pivot_element.column;
	return pivot_element;
}

void SwapLines(Matrix &a, Column &b, std::vector <bool> &used_rows, Position &pivot_element)
{
	std::swap(a[pivot_element.column], a[pivot_element.row]);
	std::swap(b[pivot_element.column], b[pivot_element.row]);
	std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
	pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element)
{
	// Write your code here
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
		Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
		SwapLines(a, b, used_rows, pivot_element);
		ProcessPivotElement(a, b, pivot_element);
		MarkPivotElementUsed(pivot_element, used_rows, used_columns);
		}

	return b;
}

void PrintColumn(std::ostream &out, Column &column)
{
	int size = column.size();
	out << std::fixed << std::setprecision(PRECISION);
	for (int row = 0; row < size; ++row)
		{
		if (row > 0)
			out << " ";
		out << column[row];
		}
	out << "\n";
}

void my_main(std::istream &in, std::ostream &out)
{
	Equation equation = ReadEquation(in);
	Column solution = SolveEquation(equation);
	PrintColumn(out, solution);
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

	Test("0", "\n");
	Test("2 1 2 6 3 -1 -3", "0.000000 3.000000"); // slides
	Test("4 1 0 0 0 1 0 1 0 0 5 0 0 1 0 4 0 0 0 1 3", "1.000000 5.000000 4.000000 3.000000\n");
	Test("2 1 1 3 2 3 7", "2.000000 1.000000\n");
	Test("2 5 -5 -1 -1 -2 -1", "0.200000 0.400000\n");

	return 0;
}