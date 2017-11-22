#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <utility>
#include <sstream>
#include <cassert>

using std::make_pair;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

static const size_t ALPHABET = 91; // accomodates ascii character codes of all upper case letters

vector<size_t> SortCharacters(const string &s);
vector<size_t> ComputeCharClasses(const string &s, const vector<size_t> &order);
vector<size_t> SortDoubled(const string &s, size_t l, const vector<size_t> &order, const vector<size_t> &classes);
vector<size_t> UpdateClasses(const vector<size_t> &newOrder, const vector<size_t> &classes, size_t l);
std::pair<size_t, size_t> PatternMatching(const string &pattern, const string &text, const vector<size_t> &suffix);


// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<size_t> BuildSuffixArray(const string& text)
{
	auto order = SortCharacters(text);
	auto classes = ComputeCharClasses(text, order);

	size_t l = 1;

	while (l < text.size())
	{
		order = SortDoubled(text, l, order, classes);
		classes = UpdateClasses(order, classes, l);
		l *= 2;
	}

	return order;
}

// Find the indexes in text where pattern starts. This uses the suffix array determined earlier.
vector<size_t> FindOccurrences(const string &pattern, const string &text, const vector<size_t> &suffix)
{
	auto range = PatternMatching(pattern, text, suffix);

	vector<size_t> occurences;

	for (size_t i = range.first; i < range.second && i < suffix.size(); ++i)
		occurences.push_back(suffix[i]);

	return occurences;
}

std::pair<size_t, size_t> PatternMatching(const string &pattern, const string &text, const vector<size_t> &suffixArray)
{
	size_t minIndex = 0;
	size_t maxIndex = text.size();

	while (minIndex < maxIndex)
	{
		auto mid = (minIndex + maxIndex) / 2;
		int res = strncmp(pattern.c_str(), &(text[suffixArray[mid]]), pattern.size());

		if (res > 0)
			minIndex = mid + 1;
		else
			maxIndex = mid;
	}

	auto start = minIndex;
	maxIndex = text.size();

	while (minIndex < maxIndex)
	{
		auto mid = (minIndex + maxIndex) / 2;
		int res = strncmp(pattern.c_str(), &(text[suffixArray[mid]]), pattern.size());

		if (res < 0)
			maxIndex = mid;
		else
			minIndex = mid + 1;
	}

	auto end = maxIndex;
	
	return make_pair(start, end);
}

vector<size_t> SortCharacters(const string &s)
{
	vector<size_t> order(s.size());
	vector<size_t> count(ALPHABET, 0);

	for (size_t i = 0; i < s.size(); ++i)
		count[s[i]]++;

	for (size_t j = 1; j < ALPHABET; ++j)
		count[j] += count[j - 1];

	for (size_t i = s.size() - 1; i >= 0;)
	{
		auto c = s[i];
		count[c]--;
		order[count[c]] = i;

		if (i == 0)
			break;
		else
			--i;
	}

	return order;
}

vector<size_t> ComputeCharClasses(const string &s, const vector<size_t> &order)
{
	vector<size_t> classes(s.size(), 0);

	for (size_t i = 1; i < s.size(); ++i)
	{
		if (s[order[i]] != s[order[i - 1]])
			classes[order[i]] = classes[order[i - 1]] + 1;
		else
			classes[order[i]] = classes[order[i - 1]];
	}

	return classes;
}

vector<size_t> SortDoubled(const string &s, size_t l, const vector<size_t> &order, const vector<size_t> &classes)
{
	size_t ssize = s.size();

	vector<size_t> count(ssize, 0);
	vector<size_t> newOrder(ssize);

	for (size_t i = 0; i < ssize; ++i)
		count[classes[i]]++;

	for (size_t j = 1; j < ssize; ++j)
		count[j] += count[j - 1];

	for (size_t i = ssize - 1; i >= 0;)
	{
		auto start = (order[i] - l + ssize) % ssize;
		auto cl = classes[start];
		count[cl]--;
		newOrder[count[cl]] = start;

		if (i == 0)
			break;
		else
			--i;
	}

	return newOrder;
}

vector<size_t> UpdateClasses(const vector<size_t> &newOrder, const vector<size_t> &classes, size_t l)
{
	size_t n = newOrder.size();
	vector<size_t> newClass(n, 0);

	for (size_t i = 1; i < n; ++i)
	{
		auto cur = newOrder[i];
		auto prev = newOrder[i - 1];
		auto mid = cur + l;
		auto midPrev = (prev + l) % n;

		if (classes[cur] != classes[prev] || classes[mid] != classes[midPrev])
			newClass[cur] = newClass[prev] + 1;
		else
			newClass[cur] = newClass[prev];
	}

	return newClass;
}

void my_main(std::istream &in, std::ostream &out)
{
	string text;
	in >> text;
	text += "$";

	size_t n;
	in >> n;

	vector<string> patterns(n);

	for (size_t i = 0; i < n; ++i)
		in >> patterns[i];

	vector<size_t> suffix_array = BuildSuffixArray(text);

	vector<bool> occurs(text.size(), false);

	for (auto pattern : patterns)
	{
		vector<size_t> occurrences = FindOccurrences(pattern, text, suffix_array);

		for (size_t j = 0; j < occurrences.size(); ++j)
			occurs[occurrences[j]] = true;
	}

	for (size_t i = 0; i < occurs.size(); ++i) 
	{
		if (occurs[i])
			out << i << " ";
	}
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

	//Test("ATATATA 3 ATA C TATAT", "0 1 2 4 \n");
	//Test("AAA 1 A", "0 1 2 \n");
	//Test("ATA 3 C G C", "\n");
	//Test("AAA 2 A AA", "0 1 2 \n");
	//Test("AAA 1 AA", "0 1 \n");
	//Test("ACGTACGTACGTACGTACGT 1 A", "0 4 8 12 16 \n");
	//Test("ACGTACGTACGTACGTACGT 1 AC", "0 4 8 12 16 \n");
	//Test("ACGTACGTACGTACGTACGT 1 ACG", "0 4 8 12 16 \n");
	//Test("ACGTACGTACGTACGTACGT 1 ACGT", "0 4 8 12 16 \n");
	//Test("ACGTACGTACGTACGTACGT 1 ACGTA", "0 4 8 12 \n");
	//Test("ACGTACGTACGTACGTACGT 1 ACGTAC", "0 4 8 12 \n");
	//Test("ACGTACGTACGTACGTACGT 1 ACGTACG", "0 4 8 12 \n");
	//Test("ACGTACGTACGTACGTACGT 1 ACGTACGT", "0 4 8 12 \n");
	//Test("ACGTACGTACGTACGTACGT 1 GC", "\n");
	//Test("PANAMABANANA 1 ANA", "1 7 9 \n");

	return 0;
}