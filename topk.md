# Top $`k`$ elements and $`k`$-th element

Two twists on the same idea that show up frequently in coding interviews in various incarnations:

 - given an unsorted array of objects of length $`n`$, find the top $`k`$ objects based on a given comparison function
 - given an unsorted array of objects of length $`n`$, find $`k`$-th object based on a given comparion function, that is an object that would appear at the $`k`$-th place in an sorted array
 
It is usually mentioned that you can expect $`k << n`$, that is the length of the array is much bigger than the number of elements we are interested in.

In this article, we will go through 3 solutions to these problems:

  - a trivial solution with $`O(n * log(n))`$ complexity
  - a medium complexity solution with $`O(n * log(k))`$ complexity
  - a complex solution that relies on prior knowledge of specific algorithms with $`O(n)`$ complexity

## Trivial solution $`O(n*log(n))`$

The trivial solution is given to us on a silver plate in the assignment text.

If the array would be sorted, returning either the $`k`$-th element or the top $`k`$ elements would be trivial. So let's just do that. Sort the array and simply return the $`k`$-th element or the top $`k`$ elements of the sorted array.


```C++ runnable
// { autofold
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
using namespace std;
// }
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
// { autofold
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
// }
```

### Complexity analysis of trivial solution

We sort the array, which has $`n`$ elements resulting in $`O(n*log(n)`$ complexity.
Since sorting is a destructive operation we also need to create a copy of the array resulting in $`O(n)`$ space complexity.
