#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cassert>
#include <utility>
#include <queue>

using std::vector;
using std::string;
using std::stringstream;
using std::pair;

#define infinity 1000000000

class FlowGraph;

FlowGraph ConstructNetwork(const vector<vector<bool>>& adj_matrix);
int max_flow(FlowGraph &graph, int from, int to);

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
	explicit FlowGraph(size_t n) : graph(n) {}

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

class MaxMatching
{
public:
	void Solve(std::istream &in, std::ostream &out)
	{
		vector<vector<bool>> adj_matrix = ReadData(in);
		vector<int> matching = FindMatching(adj_matrix);
		WriteResponse(out, matching);
	}

private:
	vector<vector<bool>> ReadData(std::istream &in)
	{
		int num_left, num_right;
		in >> num_left >> num_right;
		vector<vector<bool>> adj_matrix(num_left, vector<bool>(num_right));

		for (int i = 0; i < num_left; ++i)
		{
			for (int j = 0; j < num_right; ++j)
			{
				int bit;
				in >> bit;
				adj_matrix[i][j] = (bit == 1);
			}
		}
		return adj_matrix;
	}

	vector<int> FindMatching(const vector<vector<bool>>& adj_matrix)
	{
		FlowGraph graph = ConstructNetwork(adj_matrix);

		/*int flow =*/ max_flow(graph, 0, graph.size() - 1);

		int num_flights = adj_matrix.size();
		vector<int> matching(num_flights, -1);

		for (int from = 0; from < num_flights; ++from)
		{
			auto ids = graph.get_ids(from + 1);

			for (auto id : ids)
			{
				auto edge = graph.get_edge(id);
				if (edge.from < edge.to && edge.flow > 0)
					matching[from] = edge.to - num_flights;
			}

		}

		return matching;
	}

	void WriteResponse(std::ostream &out, vector<int>& matching)
	{
		for (size_t i = 0; i < matching.size(); ++i)
		{
			if (i > 0)
				out << " ";
			if (matching[i] == -1)
				out << "-1";
			else
				out << (matching[i]);
		}
		out << "\n";
	}
};

FlowGraph ConstructNetwork(const vector<vector<bool>>& adj_matrix)
{
	int num_flights = adj_matrix.size();
	int num_crews = adj_matrix.front().size();
	int vertex_count = num_flights + num_crews + 2; // 2 extra for source and sink

	FlowGraph graph(vertex_count);

	// left-hand side edges
	int source_id = 0;
	for (int f = 0; f < num_flights; ++f)
		graph.add_edge(source_id, f + 1, 1);

	// middle edges
	for (int f = 0; f < num_flights; ++f)
	{
		for (int c = 0; c < num_crews; ++c)
		{
			if (adj_matrix[f][c])
				graph.add_edge(f + 1, num_flights + c + 1, 1);
		}
	}

	// right-hand side edges
	int sink_id = num_flights + num_crews + 1;
	for (int c = 0; c < num_crews; ++c)
		graph.add_edge(num_flights + c + 1, sink_id, 1);

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
	MaxMatching max_matching;
	max_matching.Solve(in, out);
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
	//	assert(actual == expected);
	//};

	//Test("3 4 1 1 0 1 0 1 0 0 0 0 0 0", "1 2 -1\n");
	//Test("2 2 1 1 1 0", "2 1\n");
	//Test("2 2 0 0 0 0", "-1 -1\n");

	return 0;
}
