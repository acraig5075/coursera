#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

int LeftChild(int i)
{
	return 2 * i + 1;
}

int RightChild(int i)
{
	return 2 * i + 2;
}

class HeapBuilder {
 private:
  vector<int> data_;
  vector< pair<int, int> > swaps_;

  void WriteResponse() const {
    cout << swaps_.size() << "\n";
    for (int i = 0; i < (int)swaps_.size(); ++i) {
      cout << swaps_[i].first << " " << swaps_[i].second << "\n";
    }
  }

  void ReadData() {
    int n;
    cin >> n;
    data_.resize(n);
    for(int i = 0; i < n; ++i)
      cin >> data_[i];
  }

	void GenerateSwaps()
	{
		swaps_.clear();
		for (int i = data_.size() / 2; i >= 0; i--)
			SiftDown(i);
	}

	void SiftDown(int i)
	{
		int maxIndex = i;
		int l = LeftChild(i);
		if (l < (int)data_.size() && data_[l] < data_[maxIndex])
			maxIndex = l;
		int r = RightChild(i);
		if (r < (int)data_.size() && data_[r] < data_[maxIndex])
			maxIndex = r;
		if (i != maxIndex)
		{
			swaps_.push_back(make_pair(i, maxIndex));
			swap(data_[i], data_[maxIndex]);
			SiftDown(maxIndex);
		}
	}

	public:
	void Solve()
	{
		ReadData();
		GenerateSwaps();
		WriteResponse();
	}
};

int main() {
  std::ios_base::sync_with_stdio(false);
  HeapBuilder heap_builder;
  heap_builder.Solve();
  return 0;
}
