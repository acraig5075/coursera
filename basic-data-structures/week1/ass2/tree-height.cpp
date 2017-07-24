#include <algorithm>
#include <iostream>
#include <vector>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif
#include <algorithm>

class Node;

class Node
{
public:
	int key;
	Node *parent;
	std::vector<Node *> children;

	Node()
	{
		this->parent = NULL;
	}

	void setParent(Node *theParent)
	{
		parent = theParent;
		parent->children.push_back(this);
	}
};

int height(int node_height, const Node *node)
{
	if (node->children.empty())
		return node_height;

	const int child_height = node_height + 1;

	int max_height = child_height;

	for (const auto &child : node->children)
		{
		int val = height(child_height, child);

		if (val > max_height)
			max_height = val;
		}

	return max_height;
}

int tree_height(int n, const std::vector<Node> &nodes)
{
	auto root_itr = std::find_if(begin(nodes), end(nodes), [](const Node &node)
		{
		return node.parent == nullptr;
		});

	if (root_itr == end(nodes))
		return 0;

	int root_index = root_itr - begin(nodes);

	int root_height = 1;

	return height(root_height, &nodes[root_index]);
}

int main_with_large_stack_space()
{
	std::ios_base::sync_with_stdio(0);
	int n;
	std::cin >> n;

	std::vector<Node> nodes;
	nodes.resize(n);
	for (int child_index = 0; child_index < n; child_index++)
		{
		int parent_index;
		std::cin >> parent_index;
		if (parent_index >= 0)
			nodes[child_index].setParent(&nodes[parent_index]);
		nodes[child_index].key = child_index;
		}

	int maxHeight = tree_height(n, nodes);

	std::cout << maxHeight << std::endl;
	return 0;
}

int main(int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
	// Allow larger stack space
	const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
	struct rlimit rl;
	int result;

	result = getrlimit(RLIMIT_STACK, &rl);
	if (result == 0)
		{
		if (rl.rlim_cur < kStackSize)
			{
			rl.rlim_cur = kStackSize;
			result = setrlimit(RLIMIT_STACK, &rl);
			if (result != 0)
				{
				std::cerr << "setrlimit returned result = " << result << std::endl;
				}
			}
		}

#endif
	return main_with_large_stack_space();
}
