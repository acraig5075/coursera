#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <cassert>
#include <numeric>
#include <algorithm>

using std::vector;
using std::pair;

const double EPS = 1.0e-6;
const double BIG = 10.0e9;

bool IsZero(double a)                { return fabs(a) <= EPS; }
bool NotZero(double a)               { return fabs(a) > EPS; }
//bool Equal(double a, double b)       { return fabs(a - b) <= (EPS + EPS * fabs(b)); }
//bool LessOrEqual(double a, double b) { return a <= (b + (EPS + EPS * fabs(b))); }
//bool GreaterThan(double a, double b)     { return a >  (b + (EPS + EPS * fabs(b))); }
//bool Equal(double a, double b)       { return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * EPS); }
bool Equal(double a, double b)       { return fabs(a - b) <= EPS; }
bool GreaterThan(double a, double b) { return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPS); }
bool LessThan(double a, double b)    { return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPS); }

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

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

Matrix ReadMatrix(std::istream &in, size_t n, size_t m)
{
	Matrix mat(n, Row(m));
	for (size_t r = 0; r < n; ++r)
	{
		for (size_t c = 0; c < m; ++c)
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

void AppendBounds(Matrix &inequal, Column &amounts, size_t m)
{
	// m inequalities of the form amount[i] >= 0
	for (size_t i = 0; i < m; ++i)
	{
		Row row(m, 0.0);
		row[i] = -1.0;
		inequal.push_back(row);
		amounts.push_back(0.0);
	}

	// one inequality to distinguish between bounded and unbounded cases
	Row row(m, 1.0);
	inequal.push_back(row);
	amounts.push_back(BIG);
}

Position SelectPivotElement(
	const Matrix &a,
	std::vector <bool> &used_rows,
	std::vector <bool> &used_columns)
{
	int size = a.size();

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
	{
		if (i == pivot_element.column)
			a[pivot_element.row][i] = 1.0;
		else
			a[pivot_element.row][i] /= constant;
	}
	
	if (IsZero(b[pivot_element.row]))
		b[pivot_element.row] = 0.0;
	else
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
		if (IsZero(factor))
			continue;

		for (int c = 0; c < size; ++c)
		{
			if (Equal(a[r][c], pivot_row[c] * factor))
				a[r][c] = 0.0;
			else
				a[r][c] += pivot_row[c] * factor;

			if (IsZero(a[r][c]))
				a[r][c] = 0.0;
		}

		if (Equal(b[r], b[0] * factor))
			b[r] = 0.0;
		else
			b[r] += b[0] * factor;

		if (IsZero(b[r]))
			b[r] = 0.0;
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

void PrintSolution(std::ostream &out, bool ok, const Column &b)
{
	if (ok)
	{
		out << "Solution:";
		for (size_t c = 0; c < b.size(); ++c)
			out << std::fixed << std::setprecision(2) << std::setw(10) << b[c] << " ";
		out << "\n";
	}
	else
	{
		out << "Solution: <none>\n";
	}
}

Column SolveEquation(Equation &equation, bool &ok)
{
	ok = true;
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
		//assert(itr != end(a));

		if (itr != end(a))
		{
			std::swap(a[dim], a[itr - begin(a)]);
			std::swap(b[dim], b[itr - begin(a)]);
		}
		else
		{
			ok = false;
			break;
		}
	}

	//PrintSolution(std::cout, ok, b);

	return b;
}

vector<vector<size_t>> getAllSubsets(vector<size_t> set)
{
	vector< vector<size_t> > subset;
	vector<size_t> empty;
	subset.push_back(empty);

	for (size_t i = 0; i < set.size(); i++)
	{
		vector< vector<size_t> > subsetTemp = subset;

		for (size_t j = 0; j < subsetTemp.size(); j++)
			subsetTemp[j].push_back(set[i]);

		for (size_t j = 0; j < subsetTemp.size(); j++)
			subset.push_back(subsetTemp[j]);
	}

	return subset;
}


void PrintMatrix(std::ostream &out, Matrix &A, const Column &b)
{
	assert(A.size() == b.size());

	for (size_t r = 0; r < A.size(); ++r)
	{
		for (size_t c = 0; c < A[0].size(); ++c)
			out << std::fixed << std::setprecision(2) << std::setw(10) << A[r][c] << " ";
		out << "| " << std::setw(14) << b[r] << "\n";
	}
	out << "\n";
}

pair<int, vector<double>> SolveDietProblem(size_t n, size_t m, const Matrix &A, const Column &b, const Column &c)
{
	size_t dim = A.size();

	// get all possible subsets of size m
	std::vector<size_t> indexes(dim);
	std::iota(begin(indexes), end(indexes), 0);
	vector<vector<size_t>> subsets = getAllSubsets(indexes);
	subsets.erase(std::remove_if(begin(subsets), end(subsets), [m](const vector<size_t> &s)
	{
		return s.size() != m;
	}), end(subsets));

	double maxPleasure = std::numeric_limits<double>::lowest();
	pair<int, vector<double>> ret = std::make_pair(-1, vector<double>(m, 0));

	// for each subset solve the system of linear inequalities
	for (const auto s : subsets)
	{
		Matrix lhs;
		Column rhs;
		for (auto i : s)
		{
			lhs.push_back(A[i]);
			rhs.push_back(b[i]);
		}

		//PrintMatrix(std::cout, lhs, rhs);

		assert(lhs.size() == m);
		assert(lhs[0].size() == m);
		assert(rhs.size() == m);

		Equation eq(lhs, rhs);
		bool ok;
		Column soln = SolveEquation(eq, ok);
		if (!ok)
			continue;

		assert(soln.size() == m);

		// check whether this solution satisfies all the other inequalities
		bool satisfies = true;
		for (size_t r = 0; r < dim; ++r)
		{
			double left = 0.0;
			for (size_t c = 0; c < m; ++c)
			{
				left += A[r][c] * soln[c];
			}

			double right = b[r];
			if (GreaterThan(left, right))
			{
				satisfies = false;
				if (r == dim - 1)
					ret.first = 1;
			}
		}

		// pleasure
		if (satisfies)
		{
			assert(soln.size() == c.size());
			double pleasure = std::inner_product(begin(soln), end(soln), begin(c), 0.0);
			if (GreaterThan(pleasure, maxPleasure))
			{
				maxPleasure = pleasure;
				ret.first = 0;
				ret.second = soln;
			}
		}
	}

	return ret;
}

int my_main(std::istream &in, std::ostream &out)
{
	size_t n; // n regular inequalities:	the number of restrictions on your diet (between 1 and 8 inclusive)
	size_t m; // m variables:				the number of all available dishes and drinks respectively (between 1 and 8 inclusive)
	in >> n >> m;

	// regular inequalities
	Matrix A = ReadMatrix(in, n, m);

	// amounts of each ingredient
	Column b = ReadColumn(in, n);

	// pleasure of consuming each dish
	Column c = ReadColumn(in, m);

	// append further inequalities
	AppendBounds(A, b, m);

	//PrintMatrix(std::cout, A, b);

	pair<int, vector<double>> ans = SolveDietProblem(n, m, A, b, c);

	switch (ans.first)
		{
		case -1:
			out << "No solution\n";
			break;
		case 0:
			out << "Bounded solution\n";
			out << std::fixed << std::setprecision(18);
			for (size_t i = 0; i < ans.second.size(); ++i)
			{
				if (i > 0)
					out << " ";
				out << (IsZero(ans.second[i]) ? 0.0 : ans.second[i]);
			}
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

	auto Test = [](const std::string &input, const std::string &expected)
		{
		std::stringstream in(input);
		std::stringstream out;
		my_main(in, out);
		std::string actual = out.str();
		//assert(actual == expected);
		if (actual == expected)
			std::cout << "Pass\n";
		else
		{
			std::string bounded("Bounded solution\n");
			if (actual.compare(0, bounded.length(), bounded) == 0 && expected.compare(0, bounded.length(), bounded) == 0)
			{
				auto GetValues = [](const std::string &s)
				{
					std::string str;
					std::stringstream ss1(s);
					std::getline(ss1, str);
					std::getline(ss1, str);
					std::stringstream ss2(str);
					std::vector<double> vals;
					double v;
					while (ss2 >> v)
						vals.push_back(v);
					return vals;
				};

				std::vector<double> vals1 = GetValues(actual);
				std::vector<double> vals2 = GetValues(expected);
				size_t differences = std::inner_product(begin(vals1), end(vals1), begin(vals2), 0, std::plus<>(), [](double a, double b) { return Equal(a, b) ? 0 : 1; });
				if (differences == 0)
					std::cout << "Pass\n";
				else
					std::cout << "Fail\n";
			}
			else
				std::cout << "Fail\n";
		}
		};

	Test("3 2 -1 -1 1 0 0 1 -1 2 2 -1 2", "Bounded solution\n0.000000000000000000 2.000000000000000000\n");
	Test("2 2 1 1 -1 1 10 -10 1 2", "Bounded solution\n10.000000000000000000 0.000000000000000000\n");
	Test("2 2 1 1 -1 -1 1 -2 1 1", "No solution\n");
	Test("3 7 78 15 -76 -66 -27 33 -33 -22 78 -53 -17 -34 -32 20 -63 7 -77 -5 76 19 47 123584 -276905 134213 -85 -31 0 -71 -66 -86 -60", "Bounded solution\n0.000000000000000000 0.000000000000000000 5224.622641509434000000 0.000000000000000000 0.000000000000000000 0.000000000000000000 0.000000000000000000\n");
	Test("2 3 13 54 47 -61 -45 2 4382 -5006 -97 -66 -55", "Bounded solution\n26.996677740863785000 74.648947951273527000 0.000000000000005225\n");
	Test("3 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0", "Bounded solution\n0.000000000000000000 0.000000000000000000 0.000000000000000000\n");
	Test("1 1 -3 359642 0", "Bounded solution\n0.000000000000000\n");
	Test("1 3 0 0 1 3 1 1 1", "Infinity\n");
	Test("2 7 -60 -53 65 53 56 -2 -14 -42 11 -4 -47 -9 9 -54 424963 630662 -46 -16 4 12 -60 -50 0", "Infinity\n");

	return 0;
}