#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using std::vector;
using std::pair;
using std::begin;
using std::end;

double loot(int &capacity, vector<std::pair<int, int>> &items)
{
  double value = 0.0;

  items.erase(std::remove_if(begin(items), end(items), [capacity](const std::pair<int, int> &i) { return i.second > capacity; }), end(items));

  if (!items.empty())
  {
	  auto itr = items.cbegin();
	  value += itr->first;
	  capacity -= itr->second;
	  items.erase(itr);
  }

  return value;
}

int main() {
  int n;
  int capacity;
  std::cin >> n >> capacity;

  vector<pair<int,int>> items(n);

  for (int i = 0; i < n; i++) {
    std::cin >> items[i].first >> items[i].second;
  }

  std::sort(begin(items), end(items), [](pair<int, int> a, pair<int, int> b) { return a.first > b.first; });

  double optimal_value = 0.0;
  while (capacity > 0 && !items.empty())
  {
    optimal_value += loot(capacity, items);
  }

  std::cout << std::fixed << std::setprecision(4) << optimal_value << "\n";
  return 0;
}
