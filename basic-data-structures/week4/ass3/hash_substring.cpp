#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <ctime> 
#include <sstream> 
#include <unordered_map>
#include <cassert>

using std::string;
using std::vector;
using std::unordered_map;
using std::int64_t;


static const int64_t primes[] =
{
	1299721,
	15485867,
	179424691,
	1500450271
};

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

void print_occurrences(std::ostream &os, const vector<size_t> &output)
{
	for (const auto i : output)
	{
		os << i << " ";
	}
	os << "\n";
}

int64_t mod(int64_t a, int64_t p)
{
	return ((a % p) + p) % p;
}

int64_t random(int64_t low, int64_t high)
{
	srand((unsigned)time(0));

	return low + (int64_t)(rand() * ((high - low) / RAND_MAX));
}

int64_t poly_hash(const string &S, size_t begin, size_t end, int64_t p, int64_t x)
{
	int64_t hash = 0;
	for (size_t i = end; i > begin; --i)
	{
		hash = mod(hash * x + S[i - 1], p);
	}
	return hash;
}

int64_t poly_hash(const string &S, int64_t p, int64_t x)
{
	return poly_hash(S, 0, S.length(), p, x);
}

bool are_equal(const string &pattern, const string &text, size_t off)
{
	const char *p = pattern.c_str();
	const char *t = text.c_str() + off;

	while (*p != '\0')
	{
		if (*p != *t)
			return false;
		++p;
		++t;
	}

	return true;
}

unordered_map<size_t, int64_t> precompute_hashes(const string &T, size_t L, int64_t p, int64_t x)
{
	size_t TL = T.length();

	unordered_map<size_t, int64_t> H;
	H.reserve(TL - L + 1);

	string S = T.substr(TL - L);
	H[TL - L] = poly_hash(S, p, x);

	int64_t y = 1;
	for (size_t i = 1; i <= L; ++i)
		{
		y = mod(y * x, p);
		}

	for (size_t j = TL - L; j > 0; --j)
		{
		size_t i = j - 1;
		H[i] = mod(x * H[i + 1] + T[i] - y * T[i + L], p);
		}

	return H;
}

vector<size_t> rabin_karp(const Data &input)
{
	vector<size_t> ans;

	const string &P = input.pattern;
	const string &T = input.text;
	const size_t L = P.length();
	const size_t TL = T.length();

	if (L <= TL)
	{
		int64_t p = primes[2];
		int64_t x = random(1, p - 1);

		int64_t pHash = poly_hash(P, p, x);

		unordered_map<size_t, int64_t> H = precompute_hashes(T, L, p, x);

		for (size_t i = 0; i <= TL - L; ++i)
		{
			int64_t tHash = H[i];

			if (pHash == tHash)
			{
				if (are_equal(P, T, i))
					ans.push_back(i);
			}
		}
	}

	return ans;
}

vector<size_t> get_occurrences(const Data &input)
{
	return rabin_karp(input);
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	print_occurrences(std::cout, get_occurrences(read_input()));

	//auto Test = [&](const string &text, const string &pattern, const string &expected)
	//{
	//	Data d;
	//	d.text = text;
	//	d.pattern = pattern;
	//	std::stringstream ss;
	//	print_occurrences(ss, get_occurrences(d));
	//	assert(expected == ss.str());
	//};

	//Test("alasdair", "ala", "0 \n");
	//Test("alasdair", "air", "5 \n");
	//Test("alasdair", "a", "0 2 5 \n");
	//Test("abacaba", "aba", "0 4 \n");
	//Test("testTesttesT", "Test", "4 \n");
	//Test("baaaaaaa", "aaaaa", "1 2 3 \n");
	//Test("hello", "lo", "3 \n");
	//Test("lo", "hello", "\n");

	//string example = "the quick brown fox jumps over the lazy dog. jackdaws love my big sphinx of quartz.";

	//Test(example, "the", "0 31 \n");
	//Test(example, ".", "43 82 \n");
	//Test(example, "jackdaws love my big sphinx of quartz.", "45 \n");
	//Test(example, " ", "3 9 15 19 25 30 34 39 44 53 58 61 65 72 75 \n");
	//Test(example, "brown fox jumps over the lazy dog. jackdaws love my big sph", "10 \n");

	//string example2 = "In this programming assignment, you will practice implementing hash functions and hash tables and using them to solve algorithmic problems. In some cases you will just implement an algorithm from the lectures, while in others you will need to invent an algorithm to solve the given problem using hashing.";
	//Test(example2, "you ", "32 154 226 \n");
	//Test(example2, "In some cases you will just implement an algorithm from the lectures, while in others you will need to invent an algorithm to solve the given problem using hashing.", "140 \n");

	//string lorem = "Lorem ipsum dolor sit amet, eu pri fuisset singulis, cibo veniam labores te vis. Vix laudem graecis an, eam tale iuvaret molestie ad. Error blandit repudiare sea ex, eos persius reformidans concludaturque id. Per nostrud saperet concludaturque eu. At sea sint omnium, mel vitae vivendo temporibus ne, dicant nonumes in eos. Ignota noluisse vix in, an tamquam appareat definiebas sea. Prompta facilis pri ne, cum ei cibo vivendum, te mundi recteque sed. Per ad dicit essent nostrud. Per ad vidit oportere prodesset, quo cu consul partem salutandi, vide mollis at mel. Ex mea mutat dicat. Duo ea iudicabit maluisset temporibus, augue verear nominati no vix, sed eu nostro impetus. Vim animal equidem eu, sed amet torquatos cu. Stet denique consulatu his ex. Sea id possim nostrum percipit, dicam impetus verterem cum ea. An vidisse utroque eum, option dolorem volutpat at quo. Ea zril fierent contentiones duo. Et agam accusamus vim. Pri in noster omittam, wisi causae percipit his ei. Id antiopam scriptorem est, eam ne quem vidit assum. Ad mei virtute placerat theophrastus, duo tempor inermis at. Eu error aeque epicuri duo, ei eam inermis hendrerit persecuti, ea laoreet nominati sea. Pri in platonem constituto, qui quaeque intellegat intellegebat id. Has suas error insolens in. Et sea etiam epicuri ancillae. At ius quod philosophia. Has salutandi interesset et, id sed adhuc debet, est fabulas nostrum explicari ei. Eos in ipsum accusam, qui cu iudico electram reformidans.";

	//Data d;
	//d.text = lorem;
	//d.pattern = "reformidans";
	//print_occurrences(std::cout, get_occurrences(d));

	return 0;
}
