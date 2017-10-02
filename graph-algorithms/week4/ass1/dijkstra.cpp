#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <queue>
#include <deque>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;
using std::queue;
using std::deque;

#define infinity -1

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
			int x, y, w;
			in >> x >> y >> w;
			x--;
			y--;

			adjacency_list[x].push_back({ y, w });
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
	};

	deque<dijkstr_type> make_queue()
	{
		deque<dijkstr_type> h;

		for (int n = 0; n < (int)dist.size(); ++n)
		{
			h.push_back({ n, dist[n] });
		}

		return h;
	}

	int extract_min(deque<dijkstr_type> &h)
	{
		sort(begin(h), end(h));

		auto front = h.front();
		h.pop_front();

		return front.node;
	}

	void change_priority(deque<dijkstr_type> &h, int node, int dist)
	{
		auto itr = find_if(begin(h), end(h), [node](const dijkstr_type &item) 
		{
			return item.node == node;
		});

		if (itr != end(h))
			itr->dist = dist;
	}

	// week 4, assignment 1
	void dijkstra(int s)
	{
		dist.resize(n, std::numeric_limits<int>::max());
		prev.resize(n, -1);

		dist[s] = 0;
		bool changed = true;

		auto h = make_queue();

		while (changed && !h.empty())
		{
			changed = false;
			int u = extract_min(h);
			for (auto edge : adjacency_list[u])
			{
				if (dist[edge.to] > dist[u] + edge.weight)
				{
					dist[edge.to] = dist[u] + edge.weight;
					prev[edge.to] = u;
					change_priority(h, edge.to, dist[edge.to]);
					changed = true;
				}
			}
		}
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

	int a, b;
	in >> a >> b;
	a--;
	b--;

	g.dijkstra(a);

	int distance = g.get_distance(b);

	if (distance == std::numeric_limits<int>::max())
		out << "-1";
	else
		out << distance;
}

int main()
{
	//my_main(std::cin, std::cout);

	auto Test = [](string input, string expected)
	{
		stringstream in(input);
		stringstream out;
		my_main(in, out);
		assert(out.str() == expected);
	};


	Test("6 11 1 2 3 1 3 10 2 3 8 2 4 3 2 5 5 3 2 2 3 5 5 4 3 3 4 5 1 4 6 2 5 6 0 1 6 ", "7");
	Test("4 4 1 2 1 4 1 2 2 3 2 1 3 5 1 3 ", "3");
	Test("5 9 1 2 4 1 3 2 2 3 2 3 2 1 2 4 2 3 5 4 5 4 1 2 5 3 3 4 4 1 5 ", "6");
	Test("3 3 1 2 7 1 3 5 2 3 2 3 2 ", "-1");
	Test("1 0 1 1 ", "0");
	Test("6 11 1 2 3 1 3 10 2 3 8 2 4 3 2 5 5 3 2 2 3 5 5 4 3 3 4 5 1 4 6 2 6 5 0 1 6 ", "8");
	Test("6 11 1 2 3 1 3 10 2 3 8 2 4 3 2 5 5 3 2 2 3 5 5 4 3 3 4 5 1 6 4 2 6 5 0 1 6 ", "-1");

	return 0;
}
