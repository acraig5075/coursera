#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>

using std::vector;
using std::cin;
using std::cout;
using std::priority_queue;

class JobQueue
{
private:
	int num_workers_;
	vector<int> jobs_;

	vector<int> assigned_workers_;
	vector<long long> start_times_;

	void WriteResponse() const
	{
		for (size_t i = 0; i < jobs_.size(); ++i)
			{
			cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
			}
	}

	void ReadData()
	{
		int m;
		cin >> num_workers_ >> m;
		jobs_.resize(m);
		for (int i = 0; i < m; ++i)
			cin >> jobs_[i];
	}

	void AssignJobs()
	{
		priority_queue<long long, vector<long long>, std::greater<long long>> heap;
		heap.push(0);

		vector<long long> endTimes(num_workers_);

		auto next = jobs_.begin();

		while (!heap.empty() && next != jobs_.end())
		{
			auto now = heap.top();
			heap.pop();

			int worker = 0;

			for (auto &t : endTimes)
			{
				if (now == t)
				{
					t = now + *next;
					heap.push(t);
					assigned_workers_.push_back(worker);
					start_times_.push_back(now);
					++next;
				}

				++worker;

				if (next == jobs_.end())
					break;
			}
		}
	}

public:
	void Solve()
	{
		ReadData();
		AssignJobs();
		WriteResponse();
	}
};

int main()
{
	std::ios_base::sync_with_stdio(false);
	JobQueue job_queue;
	job_queue.Solve();
	return 0;
}
