#include <iostream>
#include <vector>
#include <cassert>

using std::vector;

void reconstruct_solution(const vector<vector<int>> &value, size_t W, const vector<int> &w, const vector<int> &v)
{
	size_t r = value.size() - 1;
	size_t c = value[0].size() - 1;

	std::vector<bool> solution(r); // true if item was packed in the knapsack, false if not. 

	while (r > 0)
	{
		bool item_used = false;

		int item_weight = w[r - 1];
		int item_value = v[r - 1];

		if (value[r][c] >= item_value)
		{
			int a = value[r - 1][c - item_weight];
			int b = value[r - 1][c];
			item_used = (a + item_value > b);
		}
		
		if (item_used)
		{
			solution[r - 1] = true;
			c = c - item_weight;
			r--;
		}
		else
		{
			solution[r - 1] = false;
			c = c;
			r--;
		}
	}

	// go through solution and sum the knapsack contents
	int knapsack_weight = 0;
	int knapsack_value = 0;

	for (size_t i = 0; i < solution.size(); ++i)
	{
		std::cout << std::boolalpha << solution[i] << " ";
		if (solution[i])
		{
			knapsack_weight += w[i];
			knapsack_value += v[i];
		}
	}
	std::cout << "\n";

	// global check
	r = value.size() - 1;
	c = value[0].size() - 1;
	assert((size_t)knapsack_weight <= W);
	assert(knapsack_value == value[r][c]);
}

int optimal_weight(size_t W, const vector<int> &w, const vector<int> &v)
{
	size_t n = w.size();
	size_t rows = w.size() + 1;
	size_t cols = W + 1;

	vector<vector<int>> value;

	// construct and initialize
	for (size_t i = 0; i < rows; ++i)
		value.emplace_back(vector<int>(cols));
	for (size_t c = 0; c < cols; ++c)
		value[0][c] = 0;
	for (size_t r = 0; r < rows; ++r)
		value[r][0] = 0;

	// knapsack algorithm
	for (size_t i = 1; i <= n; ++i)
	{
		for (size_t j = 1; j <= W; ++j)
		{
			value[i][j] = value[i - 1][j];
			if ((size_t)w[i-1] <= j)
			{
				int val = value[i - 1][j - w[i-1]] + v[i-1];
				if (value[i][j] < val)
					value[i][j] = val;
			}
		}
	}

	// optional:
	//reconstruct_solution(value, W, w, v);

	return value[n][W];
}

int main()
{
	int n, W;
	std::cin >> W >> n;

	vector<int> weights(n);
	for (int i = 0; i < n; i++)
		std::cin >> weights[i];

	vector<int> values(n); // in the case of gold, the value is proportional to the weight

	std::cout << optimal_weight(W, weights, values) << '\n';
}
