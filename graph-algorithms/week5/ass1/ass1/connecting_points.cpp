#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <vector>
#include <set>
#include <cmath>

using std::istream;
using std::ostream;
using std::stringstream;
using std::string;
using std::vector;
using std::set;
using std::pair;


// Node has coordinate and ID
struct Node
{
	size_t id = 0;
	int x = 0;
	int y = 0;

	Node(size_t id, int x, int y)
		: id(id)
		, x(x)
		, y(y)
	{}

};

// Edge has two node terminals and a weight
struct Edge
{
	size_t a, b;
	double weight = 0.0;

	Edge(size_t a, size_t b, double weight)
		: a(a)
		, b(b)
		, weight(weight)
	{}
};

bool operator<(const Node &lhs, const Node &rhs)
{
	return lhs.id < rhs.id;
}

double length(const Node &a, const Node &b)
{
	int dx = a.x - b.x;
	int dy = a.y - b.y;
	return std::sqrt((dx * dx) + (dy * dy));
}

// Minimum Spanning Tree with Kruskal algorithm for minimum weight.
class MinimumSpanningTree
{
public:
	MinimumSpanningTree(size_t n)
		: n(n)
	{
	}

	void read(std::istream &in)
	{
		readNodes(in);
		makeEdges();
	}

	void readNodes(std::istream &in)
	{
		nodeList.clear();

		for (size_t i = 1; i <= n; ++i)
			{
			int x, y;
			in >> x >> y;
			nodeList.push_back(Node{ i, x, y });
			}
	}

	void makeEdges()
	{
		edgeList.clear();

		auto itr1 = nodeList.begin();
		for (size_t a = 1; a < n; ++a, ++itr1)
			{
			auto itr2 = itr1 + 1;
			for (size_t b = a + 1; b <= n; ++b, ++itr2)
				{
				double weight = length(*itr1, *itr2);

				edgeList.push_back(Edge{ a, b, weight });
				}
			}
	}

	// Naive implementation of Disjoint Sets.
	template<class T>
	class DisjointSets
	{
	public:
		DisjointSets(size_t initial)
		{
			sets.reserve(initial);
		}

		void MakeSet(T value)
		{
			set<T> s;
			s.insert(value);
			sets.push_back(s);
		}

		pair<size_t, size_t> Find(T value)
		{
			for (auto itr1 = begin(sets); itr1 != end(sets); ++itr1)
				{
				auto itr2 = itr1->find(value);

				if (itr2 != end(*itr1))
					{
					size_t outer = std::distance(begin(sets), itr1);
					size_t inner = std::distance(begin(*itr1), itr2);
					return std::make_pair(outer, inner);
					}
				}

			assert(false);
			return std::make_pair(sets.size(), 0);
		}

		void Union(size_t s1, size_t s2)
		{
			assert(s1 < sets.size());
			assert(s2 < sets.size());

			if (s1 >= sets.size() || s2 >= sets.size())
				return;

			auto &set1 = sets.at(s1);
			auto &set2 = sets.at(s2);

			set1.insert(begin(set2), end(set2));
			set2.clear();
		}

	private:
		vector<set<T>> sets;
	};

	vector<Edge> kruskal()
		{
		DisjointSets<size_t> sets(n);
		for (const auto &n : nodeList)
			sets.MakeSet(n.id);

		vector<Edge> x;

		sort(begin(edgeList), end(edgeList), [](const Edge & a, const Edge & b)
			{
			return a.weight < b.weight;
			});

		for (const auto &e : edgeList)
			{
			pair<size_t, size_t> find1 = sets.Find(e.a);
			pair<size_t, size_t> find2 = sets.Find(e.b);

			size_t s1 = find1.first;
			size_t s2 = find2.first;

			if (s1 != s2)
				{
				x.push_back(e);
				sets.Union(s1, s2);
				}
			}

		return x;
	}

	double minimumWeight(const vector<Edge> &tree)
	{
		double sum = 0.0;
		for (auto &e : tree)
			sum += e.weight;

		return sum;
	}

private:
	size_t n = 0;
	vector<Node> nodeList;
	vector<Edge> edgeList;
};

void my_main(istream &in, ostream &out)
{
	size_t n;
	in >> n;

	MinimumSpanningTree mst(n);

	mst.read(in);

	auto tree = mst.kruskal();

	double length = mst.minimumWeight(tree);

	out << std::fixed << std::setprecision(9) << length << "\n";
}

int main()
{
	my_main(std::cin, std::cout);

	//auto Test = [](string input, string expected)
	//	{
	//	stringstream in(input);
	//	stringstream out;
	//	my_main(in, out);
	//	string actual = out.str();
	//	assert(actual == expected);
	//	};

	//Test("4 0 0 0 1 1 0 1 1 ", "3.000000000\n");
	//Test("5 0 0 0 2 1 1 3 0 3 2", "7.064495102\n");
	//Test("9 10 5 30 5 20 -5 30 -5 0 0  10 0 20 0 30 0 40 0", "60.000000000\n");
	//Test("1 10 20", "0.000000000\n");
	//Test("0", "0.000000000\n");
	//Test("3 10 5 10 0 20 0", "15.000000000\n");
	//Test("12 10000 50000 10000 50001 10000 50002 20000 50000 20000 50001 20000 50002 30000 50000 30000 50001 30000 50002 40000 50000 40000 50001 40000 50002", "30008.000000000\n");

	return 0;
}
