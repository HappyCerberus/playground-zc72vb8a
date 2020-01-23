#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
#include <queue>
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

priority_queue<int, vector<int>, greater<int>> build_priority_queue(const vector<int>& v, int k) {
	priority_queue<int, vector<int>, greater<int>> q;
	for (auto i : v) {
		// If we have space, or the lowest element is lower than current.
		if (q.size() < k || q.top() < i) {
			q.push(i);
		}
		// If we have to many elements, remove the lowest one.
		if (q.size() > k) {
			q.pop();
		}
	}
	return q;
}

int medium_kth(const vector<int>& v, int k) {
	auto q = build_priority_queue(v, k);
	do {
		if (q.size() == k)
			return q.top();
		q.pop();
	} while (!q.empty());
	return -1;
}

vector<int> medium_topk(const vector<int>& v, int k) {
	auto q = build_priority_queue(v, k);
	vector<int> result;
	while (!q.empty()) {
		result.push_back(q.top());
		q.pop();
	}
	return result;
}

size_t median_of_medians(vector<int>& v, size_t l, size_t r);
size_t partition_3way(vector<int>& v, size_t l, size_t r, size_t p, size_t k);

size_t select(vector<int>& v, size_t l, size_t r, size_t k) {
	while (l != r) {
		size_t p = median_of_medians(v, l, r);
		p = partition_3way(v, l, r, p, k);

		if (k == p)
			return k;
		else if (k < p)
			r = p - 1;
		else
			l = p + 1;
	}
	return l;
}

size_t partition_3way(vector<int>& v, size_t l, size_t r, size_t p, size_t k) {
	int value = v[p];
	swap(v[p], v[r]);
	
	// Find elements smaller than pivot and move them to the begining of this section.
	size_t store = l;
	for (size_t i = l; i < r; i++) {
		if (v[i] > value) {
			swap(v[store],v[i]);
			store++;
		}
	}

	// Find elements equal to the pivot (to handle repeated values) right after elements smaller than pivot.
	size_t storeEq = store;
	for (size_t i = store; i < r; i++) {
		if (v[i] == value) {
			swap(v[storeEq], v[i]);
			storeEq++;
		}
	}

	// Move the pivot right after the elements equal to it.
	swap(v[r],v[storeEq]);

	// If our target is within the smaller elements, we return the index right after all the smaller elements.
	if (k < store) return store;
	// If our target is within the equal elements, we return the target.
	if (k <= storeEq) return k;
	// Otherwise our target is within the larger elements, we return the index right after all the equal elements.
	return storeEq;
}

size_t median_of_medians(vector<int>& v, size_t l, size_t r) {
	if (r-l < 5) {
		// Sort the constant size array.
		sort(begin(v)+l, begin(v)+r, greater<int>());
		// Simply return median, since we have just one array.
		return (l + r) / 2;
	}

	for (size_t i = l; i < r; i+=5) {
		size_t r_new = min(i+4, r);
		// Sort the constant size array.
		sort(begin(v)+i, begin(v)+r_new, greater<int>());

		// Move the newly calculate median to the front of this section.
		size_t med = (i+r_new)/2;
		swap(v[med], v[l+(i-l)/5]);
	}

	// Recurse to get median of medians.
	size_t mid = (r-l)/10 + l + 1;
	return select(v, l, l+(r-l)/5, mid);
}



int complex_kth(vector<int> v, size_t k) {
	return v[select(v,0,v.size()-1,k-1)];
}


int main() {
	// demo code
	cout << "Generating random array." << endl;
	auto seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine dre(seed);
	uniform_int_distribution<int> di(0,1000);

	vector<int> v(49);
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

	// heap "medium" solution
	{
		vector<int> topk = medium_topk(v, 5);
		cout << "Result of medium_topk(v, 5):" << endl;
		copy(begin(topk), end(topk), ostream_iterator<int>(cout, " "));
		cout << endl;

		int kth = medium_kth(v, 5);
		cout << "Result of medium_kth(v, 5):" << endl;
		cout << kth;
		cout << endl;
	}

	// complex solution
	{
		int kth = complex_kth(v, 5);
		cout << "Result of medium_kth(v, 5):" << endl;
		cout << kth;
		cout << endl;
	}




}
