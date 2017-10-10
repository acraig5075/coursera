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
	int lengthSqr = 0;
	double length = 0.0;

	Edge(size_t a, size_t b, int lengthSqr)
		: a(a)
		, b(b)
		, lengthSqr(lengthSqr)
		, length(std::sqrt(lengthSqr))
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

int length2(const Node &a, const Node &b)
{
	int dx = a.x - b.x;
	int dy = a.y - b.y;
	return (dx * dx) + (dy * dy);
}

bool sortEdgeFunc(const Edge &lhs, const Edge &rhs)
{
	if (lhs.lengthSqr == rhs.lengthSqr)
	{
		if (lhs.a == rhs.a)
			return lhs.b < rhs.b;
		else
			return lhs.a < rhs.a;
	}
	else
	{
		return lhs.lengthSqr < rhs.lengthSqr;
	}
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
				int lengthSqr = length2(*itr1, *itr2);

				edgeList.push_back(Edge{ a, b, lengthSqr });
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

			auto itr = begin(sets) + s2;
			sets.erase(itr);
		}

		size_t size() const
		{
			return sets.size();
		}

	private:
		vector<set<T>> sets;
	};

	double kruskal(size_t clusters)
		{
		DisjointSets<size_t> sets(n);
		for (const auto &n : nodeList)
			sets.MakeSet(n.id);

		vector<Edge> x;

		sort(begin(edgeList), end(edgeList), sortEdgeFunc);

		auto edge = begin(edgeList);
		for (; edge != end(edgeList); ++edge)
			{
			pair<size_t, size_t> find1 = sets.Find(edge->a);
			pair<size_t, size_t> find2 = sets.Find(edge->b);

			size_t s1 = find1.first;
			size_t s2 = find2.first;

			if (s1 != s2)
				{
				x.push_back(*edge);
				sets.Union(s1, s2);
				}

			if (sets.size() < clusters)
				break;
		}

		if (edge == end(edgeList))
			return 0.0;

		return edge->length;
	}

	double minimumWeight(const vector<Edge> &tree)
	{
		double sum = 0.0;
		for (auto &e : tree)
			sum += e.length;

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

	size_t clusters;
	in >> clusters;

	double separation = mst.kruskal(clusters);

	out << std::fixed << std::setprecision(9) << separation << "\n";
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

	//Test("12 7 6 4 3 5 1 1 7 2 7 5 7 3 3 7 8 2 8 4 4 6 7 2 6 3", "2.828427125\n");
	//Test("8 3 1 1 2 4 6 9 8 9 9 8 9 3 11 4 12 4 ", "5.000000000\n");
	//Test("5 0 0 0 2 1 1 3 0 3 2 2", "2.236067977\n");
	//Test("12 7 6 4 3 5 1 1 7 2 7 5 7 3 3 7 8 2 8 4 4 6 7 2 6 12", "1.000000000\n");
	//Test("0", "0.000000000\n");
	//Test("1 10 20 1", "0.000000000\n");
	//Test("2 5 0 10 0 2", "5.000000000\n");
	//Test("2 5 0 10 0 1", "0.000000000\n");

	return 0;
}
