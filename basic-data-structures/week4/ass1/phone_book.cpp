#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;
using std::cin;

struct Query
{
	string type;
	string name;
	int number = 0;
};

vector<Query> read_queries()
{
	int n;
	cin >> n;
	vector<Query> queries(n);
	for (int i = 0; i < n; ++i)
		{
		cin >> queries[i].type;
		if (queries[i].type == "add")
			cin >> queries[i].number >> queries[i].name;
		else
			cin >> queries[i].number;
		}
	return queries;
}

void write_responses(const vector<string> &result)
{
	for (size_t i = 0; i < result.size(); ++i)
		std::cout << result[i] << "\n";
}

vector<string> process_queries(const vector<Query> &queries)
{
	vector<string> result;
	unordered_map<int, string> book;
	book.reserve(result.size());

	for (const auto &q : queries)
		{
		if (q.type == "add")
			{
			book[q.number] = q.name;
			}
		else if (q.type == "del")
			{
			book.erase(q.number);
			}
		else if (q.type == "find")
			{
			auto itr = book.find(q.number);
			if (itr == book.end())
				{
				result.push_back("not found");
				}
			else
				{
				result.push_back(itr->second);
				}
			}
		}

	return result;
}

int main()
{
	write_responses(process_queries(read_queries()));
	return 0;
}
