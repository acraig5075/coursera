#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

using std::string;
using std::vector;
using std::stringstream;

vector<int> compute_prefix_function(const string &p)
{
	vector<int> s(p.size(), 0);
	s[0] = 0;
	int border = 0;

	for (size_t i = 1; i <= p.size() - 1; ++i)
	{
		while (border > 0 && p[i] != p[border])
			border = s[border - 1];

		if (p[i] == p[border])
			border++;
		else
			border = 0;

		s[i] = border;
	}

	return s;
}

// Knuth-Morris-Pratt algorithm.
// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where 
// the pattern starts in the text.
vector<size_t> find_all_occurences(const string& pattern, const string& text)
{
	const string s = pattern + string("$") + text;

	vector<int> prefix = compute_prefix_function(s);

	vector<size_t> result;

	size_t psize = pattern.size();

	for (size_t i = psize + 1; i <= s.size() - 1; ++i) // TODO is <= correct?
	{
		if (prefix[i] == psize)
			result.push_back(i - 2 * psize);
	}

	return result;
}

void my_main(std::istream &in, std::ostream &out)
{
	string pattern, text;
	in >> pattern;
	in >> text;

	vector<size_t> result = find_all_occurences(pattern, text);

	for (size_t i = 0; i < result.size(); ++i)
		out << result[i] << " ";
	out << "\n";
}

int main()
{
	my_main(std::cin, std::cout);

	//auto Test = [](const string &input, const string &expected)
	//{
	//	stringstream in(input);
	//	stringstream out;
	//	my_main(in, out);
	//	string actual = out.str();
	//	assert(actual == expected);
	//};

	//Test("TACG GT", "\n");
	//Test("ATA ATATA", "0 2 \n");
	//Test("ATAT GATATATGCATATACTT", "1 3 9 \n");
	//Test("abra abracadabra", "0 7 \n");

	return 0;
}