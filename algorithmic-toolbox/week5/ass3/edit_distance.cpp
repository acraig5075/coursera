#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;
using std::cout;


void output_alignment(size_t i, size_t j, const vector<vector<int>> &d, const string &a, const string &b, string &a_, string &b_)
{
	if (i == 0 && j == 0)
	{
		return;
	}
	if (i > 0 && d[i][j] == d[i - 1][j] + 1)
	{
		output_alignment(i - 1, j, d, a, b, a_, b_);
		a_ += a[i-1];
		b_ += "-";
	}
	else if (j > 0 && d[i][j] == d[i][j - 1] + 1)
	{
		output_alignment(i, j - 1, d, a, b, a_, b_);
		a_ += "-";
		b_ += b[j-1];
	}
	else
	{
		output_alignment(i - 1, j - 1, d, a, b, a_, b_);
		a_ += a[i-1];
		b_ += b[j-1];
	}
}

int edit_distance(const string &str1, const string &str2)
{
	size_t rows = str1.size() + 1;
	size_t cols = str2.size() + 1;

	vector<vector<int>> distances;

	// construct and initialize
	for (size_t r = 0; r < rows; ++r)
		distances.emplace_back(vector<int>(cols));
	for (size_t c = 0; c < cols; ++c)
		distances[0][c] = (int)c;
	for (size_t r = 0; r < rows; ++r)
		distances[r][0] = (int)r;

	// fill
	for (size_t j = 1; j < cols; ++j)
	{
		for (size_t i = 1; i < rows; ++i)
		{
			int insertion = distances[i]  [j-1] + 1;
			int deletion =  distances[i-1][j]   + 1;
			int match =     distances[i-1][j-1];
			int mismatch =  distances[i-1][j-1] + 1;

			if (str1[i-1] == str2[j-1])
				distances[i][j] = std::min({ insertion, deletion, match });
			else
				distances[i][j] = std::min({ insertion, deletion, mismatch });
		}
	}

	//// optional output
	//string outstr1, outstr2;
	//output_alignment(rows - 1, cols - 1, distances, str1, str2, outstr1, outstr2);
	//cout << outstr1 << "\n";
	//cout << outstr2 << "\n";

	return distances[rows-1][cols-1];
}

int main()
{
	string str1;
	string str2;
	std::cin >> str1 >> str2;
	std::cout << edit_distance(str1, str2) << "\n";
	return 0;
}
