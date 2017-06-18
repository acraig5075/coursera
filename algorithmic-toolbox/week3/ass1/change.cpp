#include <iostream>
#include <vector>

int get_change(int m) {
	std::vector<int> denominations = { 10, 5, 1 };

	int n = 0;
	int value = m;
	for (auto d : denominations)
	{
		n += value / d;
		value = value % d;
	}

  return n;
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
