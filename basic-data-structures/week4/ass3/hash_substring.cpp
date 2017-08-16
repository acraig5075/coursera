#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <array>

using std::string;
using std::uint64_t;
using std::pow;

uint64_t powers[] =
{
	1,
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000,
	1000000000,
	10000000000,
	100000000000,
	1000000000000,
	10000000000000,
	100000000000000,
	1000000000000000,
	10000000000000000,
	100000000000000000,
	1000000000000000000
};

static const uint64_t prime = 1301081;


struct Data
{
	string pattern, text;
};

Data read_input()
{
	Data data;
	std::cin >> data.pattern >> data.text;
	return data;
}

void print_occurrences(const std::vector<int> &output)
{
	for (size_t i = 0; i < output.size(); ++i)
		std::cout << output[i] << " ";
	std::cout << "\n";
}

uint64_t hash(const string &k, size_t L, uint64_t m)
{
	uint64_t h = 0;
	uint64_t base = 10;

	for (size_t l = 0; l < L; ++l)
		{
		h += uint64_t(k[l]) * powers[L - l - 1];
		}

	return h % m;
}

uint64_t next_hash(uint64_t hash, size_t i, const string &s, size_t L, uint64_t m)
{
	uint64_t base = 10;

	return (base * (hash - powers[L - 1] * uint64_t(s[i])) + uint64_t(s[i + L])) % m;
}

std::vector<int> get_occurrences(const Data &input)
{
	const string &p = input.pattern;
	const string &s = input.text;

	std::vector<int> ans;

	return ans;
}


int main()
{
	std::ios_base::sync_with_stdio(false);
	//print_occurrences(get_occurrences(read_input()));

	string a1 = "alasdair";
	string a2 = "ala";

	uint64_t h3 = hash("ala", 3, prime);
	uint64_t h4 = hash("las", 3, prime);
	uint64_t h5 = hash("asd", 3, prime);
	uint64_t h6 = hash("sda", 3, prime);
	uint64_t h7 = hash("dai", 3, prime);
	uint64_t h8 = hash("air", 3, prime);

	uint64_t h1 = hash(a2, a2.length(), prime);

	uint64_t h2 = hash(a1, a2.length(), prime);

	for (size_t i = 1; i < a1.length() - a2.length() + 1; ++i)
		{
		h2 = next_hash(h2, i, a1, a2.length(), prime);
		}

	return 0;
}
