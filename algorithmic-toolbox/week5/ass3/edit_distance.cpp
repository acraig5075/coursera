#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;

int at(size_t r, size_t c, const vector<vector<int>> &scores)
{
	return scores[r][c];
}

int &at(size_t r, size_t c, vector<vector<int>> &scores)
{
	return scores[r][c];
}

void print(const vector<vector<int>> &scores)
{
	for (size_t r = 0; r < scores.size(); ++r)
		{
		for (size_t c = 0; c < scores[r].size(); ++c)
			{
			cout << at(r, c, scores) << " ";
			}
		cout << "\n";
		}
	cout << "\n";
}


int edit_distance(const string &str1, const string &str2)
{
	size_t rows = str1.size() + 1;
	size_t cols = str2.size() + 1;

	vector<vector<int>> scores;

	for (size_t r = 0; r < rows; ++r)
		scores.emplace_back(vector<int>(cols));
	for (size_t c = 0; c < cols; ++c)
		at(0, c, scores) = (int)c;
	for (size_t r = 0; r < rows; ++r)
		at(r, 0, scores) = (int)r;

	print(scores);

	return 0;
}

int main()
{
	string str1;
	string str2;
	std::cin >> str1 >> str2;
	std::cout << edit_distance(str1, str2) << "\n";
	return 0;
}
