# Top $`k`$ elements and $`k`$-th element

Two twists on the same idea that show up frequently in coding interviews in various incarnations:

 - given an unsorted array of objects of length $`n`$, find the top $`k`$ objects based on a given comparison function
 - given an unsorted array of objects of length $`n`$, find $`k`$-th object based on a given comparion function, that is an object that would appear at the $`k`$-th place in an sorted array
 
You can expect $`k << n`$, that is the length of the array is much bigger than the number of elements we are interested in.

In this article, we will go through 3 solutions to these problems:

  - a trivial solution with $`O(n * log(n))`$ complexity
  - a medium complexity solution with $`O(n * log(k))`$ complexity
  - a complex solution that relies on prior knowledge of specific algorithms with $`O(n)`$ complexity
  
All solutions are presented using an array of ints to minimize amount of boilerplate.

## Trivial solution $`O(n*log(n))`$

The trivial solution is given to us on a silver plate in the assignment text.

If the array would be sorted, returning either the $`k`$-th element or the top $`k`$ elements would be trivial.

So let's just do that. Sort the array and simply return the $`k`$-th element or the top $`k`$ elements of the sorted array.


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

We sort the array, which has $`n`$ elements resulting in $`O(n*log(n))`$ complexity.
Since sorting is a destructive operation we also need to create a copy of the array resulting in $`O(n)`$ space complexity.

## Medium solution $`O(n*log(k))`$

When we consider optimizations, we need to look at the unnecessary work we are doing. Since we do not care about the elements besides the top $`k`$ we do not need to sort them. The standard library already offers an appropriate function ``partial_sort``.

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
}
// }
```

### Complexity analysis of partial sort solution

Time complexity of ``partial_sort`` is $`O(n*log(k))`$.
We are still copying the array, so space complexity remains unchanged at $`O(n)`$.

## Medium solution with $`O(k)`$ space complexity

One problem with the previous straightforward solution is that we still need to make copy of the entire array, since ``partial_sort`` is still destructive, leaving us with $`O(n)`$ space complexity.

We must store the top $`k`$ elements, so the space complexity will be at least $`O(k)`$. If we scan through the array and remember the top $`k`$ elements upto now, we would end up with $`O(k)`$ space complexity.

Datastructure we require must have the ability to insert elements, look a the smallest element and remove the smallest element all in $`log(k)`$ time. Fortunately, heap provides such characteristics. In C++ we can use the ``priority_queue`` (which is not necessarily implemented as a heap, but providing the same performance characteristics).

```C++ runnable
// { autofold
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
#include <queue>
using namespace std;
// }
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
}
// }
```

### Complexity analysis of medium solution

We scan through all the elements of the array and at worst case we will insert each of them into the ``priority_queue``. This leads to total $`O(n*2*log(k)`$ complexity ($`n`$ calls of ``push()`` & ``pop()``, each with $`log(k)`$ complexity), ultimately collapsing to $`O(n*log(k))`$. As already discussed the space complexity of this approach is $`O(k)`$, as we only ever store $`k+1`$ elements in the ``priority_queue``.

## Complex solution $`O(n)`$

First observation to notice is that if we know the $`k`$-th element, getting the top $`k`$ elements is a simple scan through the array with $`O(n)`$ complexity. So let's just concentrate on solving the $`k`$-th element in $`O(n)`$ time.

Let's imagine that would believe in our luck and just blindly guess that a particular element is the $`k`$-the element. How would we verify that? If we use this element as a pivot and partition the array around it, we end up with all the bigger elements on its left and all the smaller elements on its left. If the picked element ends up on the $`k`$-th position, we know that our guess was correct.

What happens if we weren't correct? Well, we know on which side of the pivot our $`k`$-the element lies at least. If the pivot ended up on a position $`i<k`$ then we need to search the elements to the right of the pivot, if it ended up on a position $`i>k`$, we need to search to the left.

This might sound very similar to the Quicksort algorithm and indeed, this is referred to as the Quickselect algorithm.

There is one more important step however. You might recall that Quicksort with a random pivot is $`O(n^2)`$ in worst case. Quickselect suffers the same faith unfortunately.

There is one algorithm however that might save us.

### Median of medians

The source for the worst case of Quickselect is a corner case when we keep choosing either the smallest or the largest element as our pivot. In that case we only improve the situation by one element, recursing over $`n+(n-1)+(n-2)...`$ elements which ends up in the $`O(n^2)`$ class.

What if we could pick the pivot somewhat well. Let's say that we always pick a pivot that splits that array in such a way that the larger portion has $`q*n; q < 1`$ elements. Now the recursive pattern would be a geometric progression of $`n + q*n + q^2*n...`$ Sum of such progression is $`\sum_1^n q^i*n = n*\frac{1-q^n}{1-q}`$. For all $`q < 1`$ the right side of the sum ends up a constant number as $`q^n < 1`$.
