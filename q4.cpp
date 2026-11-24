// Name    : Farhad Khan
// Reg No  : 549034
// Section : BSCS-15E
// Lab 11, Task 4: Bubble sort (normal and optimized)

// Why the optimized version needs only one pass on sorted input: in a pass we
// compare every neighbouring pair. If the array is already sorted no pair is
// out of order, so no swap happens and the swapped flag stays false. A pass
// without a single swap means every pair is in order, so the whole array is
// sorted and we can stop at once. The normal version has no flag, so it keeps
// doing the remaining passes even though nothing can change any more.
//
// Only comparisons between two array elements are counted, not the loop
// conditions or the swapped flag.

#include <iostream>
using namespace std;

const int MAX = 20;   // largest array used in the tests

void swapInts(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
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

// normal bubble sort, ascending.
// There are n - 1 passes. After pass k the k biggest values are in their final
// places at the end, so the unsorted part shrinks by one every pass and the
// inner loop gets shorter.
void bubbleSort(int arr[], int n)
{
    int comparisons = 0;
    int swaps = 0;
    int passes = 0;

    for (int pass = 1; pass <= n - 1; pass++)
    {
        for (int j = 0; j < n - pass; j++)
        {
            comparisons++;
            if (arr[j] > arr[j + 1])         // equal values are never swapped
            {
                swapInts(arr[j], arr[j + 1]);
                swaps++;
            }
        }
        passes++;
        cout << "  After pass " << pass << ": ";
        printArray(arr, n);
        cout << endl;
    }

    cout << "  Comparisons: " << comparisons << ", swaps: " << swaps
         << ", passes: " << passes << endl;
}

// optimized bubble sort: the same, but it stops after a pass with no swaps
void bubbleSortOptimized(int arr[], int n)
{
    int comparisons = 0;
    int swaps = 0;
    int passes = 0;

    for (int pass = 1; pass <= n - 1; pass++)
    {
        bool swapped = false;                // did this pass change anything?

        for (int j = 0; j < n - pass; j++)
        {
            comparisons++;
            if (arr[j] > arr[j + 1])
            {
                swapInts(arr[j], arr[j + 1]);
                swaps++;
                swapped = true;
            }
        }
        passes++;
        cout << "  After pass " << pass << ": ";
        printArray(arr, n);
        cout << endl;

        if (!swapped)
        {
            cout << "  No swaps in this pass, the array is sorted, stopping early" << endl;
            break;
        }
    }

    cout << "  Comparisons: " << comparisons << ", swaps: " << swaps
         << ", passes: " << passes << endl;
}

// runs both versions, each on a fresh copy of the original array
void runBoth(const char* title, const int original[], int n)
{
    int arr[MAX];

    cout << endl << title << endl;
    cout << "Original: ";
    printArray(original, n);
    cout << endl;

    for (int i = 0; i < n; i++)
    {
        arr[i] = original[i];
    }
    cout << "Normal bubble sort:" << endl;
    bubbleSort(arr, n);
    cout << "  Result: ";
    printArray(arr, n);
    cout << endl << "  Sorted correctly: " << (isSorted(arr, n) ? "yes" : "NO") << endl;

    for (int i = 0; i < n; i++)              // fresh copy, the first sort changed arr
    {
        arr[i] = original[i];
    }
    cout << "Optimized bubble sort:" << endl;
    bubbleSortOptimized(arr, n);
    cout << "  Result: ";
    printArray(arr, n);
    cout << endl << "  Sorted correctly: " << (isSorted(arr, n) ? "yes" : "NO") << endl;
}

int main()
{
    // hand trace of the first array (normal version):
    // pass 1: 1 4 2 5 8 (3 swaps), pass 2: 1 2 4 5 8 (1 swap),
    // pass 3: no swaps, pass 4: no swaps. Normal: 10 comparisons, 4 swaps.
    // Optimized stops after pass 3: 9 comparisons, 4 swaps.
    int a[] = {5, 1, 4, 2, 8};
    runBoth("TEST 1: {5, 1, 4, 2, 8}", a, 5);

    // already sorted: the optimized version needs one pass (4 comparisons)
    int b[] = {1, 2, 4, 5, 8};
    runBoth("TEST 2: {1, 2, 4, 5, 8} (already sorted)", b, 5);

    // reverse order is the worst case: every comparison ends in a swap
    int c[] = {8, 5, 4, 2, 1};
    runBoth("TEST 3: {8, 5, 4, 2, 1} (reverse sorted)", c, 5);

    int repeated[] = {3, 1, 3, 2, 1};
    runBoth("TEST 4: repeated values {3, 1, 3, 2, 1}", repeated, 5);

    int empty[1] = {0};
    runBoth("TEST 5: empty array (n = 0)", empty, 0);

    int single[] = {7};
    runBoth("TEST 6: single element (n = 1)", single, 1);

    return 0;
}
