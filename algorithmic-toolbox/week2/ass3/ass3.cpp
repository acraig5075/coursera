#include <iostream>
#include <utility>

int remainder(int a, int b)
{
	while (a >= b)
		a -= b;

	return a;
}

int gcd(int a, int b)
{
	if (b > a)
		std::swap(a, b);

	while (b > 0)
		{
		int rem = remainder(a, b);
		a = b;
		b = rem;
		}

	return a;
}

int main()
{
	int a, b;
	std::cin >> a >> b;
	std::cout << gcd(a, b) << "\n";
	return 0;
}
