#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using std::vector;
using std::cin;
using std::cout;
using std::begin;
using std::end;

int64_t MaxPairwiseProduct(vector<int> &numbers)
{
	if (numbers.size() < 2)
		return 0;

	std::sort(begin(numbers), end(numbers), [](int a, int b)
		{
		return a > b;
		});

	return static_cast<int64_t>(numbers.at(0)) * static_cast<int64_t>(numbers.at(1));
}

int main()
{
	int n;
	cin >> n;
	vector<int> numbers;
	numbers.reserve(n);

	int v;
	for (int i = 0; i < n; ++i)
		{
		cin >> v;
		numbers.push_back(v);
		}

	cout << MaxPairwiseProduct(numbers) << "\n";
	return 0;
}
