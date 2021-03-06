#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <queue>
#include <functional>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;
using std::queue;
using std::priority_queue;

#define infinity 1000000000

struct node
{
	bool visited = false;
	int previsit = 0;
	int postvisit = 0;
	int id = 0;
};

struct edge
{
	int to = 0;
	int weight = 0;

	edge(int t, int w = 0)
		: to(t)
		, weight(w)
	{}
};

class graph
{
public:
	graph(size_t v, size_t e)
		: n(v)
		, m(e)
	{
		adjacency_list.resize(v, vector<edge>());
		node_list.resize(v);
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

	void read_directed_weighted(std::istream &in)
	{
		for (size_t i = 0; i < m; i++)
		{
			int x, y;
			long long w;
			in >> x >> y >> w;
			x--;
			y--;

			if (w > 1000000)
				w = 1000000;
			if (w < -1000000)
				w = -1000000;

			adjacency_list[x].push_back({ y, static_cast<int>(w) });
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
			if (!visited(node_list[w.to]))
				explore(w.to);
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
				if (postu <= post(v.to))
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
				if (dist[v.to] == infinity)
				{
					q.push(v.to);
					dist[v.to] = dist[u] + 1;
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
				if (colours[v.to] == None)
				{
					q.push(v.to);
					if (colours[u] == Black)
						colours[v.to] = White;
					else
						colours[v.to] = Black;
				}
				else if (colours[u] == colours[v.to])
				{
					return false;
				}
			}
		}

		return true;
	}

	struct dijkstr_type
	{
		int node;
		int dist;

		dijkstr_type(int n, int w)
			: node(n)
			, dist(w)
		{}

		bool operator < (const dijkstr_type &rhs) const
		{
			return dist < rhs.dist;
		}
		bool operator > (const dijkstr_type &rhs) const
		{
			return dist > rhs.dist;
		}
	};

	// week 4, assignment 1
	// shortest distance where weights are not negative
	void dijkstra(int s)
	{
		dist.resize(n, infinity);
		prev.resize(n, -1);

		dist[s] = 0;

		priority_queue<dijkstr_type, vector<dijkstr_type>, std::greater<dijkstr_type> > h;

		h.push({ s, dist[s] });

		while (!h.empty())
		{
			int u = h.top().node;
			h.pop();

			for (auto edge : adjacency_list[u])
			{
				assert(dist[u] != infinity);

				if (dist[edge.to] > dist[u] + edge.weight)
				{
					dist[edge.to] = dist[u] + edge.weight;
					prev[edge.to] = u;
					h.push({ edge.to, dist[edge.to] });
				}
			}
		}
	}

	// shortest distance where weights can be negative
	void bellman_ford(int s)
	{
		dist.resize(n, infinity);
		prev.resize(n, -1);

		dist[s] = 0;

		for (size_t i = 0; i < n - 1; ++i)
		{
			for (size_t u = 0; u < n; ++u)
			{
				for (auto v : adjacency_list[u])
				{
					if (dist[v.to] > dist[u] + v.weight)
					{
						dist[v.to] = dist[u] + v.weight;
						prev[v.to] = u;
					}
				}
			}
		}
	}

	// week 4, assignment 2
	bool has_negative_cycle()
	{
		bellman_ford(1);

		// whether one more iteration of bellman-ford makes a change
		bool change = false;

		for (size_t u = 0; u < n; ++u)
		{
			for (auto v : adjacency_list[u])
			{
				if (dist[v.to] > dist[u] + v.weight)
				{
					dist[v.to] = dist[u] + v.weight;
					change = true;
				}
			}
		}

		return change;
	}

	// week 4, assignment 3
	vector<int> bellman_ford_final()
	{
		vector<int> changes;

		for (size_t u = 0; u < n; ++u)
		{
			for (auto v : adjacency_list[u])
			{
				if (dist[v.to] > dist[u] + v.weight)
				{
					changes.push_back(v.to);
				}
			}
		}

		// easier this way than std::set
		sort(begin(changes), end(changes));
		changes.erase(unique(begin(changes), end(changes)), end(changes));

		return changes;
	}

	// adaptation of bfs for a number of starting nodes
	vector<int> bfs(const vector<int> &set, int start)
	{
		if (set.empty())
			return vector<int>();

		vector<int> ldist(n, infinity);

		// ignore nodes that can't be reached from the start
		queue<int> q;
		for (auto n : set)
		{
			if (reachable(start, n))
				q.push(n);
		}

		// find all nodes reachable from the set
		while (!q.empty())
		{
			int u = q.front();
			q.pop();

			if (ldist[u] == infinity)
				ldist[u] = 0;

			for (auto v : adjacency_list[u])
			{
				if (ldist[v.to] == infinity)
				{
					q.push(v.to);
					ldist[v.to] = ldist[u] + 1;
				}
			}
		}

		// ldist values that are not infinity are reachable from the set

		vector<int> nodes;
		for (int u = 0; u < (int)ldist.size(); ++u)
		{
			if (ldist[u] != infinity)
				nodes.push_back(u);
		}
		return nodes;
	}

	// is there a path from node x to node y.
	bool reachable(int x, int y)
	{
		unvisit_all();

		explore(x);

		return visited(node_list[y]);
	}

private:
	size_t n = 0;
	size_t m = 0;
	vector<vector<edge>> adjacency_list;
	vector<node> node_list;
	vector<int> dist;
	vector<int> prev;
	int clock_tick = 0;
};

void my_main(std::istream &in, std::ostream &out)
{
	size_t n, m;
	in >> n >> m;

	graph g(n, m);

	g.read_directed_weighted(in);

	int a;
	in >> a;
	a--;

	g.bellman_ford(a);

	vector<int> nodes;

	// get nodes belonging to negative cycles
	nodes = g.bellman_ford_final();

	// get nodes reachable from the negative cycles
	nodes = g.bfs(nodes, a);

	// output
	for (int v = 0; v < (int)n; ++v)
	{
		if (!g.reachable(a, v))
			out << "*\n";
		else if (find(begin(nodes), end(nodes), v) != end(nodes))
			out << "-\n";
		else
			out << g.get_distance(v) << "\n";
	}

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
	//	replace(begin(actual), end(actual), '\n', ' ');
	//	assert(actual == expected);
	//};

	//Test("6 7 1 2 10 2 3 5 1 3 100 3 5 7 5 4 10 4 3 -18 6 1 -1 1 ", "0 10 - - - * ");
	//Test("5 4 1 2 1 4 1 2 2 3 2 3 1 -5 4 ", "- - - 0 * ");
	//Test("4 4 1 2 1 1 3 5 2 3 2 4 1 2 4 ", "2 3 5 0 ");
	//Test("5 7 1 2 -2 2 3 -3 2 4 1 3 1 4 3 4 2 5 1 4 5 2 3 5", "- - - - 0 ");
	//Test("5 7 1 2 -2 2 3 -3 2 4 1 1 3 4 3 4 2 5 1 4 5 2 3 5", "4 2 -1 1 0 ");
	//Test("3 3 1 2 1 2 3 1 3 1 1 1", "0 1 2 ");
	//Test("6 6 1 2 1 2 3 1 3 1 1 4 5 1 5 6 1 6 4 1 1 ", "0 1 2 * * * ");
	//Test("1 0 1", "0 ");
	//Test("2 0 1", "0 * ");
	//Test("2 0 2", "* 0 ");
	//Test("4 4 1 2 1 2 3 1 3 1 -3 3 4 5 1 ", "- - - - ");
	//Test("4 5 1 2 1 2 3 1 3 1 -3 3 4 1 4 3 1 4 ", "- - - - ");
	//Test("9 10 4 5 1 4 1 2 1 2 1 2 3 2 3 1 -5 4 6 2 6 7 1 7 8 2 8 6 -5 8 9 1 4 ", "- - - 0 1 - - - - ");
	//Test("10 11 6 1 -1 1 2 10 1 3 100 2 3 5 3 5 7 3 7 2 5 4 10 5 8 3 8 10 2 4 3 -18 4 9 5 1 ", "0 10 - - - * - - - - ");
	//Test("6 8 2 3 -1 3 2 -1 1 4 1 1 5 1 1 6 1 2 4 1 2 5 1 2 6 1 1 ", "0 * * 1 1 1 ");
	//Test("5 6 1 2 1 2 3 -2 3 2 -2 4 1 1 4 5 -2 5 4 -2 1 ", "0 - - * * ");
	//Test("5 6 1 2 1 2 3 -2 3 2 -2 1 4 1 4 5 -2 5 4 -2 1 ", "0 - - - - ");
	//Test("5 6 2 1 1 2 3 -2 3 2 -2 4 1 1 4 5 -2 5 4 -2 1 ", "0 * * * * ");
	//Test("5 7 1 2 10 2 3 5 1 3 100 3 5 7 5 4 10 4 3 -18 5 1 -1 1", "- - - - - ");
	//Test("3 2 2 3 -1000 3 2 -1000 1 ", "0 * * "); 
	//Test("5 6 2 1 1 1 4 4 4 2 1 2 3 -4 3 4 1 3 5 4 1 ", "- - - - - ");
	
	return 0;
}
