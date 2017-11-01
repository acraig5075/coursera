#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;
using std::pair;
using std::stringstream;

using edges = map<char, size_t>;
using trie = vector<edges>;

auto find_edge(trie &t, size_t node, char symbol) -> pair<bool, size_t>;
auto add_edge(trie &t, size_t node, size_t newNode, char symbol) -> void;

trie build_trie(vector<string> & patterns)
{
	trie t;

	if (!patterns.empty())
		t.push_back(edges());

	for (string pattern : patterns)
	{
		size_t currentNode = 0;

		for (char currentSymbol : pattern)
		{
			auto found = find_edge(t, currentNode, currentSymbol);

			if (found.first)
			{
				currentNode = found.second;
			}
			else
			{
				t.push_back(edges());
				size_t newNode = t.size() - 1;
				add_edge(t, currentNode, newNode, currentSymbol);
				currentNode = newNode;
			}
		}
	}

	return t;
}

auto find_edge(trie &t, size_t node, char symbol) -> pair<bool, size_t>
{
	if (node < t.size())
	{
		edges e = t.at(node);

		auto itr = e.find(symbol);

		if (itr != e.end())
			return{ true, itr->second };
	}

	return{ false, 0 };
}

auto add_edge(trie & t, size_t node, size_t newNode, char symbol) -> void
{
	if (node < t.size())
	{
		edges &e = t.at(node);
		e.insert({ symbol, newNode });
	}
}

void my_main(std::istream &in, std::ostream &out)
{
	size_t n;
	in >> n;
	vector<string> patterns;
	for (size_t i = 0; i < n; i++)
	{
		string s;
		in >> s;
		patterns.push_back(s);
	}

	trie t = build_trie(patterns);
	for (size_t i = 0; i < t.size(); ++i)
	{
		for (const auto & j : t[i])
		{
			out << i << "->" << j.second << ":" << j.first << "\n";
		}
	}
}

int main()
{
	my_main(std::cin, std::cout);

	//auto Test = [](string input)
	//{
	//	stringstream in, out;
	//	in.str(input);
	//	my_main(in, out);
	//	std::cout << out.str();
	//};

	//Test("1 ATA");
	//Test("3 AT AG AC");
	//Test("3 ATAGA ATC GAT");
	//Test("7 banana pan nab antenna bandana ananas nana");

	return 0;
}