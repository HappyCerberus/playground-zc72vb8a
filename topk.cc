#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
using namespace std;

int trivial_kth(vector<int> v, int k) {
	// Interpreting k as ordinal number: 1 refers to the top element.
	if (k > v.size())
		return -1;

	// Simply sort and return k-th element.
	sort(begin(v), end(v), greater<int>());
	return v[k-1];
}

vector<int> trivial_topk(vector<int> v, int k) {
	// Simply sort the array.
	sort(begin(v), end(v), greater<int>());

	// Resize to include only the top k elements.
	if (k < v.size())
		v.resize(k);
	return v;
}

int partial_sort_kth(vector<int> v, int k) {
	// Interpreting k as ordinal number: 1 refers to the top element.
	if (k > v.size())
		return -1;

	// Partially sort and return k-th element.
	partial_sort(begin(v), begin(v)+k, end(v), greater<int>());
	return v[k-1];
}

vector<int> partial_sort_topk(vector<int> v, int k) {
	// Partially sort sort the array.
	partial_sort(begin(v), begin(v)+k, end(v), greater<int>());

	// Resize to include only the top k elements.
	if (k < v.size())
		v.resize(k);
	return v;
}

int main() {
	// demo code
	cout << "Generating random array." << endl;
	auto seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine dre(seed);
	uniform_int_distribution<int> di(0,1000);

	vector<int> v(50);
	generate(begin(v), end(v), [&]{ return di(dre); });

	copy(begin(v), end(v), ostream_iterator<int>(cout, " "));
	cout << endl << endl;

	// trivial solutions
	{
		vector<int> topk = trivial_topk(v, 5);
		cout << "Result of trivial_topk(v, 5):" << endl;
		copy(begin(topk), end(topk), ostream_iterator<int>(cout, " "));
		cout << endl;

		int kth = trivial_kth(v, 5);
		cout << "Result of trivial_kth(v, 5):" << endl;
		cout << kth;
		cout << endl;
	}

	// partial sort solutions
	{
		vector<int> topk = partial_sort_topk(v, 5);
		cout << "Result of partial_sort_topk(v, 5):" << endl;
		copy(begin(topk), end(topk), ostream_iterator<int>(cout, " "));
		cout << endl;

		int kth = partial_sort_kth(v, 5);
		cout << "Result of partial_sort_kth(v, 5):" << endl;
		cout << kth;
		cout << endl;
	}

	// explicit tests
	// fuzz test
}
