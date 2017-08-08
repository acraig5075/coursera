#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

//#define TESTING

#if defined TESTING
#include <chrono>
#endif

using std::vector;
using std::cin;
using std::cout;
using std::priority_queue;

struct item
{
	size_t threadId = 0;
	long long time = 0;

	item(size_t id, long long t)
		: threadId(id)
		, time(t)
		{}
};

struct comparator
{
	bool operator()(const item &a, const item &b)
	{
		if (a.time == b.time)
			return a.threadId > b.threadId;
		else
			return a.time > b.time;
	}
};

class JobQueue
{
private:
	size_t m_ = 0;
	size_t num_workers_ = 0;

	vector<int> jobs_;
	vector<item> output_;

	void WriteResponse() const
	{
		size_t i = 0;
		for (auto it = begin(output_); it != end(output_) && i < m_; ++it, ++i)
			{
			printf("%d %lld\n", it->threadId, it->time);
			}
	}

	void ReadData()
	{
		cin >> num_workers_ >> m_;

		jobs_.resize(m_);
		output_.reserve(m_);

		for (size_t i = 0; i < m_; ++i)
			cin >> jobs_[i];
	}


	void AssignJobs()
	{
		priority_queue<item, vector<item>, comparator> heap;

		for (size_t thread = 0; thread < num_workers_; ++thread)
			heap.push({ thread, 0 });

		auto nextJob = jobs_.begin();

		while (!heap.empty())
			{
			auto task = heap.top();
			heap.pop();

			output_.push_back({ task.threadId, task.time });

			task.time += *nextJob;
			heap.push(task);

			++nextJob;
			if (nextJob == jobs_.end())
				break;
			}
	}


public:
	void Solve()
	{
#if defined TESTING
		auto startTime = std::chrono::high_resolution_clock::now();
		ReadData();
		auto midTime1 = std::chrono::high_resolution_clock::now();
		AssignJobs();
		auto midTime2 = std::chrono::high_resolution_clock::now();
		WriteResponse();
		auto endTime = std::chrono::high_resolution_clock::now();

		auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(midTime1 - startTime).count();
		auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(midTime2 - midTime1).count();
		auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - midTime2).count();

		std::cout << "Duration1 = " << duration1 << " | Duration 2 = " << duration2 << " | Duration 3 = " << duration3 << "\n";
#else
		ReadData();
		AssignJobs();
		WriteResponse();
#endif
	}
};

int main()
{
	std::ios_base::sync_with_stdio(false);
	JobQueue job_queue;
	job_queue.Solve();
	return 0;
}
