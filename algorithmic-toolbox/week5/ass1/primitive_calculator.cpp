#include <iostream>
#include <vector>
#include <algorithm>


using std::vector;
using std::begin;
using std::end;


vector<size_t> optimal_sequence(size_t n)
{
	vector<int> operations(n + 1);
	vector<size_t> indices(n + 1);

	auto compute = [&](int count, size_t i, size_t x)
	{
		if (x <= n)
		{
			int &op = operations.at(x);
			if (op == 0 || count < op)
			{
				op = count;
				indices[x] = i;
			}
		}
	};

	size_t i = 1;

	while (true)
	{
		int count = operations[i] + 1;

		compute(count, i, i * 2);
		compute(count, i, i * 3);
		compute(count, i, i + 1);

		if (i == n)
			break;
		i++;
	}

	vector<size_t> sequence;

	size_t x = n;
	while (x > 0)
	{
		sequence.push_back(x);
		x = indices.at(x);
	}

	std::reverse(begin(sequence), end(sequence));
	return sequence;
}

int main()
{
	size_t n;
	std::cin >> n;

	vector<size_t> sequence = optimal_sequence(n);

	std::cout << sequence.size() - 1 << std::endl;
	for (size_t i = 0; i < sequence.size(); ++i)
	{
		std::cout << sequence[i] << " ";
	}
	std::cout << "\n";
}
