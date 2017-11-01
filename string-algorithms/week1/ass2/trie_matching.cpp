#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cassert>

using std::map;
using std::vector;
using std::string;
using std::pair;
using std::stringstream;


class Trie
{
	using edges = map<char, size_t>;
	using trie = vector<edges>;

public:
	void build_trie(const vector<string> & patterns)
	{
		if (!patterns.empty())
			t.push_back(edges());

		for (string pattern : patterns)
		{
			size_t currentNode = 0;

			for (char currentSymbol : pattern)
			{
				auto found = find_edge(currentNode, currentSymbol);

				if (found.first)
				{
					currentNode = found.second;
				}
				else
				{
					t.push_back(edges());
					size_t newNode = t.size() - 1;
					add_edge(currentNode, newNode, currentSymbol);
					currentNode = newNode;
				}
			}
		}
	}

	void print(std::ostream &out)
	{
		for (size_t i = 0; i < t.size(); ++i)
		{
			for (const auto & j : t[i])
			{
				out << i << "->" << j.second << ":" << j.first << "\n";
			}
		}
	}

	vector<size_t> trie_matching(const string &text)
	{
		vector<size_t> ans;

		for (size_t k = 0; k < text.size(); ++k)
		{
			if (prefix_trie_matching(text.substr(k)))
			{
				ans.push_back(k);
			}
		}

		return ans;
	}

private:
	auto find_edge(size_t node, char symbol) -> pair<bool, size_t>
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

	auto add_edge(size_t node, size_t newNode, char symbol) -> void
	{
		if (node < t.size())
		{
			edges &e = t.at(node);
			e.insert({ symbol, newNode });
		}
	}

	bool prefix_trie_matching(const string &text)
	{
		const char *symbol = text.c_str();

		size_t v = 0;

		while (true)
		{
			if (is_leaf(v))
			{
				return true;
			}
			else
			{
				auto found = find_edge(v, *symbol);

				if (found.first)
				{
					symbol++;
					v = found.second;
				}
				else
				{
					return false;
				}
			}
		}

		assert(false);
		return false;
	}

	bool is_leaf(size_t node)
	{
		if (node < t.size())
			return t.at(node).empty();
		return false;
	}

	trie t;
};

void output_with_spaces(const vector <size_t> &ans, std::ostream &out)
{
	for (size_t i = 0; i < ans.size(); i++)
	{
		out << ans[i];

		if (i + 1 < ans.size())
			out << " ";
		else
			out << "\n";
	}
}

void my_main(std::istream &in, std::ostream &out)
{
	string text;
	in >> text;

	int n;
	in >> n;

	vector <string> patterns(n);
	for (int i = 0; i < n; i++)
	{
		in >> patterns[i];
	}

	Trie t;
	t.build_trie(patterns);

	vector <size_t> ans;
	ans = t.trie_matching(text);

	output_with_spaces(ans, out);
}

int main()
{
	my_main(std::cin, std::cout);

	//auto Test = [](string input, string expected)
	//{
	//	stringstream in, out;
	//	in.str(input);
	//	my_main(in, out);
	//	string actual = out.str();
	//	assert(actual == expected);
	//};

	//Test("AAA 1 AA", "0 1\n");
	//Test("AA 1 T", "");
	//Test("AATCGGGTTCAATCGGGGT 2 ATCG GGGT", "1 4 11 15\n");

	return 0;
}