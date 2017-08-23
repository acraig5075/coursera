#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cassert>
#include <cstdint>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using std::vector;
using std::string;
using std::ios_base;
using std::cin;
using std::cout;
using std::cout;
using std::int64_t;

struct Seq
{
	Seq(int i, int k)
		: ind(i)
		, key(k)
	{}

	int key;
	int ind;
};

class TreeOrders
{
	int n;
	vector <int> key;
	vector <int> left;
	vector <int> right;
	vector <Seq> result;

	bool bst = true;
	int64_t minKey;
	int64_t maxKey;

public:
	void read(std::istream &in)
	{
		in >> n;
		key.resize(n);
		left.resize(n);
		right.resize(n);
		result.reserve(n);

		for (int i = 0; i < n; i++)
			{
			in >> key[i] >> left[i] >> right[i];
			}
	}

	void in_order_traversal(int i)
	{
		if (i == -1 || i >= n)
			return;

		in_order_traversal(left[i]);
		result.push_back({ i, key[i] });
		in_order_traversal(right[i]);
	}

	vector <Seq> in_order()
	{
		result.clear();
		in_order_traversal(0);
		return result;
	}

	//void test_node(int i)
	//{
	//	if (!bst || i == -1 || i >= n)
	//		return;

	//	if (i == 0)
	//		minKey = maxKey = key[0];

	//	int thisKey = key[i];
	//	int li = left[i];
	//	int ri = right[i];

	//	if (li != -1)
	//	{
	//		int lk = key[li];
	//		if (lk >= thisKey)
	//		{
	//			bst = false;
	//			return;
	//		}

	//		if (lk < minKey)
	//			minKey = lk;
	//	}

	//	test_node(li);

	//	if (ri != -1)
	//	{
	//		int rk = key[ri];
	//		if (rk < thisKey || rk > maxKey)
	//		{
	//			bst = false;
	//			return;
	//		}

	//		if (rk > maxKey)
	//			maxKey = rk;
	//	}
	//	test_node(ri);
	//}

	void test_node(int i)
	{
		int thisKey = key[i];
	
		int li = left[i];
		if (li != -1)
		{
			int lk = key[li];
			if (lk < thisKey && lk < maxKey)
			{
				if (lk < minKey)
					minKey = lk;
			}
			else
			{
				bst = false;
				return;
			}
		}

		int ri = right[i];
		if (ri != -1)
		{
			int rk = key[ri];
			if (rk >= thisKey && rk > minKey)
			{
				if (rk > maxKey)
					maxKey = rk;
			}
			else
			{
				bst = false;
				return;
			}
		}
	}

	bool is_bst()
	{
		in_order();

		if (result.empty() || result.size() == 1)
			return true;

		auto i = begin(result);
		auto j = i + 1;

		for (; j != end(result); ++i, ++j)
		{
			if (i->key > j->key)
				return false;

			if (i->key == j->key && right[i->ind] != j->ind)
				return false;
		}

		return true;
	}
};

int main_with_large_stack_space()
{
	auto Test = [](bool expected, const string &test)
	{
		TreeOrders t;
		std::stringstream ss(test);
		t.read(ss);
		assert(t.is_bst() == expected);
	};

	Test(true, "15 50 1 2 40 3 4 60 5 6 20 7 8  45 9 10 55 11 12 70 13 14 10 -1 -1 30 -1 -1 42 -1 -1 47 -1 -1 52 -1 -1 57 -1 -1 65 -1 -1 80 -1 -1");

	Test(true, "3 2 1 2 1 -1 -1 3 -1 -1");
	Test(false, "3 1 1 2 2 -1 -1 3 -1 -1 ");
	Test(true, "3 2 1 2 1 -1 -1 2 -1 -1 ");
	Test(false, "3 2 1 2 2 -1 -1 3 -1 -1 ");
	Test(true, "0");
	Test(true, "1 2147483647 -1 -1 ");
	Test(true, "5 1 -1 1 2 -1 2 3 -1 3 4 -1 4 5 -1 -1 ");
	Test(true, "7 4 1 2 2 3 4 6 5 6 1 -1 -1 3 -1 -1 5 -1 -1 7 -1 -1 ");
	Test(true, "3 2 1 2 1 -1 -1 3 -1 -1 ");
	Test(false, "3 1 1 2 2 -1 -1 3 -1 -1");
	Test(true, "5 1 -1 1 2 -1 2 3 -1 3 4 -1 4 5 -1 -1 ");
	Test(true, "5 77 -1 1 77 -1 2 77 -1 3 77 -1 4 77 -1 -1 ");
	Test(true, "7 4 1 2 2 3 4 6 5 6 1 -1 -1 3 -1 -1 5 -1 -1 7 -1 -1 ");
	Test(false, "4 4 1 -1 2 2 3 1 -1 -1 5 -1 -1 ");

	Test(false, "8 4 1 2 2 3 4 6 5 6 1 -1 -1 3 -1 7 5 -1 -1 7 -1 -1 8 -1 -1");
	Test(true, "4 -3 -1 1 -1 -1 2 0 3 -1 -1 -1 -1");
	Test(true, "3 5 -1 1 8 2 -1 5 -1 -1");
	Test(true, "5 2147483647 1 2 0 3 4 2147483647 -1 -1 - 2147483648 -1 -1 0 -1 -1");

	//ios_base::sync_with_stdio(0);
	//TreeOrders t;
	//t.read(cin);

	//if (t.is_bst())
	//	cout << "CORRECT\n";
	//else
	//	cout << "INCORRECT\n";
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

