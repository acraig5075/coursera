#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::stringstream;

// burrows-wheeler transform
string bwt(const string& text)
{
	size_t size = text.size();

	vector<string> mat(size, text);

	// store a matrix of cyclic rotations of the text
	for (size_t i = 0; i < size; i++)
	{
		string &value = mat[i];
		std::rotate(value.rbegin(), value.rbegin() + i + 1, value.rend());
	}

	// sort lexicographically
	std::sort(begin(mat), end(mat));

	// extract the final column of characters - this is the burrows-wheeler transform
	string result(size, ' ');
	std::transform(begin(mat), end(mat), begin(result), [c = size - 1](const string &s)
	{
		return s[c];
	});

	return result;
}

void my_main(std::istream &in, std::ostream &out)
{
	string text;
	in >> text;
	out << bwt(text) << "\n";
}

int main()
{
	my_main(std::cin, std::cout);

	//auto Test = [](string input, string expected)
	//{
	//	stringstream in(input);
	//	stringstream out;
	//	my_main(in, out);
	//	string actual = out.str();
	//	assert(actual == expected);
	//};

	//Test("AA$", "AA$\n");
	//Test("ACACACAC$", "CCCC$AAAA\n");
	//Test("AGACATA$", "ATG$CAAA\n");
	//Test("ACA$", "AC$A\n");
	//Test("GAGAGA$", "AGGGAA$\n");

	return 0;
}