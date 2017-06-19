#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using std::vector;
using std::pair;
using std::begin;
using std::end;

struct item
{
	int value = 0;
	int weight = 0;
	double ratio = 0.0;

	item(int value, int weight)
		: value(value)
		, weight(weight)
		, ratio(static_cast<double>(value) / weight)
	{}
};

double loot(int &capacity, vector<item> &items)
{
  double value = 0.0;

  auto itr = begin(items);

  while (capacity > 0 && itr != end(items))
  {
	  if (itr->weight <= capacity)
	  {
		  // found something that fits, so add to the loot
		  value += itr->value;
		  capacity -= itr->weight;
	  }
	  else
	  {
		  // nothing fits, so add a fraction of the most valuable
		  value += itr->ratio * capacity;
		  capacity = 0;
	  }

	  ++itr;
  }

  return value;
}

int main() {
  int n;
  int capacity;
  std::cin >> n >> capacity;

  vector<item> items;
  items.reserve(n);

  for (int i = 0; i < n; i++) {
	  {
		  int value;
		  int weight;
		  std::cin >> value >> weight;
		  items.emplace_back(item{ value, weight });
	  }
  }

  // sort items by decreasing ratio
  std::sort(begin(items), end(items), [](const item &a, const item &b) { return a.ratio > b.ratio; });

  double optimal_value = loot(capacity, items);

  std::cout << std::fixed << std::setprecision(4) << optimal_value << "\n";
  return 0;
}
