#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <queue>
#include <algorithm>
#include <utility>

using std::vector;
using std::string;
using std::stringstream;
using std::pair;

#define infinity 1000000000

/* This class implements a bit unusual scheme for storing edges of the graph,
   in order to retrieve the backward edge for a given edge quickly. */
class FlowGraph
{
public:
	struct Edge
	{
		int from, to, capacity, flow;
	};

private:
	/* List of all - forward and backward - edges */
	vector<Edge> edges;

	/* These adjacency lists store only indices of edges in the edges list */
	vector<vector<size_t>> graph;

public:
	explicit FlowGraph(size_t n): graph(n) {}

	void add_edge(int from, int to, int capacity)
	{
		/* Note that we first append a forward edge and then a backward edge,
		   so all forward edges are stored at even indices (starting from 0),
		   whereas backward edges are stored at odd indices in the list edges */
		Edge forward_edge = { from, to, capacity, 0 };
		Edge backward_edge = { to, from, 0, 0 };

		graph[from].push_back(edges.size());
		edges.push_back(forward_edge);

		graph[to].push_back(edges.size());
		edges.push_back(backward_edge);
	}

	size_t size() const
	{
		return graph.size();
	}

	const vector<size_t> &get_ids(int from) const
	{
		return graph[from];
	}

	const Edge &get_edge(size_t id) const
	{
		return edges[id];
	}

	void add_flow(size_t id, int flow)
	{
		/* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
		   due to the described above scheme. On the other hand, when we have to get a "backward"
		   edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
		   should be taken.

		   It turns out that id ^ 1 works for both cases. Think this through! */
		edges[id].flow += flow;
		edges[id].capacity -= flow;
		edges[id ^ 1].flow += flow;
		edges[id ^ 1].capacity = flow;

	}
};

FlowGraph read_data(std::istream &in)
{
	int vertex_count, edge_count;
	in >> vertex_count >> edge_count;

	FlowGraph graph(vertex_count);
	for (int i = 0; i < edge_count; ++i)
		{
		int u, v, capacity;
		in >> u >> v >> capacity;
		graph.add_edge(u - 1, v - 1, capacity);
		}

	return graph;
}

vector<size_t> bfs(FlowGraph &graph, int s, int t)
{
	size_t n = graph.size();
	vector<int> dist(n, infinity);
	vector <pair<int, size_t>> prev(n, { 0, 0 });

	dist[s] = 0;

	std::queue<int> q;
	q.push(s);

	// construct shortest-path tree
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		
		const vector<size_t> &ids = graph.get_ids(u);

		for (auto i : ids)
		{
			FlowGraph::Edge e = graph.get_edge(i);

			if (dist[e.to] == infinity && e.capacity > 0)
			{
				q.push(e.to);
				dist[e.to] = dist[u] + 1;
				prev[e.to] = { u, i };
			}
		}
	}

	// no path if sink is not reachable
	if (dist[t] == infinity)
		return{};

	// reconstruct path
	vector<size_t> result;
	int u = t;
	while (u != s)
	{
		result.push_back(prev[u].second);
		u = prev[u].first;
	}

	std::reverse(begin(result), end(result));

	return result;
}

int minimum_capacity(const FlowGraph &graph, const vector<size_t> &path)
{
	int x = std::numeric_limits<int>::max();

	for (auto id : path)
	{
		auto edge = graph.get_edge(id);

		if (edge.capacity < x)
			x = edge.capacity;
	}

	return x;
}

int do_flow_path(FlowGraph &graph, const vector<size_t> &path)
{
	int x = minimum_capacity(graph, path);

	for (auto id : path)
	{
		graph.add_flow(id, x);
	}

	return x;
}

int max_flow(FlowGraph &graph, int from, int to)
{
	int flow = 0;

	while (true)
	{
		vector<size_t> path = bfs(graph, from, to);
		if (path.empty())
			return flow;
		
		flow += do_flow_path(graph, path);
	}

	return flow;
}

void my_main(std::istream &in, std::ostream &out)
{
	std::ios_base::sync_with_stdio(false);

	FlowGraph graph = read_data(in);

	out << max_flow(graph, 0, graph.size() - 1) << "\n";
}

int main()
{
	//my_main(std::cin, std::cout);

	auto Test = [](string input, string expected)
		{
		stringstream in(input);
		stringstream out;
		my_main(in, out);
		string actual = out.str();
		assert(actual == expected);
		};

	Test("4 5 1 3 5 3 4 10 1 2 7 2 4 5 2 3 3", "12\n");
	Test("5 7 1 2 2 2 5 5 1 3 6 3 4 2 4 5 1 3 2 3 2 4 1", "6\n");
	Test("4 5 1 2 10000 1 3 10000 2 3 1 3 4 10000 2 4 10000", "20000\n");
	Test("6 7 1 2 1 1 3 1 2 4 1 2 5 1 3 4 1 4 6 1 5 6 1", "2\n");
	Test("8 9 1 2 5 1 3 20 2 5 5 2 6 5 3 4 20 4 5 20 5 8 20 6 7 5 7 8 5", "25\n");
	Test("9 16 1 2 69 1 7 76 2 3 95 2 4 53 2 5 9 2 8 85 3 4 45 3 5 54 4 7 13 4 9 16 5 6 49 5 9 24 6 7 52 7 8 100 7 9 3 8 9 63", "106\n");
	Test("10 28 1 1 8 1 3 4 1 5 35 1 6 39 1 7 97 1 8 62 1 8 89 2 5 63 2 5 74 2 10 75 3 9 45 3 10 40 4 2 22 4 3 31 4 10 10 5 8 77 6 3 50 6 5 61 7 3 12 7 4 27 7 10 49 8 2 66 8 6 19 8 9 51 8 10 54 8 10 98 9 6 4 10 10 91", "317\n");

	return 0;
}