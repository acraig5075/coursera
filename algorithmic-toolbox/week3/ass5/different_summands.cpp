#include <iostream>
#include <vector>
#include <cassert>
#include <numeric>

using std::int64_t;
using std::vector;
using std::cin;
using std::cout;

vector<int> optimal_summands(int n)
{
	vector<int> summands;

	int i = 1;
	int sum = n;

	for (; sum - i > i || sum - i == 0; ++i)
		{
		summands.push_back(i);

		sum -= i;
		}

	if (sum > 0)
		summands.push_back(sum);

	return summands;
}

int main()
{
	int n;
	cin >> n;
	vector<int> summands = optimal_summands(n);

	cout << summands.size() << '\n';
	for (size_t i = 0; i < summands.size(); ++i)
		cout << summands[i] << ' ';

	cout << '\n';

	assert(n = std::accumulate(summands.begin(), summands.end(), 0));
	return 0;
	}
