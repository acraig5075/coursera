#include <iostream>
#include <cassert>
#include <vector>

using std::vector;

int binary_search(const vector<int> &a, int x) {
  size_t left = 0, right = a.size() - 1;

  while (right - left > 1)
  {
	size_t mid = left + ((right - left) / 2);
	int mid_val = a[mid];

	if (x > mid_val)
		left = mid;
	else if (x < mid_val)
		right = mid;
	else
		return mid;
  }

	if (a[right] == x)
		return right;
	else if (a[left] == x)
		return left;
	else
		return -1;
}


int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }

  int m;
  std::cin >> m;
  vector<int> b(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> b[i];
  }

  for (int i = 0; i < m; ++i) {
    std::cout << binary_search(a, b[i]) << ' ';
  }

  std::cout << '\n';

  return 0;
}
