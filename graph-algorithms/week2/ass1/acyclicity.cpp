#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;

struct node
{
	bool visited = false;
	int previsit = 0;
	int postvisit = 0;
	int component = 0;
};

class graph
{
public:
	graph(size_t v, size_t e)
		: n(v)
		, m(e)
	{
		adjacency_list.resize(v, vector<int>());
		nodes.resize(v);
	}

	void read(std::istream &in)
	{
		for (size_t i = 0; i < m; i++)
		{
			int x, y;
			in >> x >> y;
			adjacency_list[x - 1].push_back(y - 1);
		}
	}

	void unvisit_all()
	{
		for (auto &n : nodes)
			n.visited = false;
	}

	bool &visited(node &v)
	{
		return v.visited;
	}

	void previsit(node &v)
	{
		v.previsit = clock_tick;
		clock_tick++;
	}

	void postvisit(node &v)
	{
		v.postvisit = clock_tick;
		clock_tick++;
	}

	void explore(size_t v)
	{
		node &n = nodes.at(v);

		visited(n) = true;
		previsit(n);

		for (auto w : adjacency_list[v])
		{
			if (!visited(nodes[w]))
				explore(w);
		}
		
		postvisit(n);
	}

	void dfs()
	{
		unvisit_all();

		clock_tick = 1;

		for (size_t v = 0; v < adjacency_list.size(); ++v)
		{
			if (!visited(nodes[v]))
				explore(v);
		}
	}

	int post(size_t v) const
	{
		return nodes[v].postvisit;
	}

	void print_nodes()
	{
		int i = 1;
		for (const auto &n : nodes)
		{
			std::cout << i << " " << n.previsit << " " << n.postvisit << "\n";
			++i;
		}

		std::cout << "\n";
	}

	bool is_acyclic()
	{
		dfs();

		for (size_t u = 0; u < adjacency_list.size(); ++u)
		{
			int postu = post(u);

			for (const auto &v : adjacency_list[u])
			{
				if (postu <= post(v))
					return false; // not a DAG
			}
		}

		return true; // every edge u->v has post(u) > post(v)
	}

private:
	size_t n = 0;
	size_t m = 0;
	vector<vector<int>> adjacency_list;
	vector<node> nodes;
	int clock_tick = 0;
};

void my_main(std::istream &in, std::ostream &out)
{
	size_t n, m;
	in >> n >> m;

	graph g(n, m);

	g.read(in);

	if (g.is_acyclic())
		out << "0";
	else
		out << "1";
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

	//Test("9 8 1 2 2 3 3 1 1 4 5 6 7 8 8 9 9 7", "1");
	//Test("4 4 1 2 4 1 2 3 3 1 ", "1");
	//Test("5 7 1 2 2 3 1 3 3 4 1 4 2 5 3 5 ", "0");
	//Test("5 8 4 3 1 2 3 1 3 4 2 5 5 1 5 4 5 3", "1");
	//Test("5 8 4 3 1 2 3 1 3 4 5 2 5 1 5 4 5 3", "1");
	//Test("5 1 4 3 ", "0");
	//Test("4 3 1 2 4 1 3 1 ", "0");
	//Test("4 1 3 1", "0");
	//Test("4 4 1 2 2 3 1 4 4 3 ", "0");
	//Test("4 5 1 2 2 3 1 4 4 3 3 4", "1");
	//Test("4 6 1 2 2 3 1 4 4 3 3 4 4 1", "1");

	return 0;
}
