#include <iostream>
#include <cstdint>

int64_t get_fibonacci_mod(int64_t n, int64_t m)
{
	if (n <= 1 || m <= 2)
		return n;

	return n;
}

int64_t get_pisano(int64_t m)
{
	return 0;
}

int64_t get_fibonacci_huge_mod(int64_t n, int64_t m)
{
	if (n <= 1 || m <= 2)
		return n;

	return get_fibonacci_mod(n % get_pisano(m), m);
}

int main()
{
	int64_t n, m;
	std::cin >> n >> m;
	std::cout << get_fibonacci_huge_mod(n, m) << '\n';
}
