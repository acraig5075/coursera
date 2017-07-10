#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <iterator>
#include <cmath>

using std::vector;
using std::pair;
using std::min;
using std::begin;
using std::end;
using std::sort;

using point = pair<int, int>;

double distance(const point &a, const point &b)
{
	return std::hypot(b.first - a.first, b.second - a.second);
}

double minimal_distance_strip(vector<point> &strip, double dist)
{
	if (strip.size() < 2)
		return std::numeric_limits<double>::max();

	sort(begin(strip), end(strip), [](const point &a, const point &b)
	{
		return a.second < b.second;
	});

	double minlr = dist;
	auto a = begin(strip);

	for (auto b = a + 1; b != end(strip); a = b, b += 1)
	{
		if (b->second - a->second < dist)
		{
			double d = distance(*a, *b);
			if (d < minlr)
				minlr = d;
		}
	}

	return minlr;
}

double minimal_distance(const vector<point>::iterator &left, const vector<point>::iterator &right)
{
	size_t n = right - left + 1;
	assert(n > 0);

	if (n == 1)
		return std::numeric_limits<double>::max();
	if (n == 2)
		return distance(*left, *(left + 1));

	auto mid = left + (n / 2);
	double minl = minimal_distance(left, mid);
	double minr = minimal_distance(mid + 1, right);

	double dist = min(minl, minr);
	double stripl = mid->first - dist;
	double stripr = mid->first + dist;

	vector<point> strip;
	std::copy_if(left, right + 1, std::back_inserter(strip), [stripl, stripr](const point &p)
	{
		return stripl <= p.first && p.first <= stripr;
	});

	double minlr = minimal_distance_strip(strip, dist);

	return min(dist, minlr);
}

double minimal_distance(vector<point> &pts)
{
	if (pts.size() <= 1)
		return 0.0;

	return minimal_distance(begin(pts), end(pts) - 1);
}

int main()
{
	size_t n;

	std::cin >> n;
	vector<point> pts(n);

	for (size_t i = 0; i < n; i++)
	{
		std::cin >> pts[i].first >> pts[i].second;
	}

	//vector<point> pts = { { 2, 3 },{ 12, 30 },{ 40, 50 },{ 5, 1 },{ 12, 10 },{ 3, 4 } };
	//vector<point> pts = { { 4, 4 },{ -2, -2 },{ -3, -4 },{ -1, 3 },{ 2, 3 },{ -4, 0 },{ 1, 1 },{ -1, -1 },{ 3, -1 },{ -4, 2 },{ -2, 4 } };

	sort(begin(pts), end(pts), [](const point &a, const point &b)
	{
		return a.first < b.first;
	});

	std::cout << std::fixed << std::setprecision(9) << minimal_distance(pts) << "\n";
}
