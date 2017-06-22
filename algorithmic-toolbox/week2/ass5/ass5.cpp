#include <iostream>
#include <cstdint>
#include <cassert>
#include <array>

std::array<short, 144> wiki =
{
	1, 3, 8, 6, 20, 24, 16, 12, 24, 60, 10, 24,
	28, 48, 40, 24, 36, 24, 18, 60, 16, 30, 48, 24,
	100, 84, 72, 48, 14, 120, 30, 48, 40, 36, 80, 24,
	76, 18, 56, 60, 40, 48, 88, 30, 120, 48, 32, 24,
	112, 300, 72, 84, 108, 72, 20, 48, 72, 42, 58, 120,
	60, 30, 48, 96, 140, 120, 136, 36, 48, 240, 70, 24,
	148, 228, 200, 18, 80, 168, 78, 120, 216, 120, 168, 48,
	180, 264, 56, 60, 44, 120, 112, 48, 120, 96, 180, 48,
	196, 336, 120, 300, 50, 72, 208, 84, 80, 108, 72, 72,
	108, 60, 152, 48, 76, 72, 240, 42, 168, 174, 144, 120,
	110, 60, 40, 30, 500, 48, 256, 192, 88, 420, 130, 120,
	144, 408, 360, 36, 276, 48, 46, 240, 32, 210, 140, 24
};

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

int64_t get_pisano_period(int64_t m)
{
	if (m <= 1)
		return m;

	int64_t a = 0;
	int64_t b = 1;
	int64_t c = a + b;

	for (int64_t i = 0; i < m * m; i++)
		{
		c = (a + b) % m;
		a = b;
		b = c;

		if (a == 0 && b == 1)
			return i + 1;
		}

	return 0;
	}

int64_t get_fibonacci_huge_mod(int64_t n, int64_t m)
{
	int64_t p = get_pisano_period(m);

	return get_fibonacci_mod(n % p, m);
}

void test()
{
	for (size_t i = 0; i < wiki.size(); ++i)
		{
		if (get_pisano_period(i + 1) != wiki.at(i))
			int stop = 1;
		}
}

int main()
{
	//test();

	int64_t n, m;
	std::cin >> n >> m;
	std::cout << get_fibonacci_huge_mod(n, m) << '\n';
	return 0;
}
