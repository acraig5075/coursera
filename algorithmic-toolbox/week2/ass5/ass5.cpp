#include <iostream>
#include <cstdint>
#include <cassert>

int64_t get_fibonacci_mod(int64_t n, int64_t m)
{
	if (n <= 1)
		return n;

	int64_t previous = 0;
	int64_t current = 1;

	for (int64_t i = 0; i < n - 1; ++i)
		{
		int64_t tmp_previous = previous;
		previous = current;
		current = (tmp_previous + current) % m;
		}

	return current;
}

int64_t get_pisano(int64_t m)
{
	if (m <= 1)
		return m;

	int64_t previous = 0;
	int64_t current = 1;

	for (int64_t i = 2; ; ++i)
		{
		int64_t tmp_previous = previous;
		previous = current;
		current = tmp_previous + current;

		if (i > 2 && (previous % m == 0) && (current % m == 1))
			return i - 1;
		}

	return 0;
}

int64_t get_fibonacci_huge_mod(int64_t n, int64_t m)
{
	if (n <= 1 || m <= 2)
		return n;

	int64_t p = get_pisano(m);

	return get_fibonacci_mod(n % p, m);
}

int main()
{
	int64_t n, m;
	std::cin >> n >> m;
	std::cout << get_fibonacci_huge_mod(n, m) << '\n';
	return 0;
}
