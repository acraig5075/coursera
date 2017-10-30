#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;

using edges = map<char, int>;
using trie = vector<edges>;

trie build_trie(vector<string> & patterns)
{
	trie t;

	for (const auto &pattern : patterns)
	{
		for (size_t i = 0; i < pattern.size() - 1; ++i)
		{
			char currentSymbol = pattern[i];
		}
	}

	return t;
}

int main()
{
	size_t n;
	std::cin >> n;
	vector<string> patterns;
	for (size_t i = 0; i < n; i++)
	{
		string s;
		std::cin >> s;
		patterns.push_back(s);
	}

	trie t = build_trie(patterns);
	for (size_t i = 0; i < t.size(); ++i)
	{
		for (const auto & j : t[i])
		{
			std::cout << i << "->" << j.second << ":" << j.first << "\n";
		}
	}

	return 0;
}