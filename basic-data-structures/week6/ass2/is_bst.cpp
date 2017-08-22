#include <iostream>
#include <vector>
#include <algorithm>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using std::vector;
using std::ios_base;
using std::cin;
using std::cout;

class TreeOrders
{
	int n;
	vector <int> key;
	vector <int> left;
	vector <int> right;
	vector <int> result;

public:
	void read()
	{
		cin >> n;
		key.resize(n);
		left.resize(n);
		right.resize(n);
		result.reserve(n);
		for (int i = 0; i < n; i++)
			{
			cin >> key[i] >> left[i] >> right[i];
			}
	}

	void in_order_traversal(int i)
	{
		if (i == -1 || i >= n)
			return;

		in_order_traversal(left[i]);
		result.push_back(key[i]);
		in_order_traversal(right[i]);
	}

	vector <int> in_order()
	{
		result.clear();
		in_order_traversal(0);
		return result;
	}

	bool is_bst()
	{
		in_order();

		return result.empty() || std::is_sorted(begin(result), end(result));
	}
};

int main_with_large_stack_space()
{
	ios_base::sync_with_stdio(0);
	TreeOrders t;
	t.read();

	if (t.is_bst())
		cout << "CORRECT\n";
	else
		cout << "INCORRECT\n";
	return 0;
}

int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
  // Allow larger stack space
  const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
  {
      if (rl.rlim_cur < kStackSize)
      {
          rl.rlim_cur = kStackSize;
          result = setrlimit(RLIMIT_STACK, &rl);
          if (result != 0)
          {
              std::cerr << "setrlimit returned result = " << result << std::endl;
          }
      }
  }
#endif

  return main_with_large_stack_space();
}

