#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;


void explore(int v, const vector<vector<int>> &adj, vector<bool> &visited)
{
	visited[v - 1] = true;

	for (int w : adj[v - 1])
	{
		if (!visited[w - 1])
			explore(w, adj, visited);
	}
}

int number_of_components(vector<vector<int> > &adj)
{
	vector<bool> visited(adj.size(), false);

	int cc = 0;

	for (int v = 1; v <= (int)adj.size(); ++v)
	{
		if (!visited[v - 1])
		{
			explore(v, adj, visited);
			++cc;
		}
	}

	return cc;
}

void my_main(std::istream &in, std::ostream &out)
{
	size_t n, m;
	in >> n >> m;
	vector<vector<int> > adj(n, vector<int>());

	for (size_t i = 0; i < m; i++)
		{
		int x, y;
		in >> x >> y;
		adj[x - 1].push_back(y);
		adj[y - 1].push_back(x);
		}

	out << number_of_components(adj);
}

int main()
{
	my_main(std::cin, std::cout);

	//auto Test = [](string input, string expected)
	//{
	//	stringstream in(input);
	//	stringstream out;
	//	my_main(in, out);
	//	assert(out.str() == expected);
	//};

	//Test("4 4 1 2 3 2 4 3 1 4 ", "1");
	//Test("4 2 1 2 3 2 ", "2");
	//Test("10 11 1 3 1 4 1 6 2 5 3 8 3 9 5 2 5 7 5 10 6 9 8 9 ", "2");
	//Test("9 8 1 2 1 3 1 4 2 3 5 6 7 8 7 9 8 9 ", "3");

	return 0;
}
