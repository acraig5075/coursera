#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

using std::vector;
using std::begin;
using std::end;

int64_t product(int a, int b)
{
	return static_cast<int64_t>(a) * b;
}

int64_t max_dot_product(const vector<int> &a, const vector<int> b) 
{
	return std::inner_product(begin(a), end(a), begin(b), (int64_t)0, std::plus<>(), product);
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> a(n), b(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  for (size_t i = 0; i < n; i++) {
    std::cin >> b[i];
  }

  std::sort(begin(a), end(a));
  std::sort(begin(b), end(b));

  std::cout << max_dot_product(a, b) << "\n";
}
