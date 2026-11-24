// Name    : Farhad Khan
// Reg No  : 549034
// Section : BSCS-15E
// Lab 11, Task 2: Heap sort

// Why a max heap gives ascending output: the root of a max heap is the
// biggest value. Moving it to the last place of the active heap puts the
// biggest value where it belongs in ascending order. The next biggest is then
// the new root, and so on, so the array fills up from the right.
//
// Why buildMaxHeap is called only once: after the swap only the new root can
// break the rule, because both subtrees below it are still heaps. One
// maxHeapify at index 0 repairs that in O(log n). Building the heap again
// would cost O(n) every time for no reason.
//
// Time: bottom up construction is O(n). Most nodes are near the bottom and
// sink very little (half of the nodes are leaves and do not move at all), so
// the total work adds up to O(n) and not O(n log n). Heap sort then does n - 1
// extractions and each repair costs at most the tree height, O(log n), so the
// worst case of the whole sort is O(n log n). It sorts in place and needs only
// a few extra variables.

#include <iostream>
using namespace std;

const int MAX = 20;   // largest array used in the tests

void swapInts(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// from Task 1: pushes the value at index i down inside a heap of n elements
void maxHeapify(int arr[], int n, int i)
{
    while (true)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest])
        {
            largest = left;
        }
        if (right < n && arr[right] > arr[largest])
        {
            largest = right;
        }

        if (largest == i)
        {
            return;
        }

        swapInts(arr[i], arr[largest]);
        i = largest;
    }
}

// from Task 1: builds a max heap from the last non-leaf node back to index 0
void buildMaxHeap(int arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        maxHeapify(arr, n, i);
    }
}

void printArray(const int arr[], int n)
{
    if (n == 0)
    {
        cout << "(empty)";
        return;
    }
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
}

// prints the array with a bar between the active heap (before the bar) and
// the sorted suffix (after the bar)
void printWithBoundary(const int arr[], int n, int activeSize)
{
    for (int i = 0; i < n; i++)
    {
        if (i == activeSize)
        {
            cout << "| ";
        }
        cout << arr[i] << " ";
    }
    if (activeSize == n)
    {
        cout << "|";
    }
}

// sorts arr in ascending order and prints every step
void heapSort(int arr[], int n)
{
    if (n <= 1)
    {
        cout << "  Nothing to sort when n is " << n << endl;
        return;
    }

    buildMaxHeap(arr, n);                 // done once, never again
    cout << "  After building the max heap: ";
    printWithBoundary(arr, n, n);
    cout << endl;

    int step = 1;
    for (int last = n - 1; last >= 1; last--)
    {
        // the root is the biggest value of the active heap, so it goes to the
        // last place of the active heap, which is the start of the sorted part
        cout << "  Step " << step << ": swap root " << arr[0]
             << " with arr[" << last << "] = " << arr[last] << endl;
        swapInts(arr[0], arr[last]);

        cout << "    after exchange : ";
        printWithBoundary(arr, n, last);
        cout << endl;

        // the active heap now has `last` elements, the sorted suffix is not
        // touched again, and only the root can be out of place
        maxHeapify(arr, last, 0);

        cout << "    after repair   : ";
        printWithBoundary(arr, n, last);
        cout << endl;
        step++;
    }
}

bool isSorted(const int arr[], int n)
{
    for (int i = 0; i + 1 < n; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            return false;
        }
    }
    return true;
}

// runs one test on a fresh copy of the array
void runTest(const char* title, const int original[], int n)
{
    int arr[MAX];
    for (int i = 0; i < n; i++)
    {
        arr[i] = original[i];
    }

    cout << endl << title << endl;
    cout << "Before: ";
    printArray(arr, n);
    cout << endl;

    heapSort(arr, n);

    cout << "After : ";
    printArray(arr, n);
    cout << endl;
    cout << "Sorted ascending: " << (isSorted(arr, n) ? "yes" : "NO") << endl;
}

int main()
{
    // first two iterations of the manual array, traced by hand:
    // heap after build   : 51 35 43 8 12 27 19
    // step 1 exchange    : 19 35 43 8 12 27 | 51   repair: 43 35 27 8 12 19 | 51
    // step 2 exchange    : 19 35 27 8 12 | 43 51   repair: 35 19 27 8 12 | 43 51
    int manual[] = {35, 12, 43, 8, 51, 27, 19};
    runTest("TEST 1: array from the manual", manual, 7);

    int sorted[] = {1, 2, 3, 4, 5, 6, 7};
    runTest("TEST 2: already sorted array", sorted, 7);

    int reversed[] = {7, 6, 5, 4, 3, 2, 1};
    runTest("TEST 3: reverse sorted array", reversed, 7);

    int repeated[] = {4, 2, 4, 2, 4, 1, 1};
    runTest("TEST 4: repeated values", repeated, 7);

    int empty[1] = {0};
    runTest("TEST 5: empty array (n = 0)", empty, 0);

    int single[] = {42};
    runTest("TEST 6: single element (n = 1)", single, 1);

    return 0;
}
