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
	struct node
	{
		node(size_t index, bool eos)
			: index(index)
			, eos(eos)
		{}

		size_t index = 0;
		bool eos = false;
	};

	using edges = map<char, node>;
	using trie = vector<edges>;

public:
	void build_trie(const vector<string> & patterns)
	{
		if (!patterns.empty())
			t.push_back(edges());

		for (string pattern : patterns)
		{
			size_t currentNode = 0;

			for (size_t i = 0; i < pattern.size(); ++i)
			{
				char currentSymbol = pattern[i];

				bool end = (i == pattern.size() - 1);

				auto found = find_edge(currentNode, currentSymbol);

				if (found.first)
				{
					if (end)
						flag_end(currentNode, currentSymbol);

					currentNode = found.second.index;
				}
				else
				{
					t.push_back(edges());
					size_t newNode = t.size() - 1;
					add_edge(currentNode, newNode, currentSymbol, end);
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
				out << i << "->" << j.second.index << ":" << j.first << "\n";
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
	auto find_edge(size_t index, char symbol) -> pair<bool, node>
	{
		if (index < t.size())
		{
			edges e = t.at(index);

			auto itr = e.find(symbol);

			if (itr != e.end())
				return{ true, itr->second };
		}

		return{ false, { 0, false } };
	}

	auto add_edge(size_t index, size_t newNode, char symbol, bool end) -> void
	{
		if (index < t.size())
		{
			edges &e = t.at(index);
			e.insert({ symbol, { newNode, end } });
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
					if (found.second.eos)
					{
						return true;
					}
					else
					{
						symbol++;
						v = found.second.index;
					}
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

	bool is_leaf(size_t index)
	{
		if (index < t.size())
			return t.at(index).empty();
		return false;
	}

	void flag_end(size_t index, char symbol)
	{
		if (index < t.size())
		{
			edges &e = t.at(index);

			auto itr = e.find(symbol);

			if (itr != e.end())
				itr->second.eos = true;
		}
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
	//Test("ACATA 3 AT A AG", "0 2 4\n");
	//Test("ALASDAIR 2 ASDA AS", "2\n");
	//Test("ALASDAIR 2 AS ASDA", "2\n");

	return 0;
}