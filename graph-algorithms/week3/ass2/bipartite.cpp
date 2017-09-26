#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <queue>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;
using std::queue;

#define infinity -1

struct node
{
	bool visited = false;
	int previsit = 0;
	int postvisit = 0;
	int id = 0;
};

class graph
{
public:
	graph(size_t v, size_t e)
		: n(v)
		, m(e)
	{
		adjacency_list.resize(v, vector<int>());
		node_list.resize(v);
		dist.resize(v, infinity);
	}

	void read_directed(std::istream &in)
	{
		for (size_t i = 0; i < m; i++)
		{
			int x, y;
			in >> x >> y;
			x--;
			y--;

			adjacency_list[x].push_back(y);
		}
	}

	void read_undirected(std::istream &in)
	{
		for (size_t i = 0; i < m; i++)
		{
			int x, y;
			in >> x >> y;
			x--;
			y--;

			adjacency_list[x].push_back(y);
			adjacency_list[y].push_back(x);
		}
	}

	void unvisit_all()
	{
		for (auto &n : node_list)
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
		node &n = node_list.at(v);

		visited(n) = true;
		previsit(n);

		for (auto w : adjacency_list[v])
		{
			if (!visited(node_list[w]))
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
			if (!visited(node_list[v]))
				explore(v);
		}
	}

	int post(size_t v) const
	{
		return node_list[v].postvisit;
	}

	// week 2, assignment 1
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

	void numerate_nodes()
	{
		int id = 0;
		for (auto &n : node_list)
		{
			n.id = id++;
		}
	}

	// week 2, assignment 2
	vector<int> topological_sort()
	{
		dfs();

		numerate_nodes();

		// work with a copy of the nodes
		vector<node> copy = node_list;

		// sort vertices by reverse post-order
		sort(begin(copy), end(copy), [](const node &a, const node &b)
		{
			return a.postvisit > b.postvisit;
		});

		// extract the ordered id's and return
		vector<int> order;
		transform(begin(copy), end(copy), std::back_inserter(order), [](const node &n)
		{
			return n.id;
		});

		return order;
	}

	// week 3, assignment 1
	void bfs(int s)
	{
		dist.resize(n, infinity);
		dist[s] = 0;

		queue<int> q;
		q.push(s);

		while (!q.empty())
		{
			int u = q.front();
			q.pop();

			for (auto v : adjacency_list[u])
			{
				if (dist[v] == infinity)
				{
					q.push(v);
					dist[v] = dist[u] + 1;
				}
			}
		}
	}

	int get_distance(int v)
	{
		return dist[v];
	}

	// week 3, assignment 2
	bool bipartite(int s)
	{
		enum Colour { None, Black, White };

		vector<Colour> colours(n, None);

		colours[s] = Black;

		queue<int> q;
		q.push(s);

		while (!q.empty())
		{
			int u = q.front();
			q.pop();

			for (auto v : adjacency_list[u])
			{
				if (colours[v] == None)
				{
					q.push(v);
					if (colours[u] == Black)
						colours[v] = White;
					else
						colours[v] = Black;
				}
				else if (colours[u] == colours[v])
				{
					return false;
				}
			}
		}

		return true;
	}

private:
	size_t n = 0;
	size_t m = 0;
	vector<vector<int>> adjacency_list;
	vector<node> node_list;
	vector<int> dist;
	int clock_tick = 0;
};

void my_main(std::istream &in, std::ostream &out)
{
	size_t n, m;
	in >> n >> m;

	graph g(n, m);

	g.read_undirected(in);

	if (g.bipartite(1))
		out << "1";
	else
		out << "0";
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

	//Test("4 4 1 2 4 1 2 3 3 1 ", "0");
	//Test("5 4 5 2 4 2 3 4 1 4 ", "1");
	//Test("6 9 1 2 1 4 1 6 2 3 2 5 3 4 3 6 4 5 5 ", "1");
	//Test("6 4 1 2 2 3 3 4 5 ", "1");
	//Test("4 4 1 2 2 3 3 4 4 1 ", "1");
	//Test("5 6 1 2 2 3 3 4 4 1 4 5 5 3 ", "0");
	//Test("5 5 1 2 2 3 3 4 4 5 5 1 ", "0");

	return 0;
}
