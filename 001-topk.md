# Top $`k`$ elements and $`k`$-th element

Two twists on the same idea that show up frequently in coding interviews in various incarnations:

 - given an unsorted array of objects of length $`n`$, find the top $`k`$ objects based on a given comparison function
 - given an unsorted array of objects of length $`n`$, find $`k`$-th object based on a given comparion function, that is an object that would appear at the $`k`$-th place in an sorted array
 
It is usually mentioned that you can expect $`k << n`$, that is the length of the array is much bigger than the number of elements we are interested in.

In this article, we will go through 3 solutions to these problems:

  - a trivial solution with $`O(n * log(n))`$ complexity
  - a medium complexity solution with $`O(n * log(k))`$ complexity
  - a complex solution that relies on prior knowledge of specific algorithms with $`O(n)`$ complexity


```C++ runnable
#include <iostream>

using namespace std;

int main() 
{
    cout << "Hello, World!";
    return 0;
}
```

# Advanced usage

If you want a more complex example (external libraries, viewers...), use the [Advanced C++ template](https://tech.io/select-repo/598)
