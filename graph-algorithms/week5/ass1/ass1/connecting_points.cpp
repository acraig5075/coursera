// ass1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <vector>
#include <set>

using std::istream;
using std::ostream;
using std::stringstream;
using std::string;
using std::vector;
using std::set;
using std::pair;


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

bool operator<(const Node &lhs, const Node &rhs)
{
	return lhs.id < rhs.id;
}

class MinimumSpanningTree
{
public:
	MinimumSpanningTree(size_t n)
		: n(n)
	{
	}

	void readEdges(std::istream &in)
	{
		for (size_t i = 0; i < n; ++i)
		{
			int x, y;
			in >> x >> y;
			sets[i].insert({ i, x, y });
		}
	}

	void kruskal()
	{
		sets.clear();
		edges.clear();
	
		sets.resize(n, set<Node>());

	}

private:
	size_t n = 0;
	vector<set<Node>> sets;
	vector<pair<Node, Node>> edges;
};

void my_main(istream &in, ostream &out)
{
	size_t n;
	in >> n;

	MinimumSpanningTree mst(n);

	mst.readEdges(in);
	
	out << std::setprecision(10) << "\n";
}

int main()
{
	my_main(std::cin, std::cout);

	auto Test = [](string input, string expected)
	{
		stringstream in(input);
		stringstream out;
		my_main(in, out);
		string actual = out.str();
		replace(begin(actual), end(actual), '\n', ' ');
		assert(actual == expected);
	};

	Test("4 0 0 0 1 1 0 1 1 ", "3.000000000");
	Test("5 0 0 0 2 1 1 3 0 3 2", "7.064495102");
	return 0;
}

