#include <algorithm>
#include <iostream>
#include <vector>
#include <cassert>

using std::vector;

int get_majority_element(const vector<int>::iterator &left, const vector<int>::iterator &right)
{
	if (left == right)
		return *left;

	auto n = std::distance(left, right) + 1;
	auto half = n / 2;

	assert(half != 0);

	int ml = get_majority_element(left, left + half - 1);
	int mr = get_majority_element(left + half, right);

	if (ml != -1)
	{
		if (std::count(left, right + 1, ml) > half)
			return ml;
	}
	if (mr != -1)
	{
		if (std::count(left, right + 1, mr) > half)
			return mr;
	}

	return -1;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }

  std::cout << (get_majority_element(a.begin(), a.end() - 1) != -1) << '\n';
}
