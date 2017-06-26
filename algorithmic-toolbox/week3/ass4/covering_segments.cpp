#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

using std::vector;
using std::begin;
using std::end;

struct Segment
{
	int start, end;
};

vector<int> optimal_points(vector<Segment> &segments)
{
	// sort segments by increasing end point
	std::sort(begin(segments), end(segments), [](const Segment &a, const Segment &b)
		{
		return a.end < b.end;
		});

	vector<int> points;

	// store the ned point because we're going to be constantly changing the length
	auto new_end = end(segments);

	while (begin(segments) != new_end)
		{
		// safe decision is to look at earliest end point
		int point = segments.at(0).end;

		// remove segments that overlap the safe decision
		new_end = std::remove_if(begin(segments), new_end, [point](const Segment &s)
			{
			return s.start <= point && point <= s.end;
			});

		// add safe decision and iterate until no more
		points.push_back(point);
		}

	// sort output for convenience
	std::sort(begin(points), end(points));
	return points;
}

int main()
{
	int n;
	std::cin >> n;
	vector<Segment> segments(n);
	for (size_t i = 0; i < segments.size(); ++i)
		std::cin >> segments[i].start >> segments[i].end;

	vector<int> points = optimal_points(segments);
	std::cout << points.size() << "\n";
	for (size_t i = 0; i < points.size(); ++i)
		std::cout << points[i] << " ";

	std::cout << "\n";
}
