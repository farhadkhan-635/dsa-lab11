// Name    : Farhad Khan
// Reg No  : 549034
// Section : BSCS-15E
// Lab 11, Task 5: Selection sort and comparison with bubble sort

// Why selection sort still scans sorted input: it has no way to know that the
// rest of the array is already in order. To be sure of the minimum it must
// look at every element of the unsorted part, so it always makes
// n * (n - 1) / 2 comparisons, whatever the input looks like.
//
// Why it makes at most n - 1 swaps: each pass places one value into its final
// position with at most one swap, and there are n - 1 passes. A swap is done
// only when the minimum is not already in place.
//
// Only comparisons between two array elements are counted.
//
// Viva notes:
//   Worst case time : heap sort O(n log n), bubble sort O(n^2), selection sort O(n^2).
//   Stability       : bubble sort is stable (equal values are never swapped).
//                     Selection sort and heap sort are not stable, because a
//                     swap can jump an equal value over another one. The demo
//                     at the end of main shows this with equal-key records.
//   Extra space     : my maxHeapify uses a loop, so it needs only a few
//                     variables, O(1). A recursive heapify would need O(log n)
//                     for the call stack.

#include <iostream>
#include <iomanip>
using namespace std;

const int MAX = 20;   // largest array used in the tests

// counts of the last sort that was run
int compareCount = 0;
int swapCount = 0;
int passCount = 0;
bool showPasses = true;   // false while the comparison table is filled quietly

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

void printPass(int pass, const int arr[], int n)
{
    if (showPasses)
    {
        cout << "  After pass " << pass << ": ";
        printArray(arr, n);
        cout << endl;
    }
}

void printCounts()
{
    if (showPasses)
    {
        cout << "  Comparisons: " << compareCount << ", swaps: " << swapCount
             << ", passes: " << passCount << endl;
    }
}

// ------------------------------------------------------- selection sort

// ascending selection sort. Pass i finds the smallest value in the unsorted
// part (index i to the end) and swaps it into place i.
void selectionSort(int arr[], int n)
{
    compareCount = 0;
    swapCount = 0;
    passCount = 0;

    for (int i = 0; i < n - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            compareCount++;
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }

        if (minIndex != i)                   // swap only when the minimum is elsewhere
        {
            swapInts(arr[i], arr[minIndex]);
            swapCount++;
        }
        passCount++;
        printPass(i + 1, arr, n);
    }

    printCounts();
}

// ------------------------------------- bubble sorts (from Task 4), for the table

void bubbleSort(int arr[], int n)
{
    compareCount = 0;
    swapCount = 0;
    passCount = 0;

    for (int pass = 1; pass <= n - 1; pass++)
    {
        for (int j = 0; j < n - pass; j++)
        {
            compareCount++;
            if (arr[j] > arr[j + 1])
            {
                swapInts(arr[j], arr[j + 1]);
                swapCount++;
            }
        }
        passCount++;
        printPass(pass, arr, n);
    }

    printCounts();
}

void bubbleSortOptimized(int arr[], int n)
{
    compareCount = 0;
    swapCount = 0;
    passCount = 0;

    for (int pass = 1; pass <= n - 1; pass++)
    {
        bool swapped = false;
        for (int j = 0; j < n - pass; j++)
        {
            compareCount++;
            if (arr[j] > arr[j + 1])
            {
                swapInts(arr[j], arr[j + 1]);
                swapCount++;
                swapped = true;
            }
        }
        passCount++;
        printPass(pass, arr, n);

        if (!swapped)
        {
            break;                           // a pass without swaps means sorted
        }
    }

    printCounts();
}

// ------------------------------------------------------------------ tests

// runs selection sort on a fresh copy of the array
void runSelection(const char* title, const int original[], int n)
{
    int arr[MAX];
    for (int i = 0; i < n; i++)
    {
        arr[i] = original[i];
    }

    cout << endl << title << endl;
    cout << "Original: ";
    printArray(arr, n);
    cout << endl;

    selectionSort(arr, n);

    cout << "  Result: ";
    printArray(arr, n);
    cout << endl << "  Sorted correctly: " << (isSorted(arr, n) ? "yes" : "NO") << endl;
}

// runs the three sorts quietly on fresh copies and prints one row of the table
void tableRow(const char* name, const int original[], int n)
{
    int arr[MAX];
    int bc, bs, oc, os, sc, ss;

    showPasses = false;

    for (int i = 0; i < n; i++) { arr[i] = original[i]; }
    bubbleSort(arr, n);
    bc = compareCount;
    bs = swapCount;

    for (int i = 0; i < n; i++) { arr[i] = original[i]; }
    bubbleSortOptimized(arr, n);
    oc = compareCount;
    os = swapCount;

    for (int i = 0; i < n; i++) { arr[i] = original[i]; }
    selectionSort(arr, n);
    sc = compareCount;
    ss = swapCount;

    showPasses = true;

    cout << left << setw(24) << name
         << setw(8) << bc << setw(8) << bs
         << setw(8) << oc << setw(8) << os
         << setw(8) << sc << setw(8) << ss << endl;
}

// ------------------------------------------------- stability demonstration

// a record with a key to sort by and a tag to tell equal keys apart
struct Item
{
    int key;
    char tag;
};

void swapItems(Item& a, Item& b)
{
    Item temp = a;
    a = b;
    b = temp;
}

void printItems(const Item items[], int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << items[i].key << items[i].tag << " ";
    }
}

// bubble sort on records, compares keys only
void bubbleSortItems(Item items[], int n)
{
    for (int pass = 1; pass <= n - 1; pass++)
    {
        for (int j = 0; j < n - pass; j++)
        {
            if (items[j].key > items[j + 1].key)
            {
                swapItems(items[j], items[j + 1]);
            }
        }
    }
}

// selection sort on records
void selectionSortItems(Item items[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (items[j].key < items[minIndex].key)
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            swapItems(items[i], items[minIndex]);
        }
    }
}

// heap sort on records, same steps as Task 2
void heapifyItems(Item items[], int n, int i)
{
    while (true)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && items[left].key > items[largest].key)
        {
            largest = left;
        }
        if (right < n && items[right].key > items[largest].key)
        {
            largest = right;
        }
        if (largest == i)
        {
            return;
        }
        swapItems(items[i], items[largest]);
        i = largest;
    }
}

void heapSortItems(Item items[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapifyItems(items, n, i);
    }
    for (int last = n - 1; last >= 1; last--)
    {
        swapItems(items[0], items[last]);
        heapifyItems(items, last, 0);
    }
}

// stable means the equal keys 2a and 2b are still in the order a then b
bool keepsOrder(const Item items[], int n)
{
    int posA = -1;
    int posB = -1;
    for (int i = 0; i < n; i++)
    {
        if (items[i].tag == 'a')
        {
            posA = i;
        }
        if (items[i].tag == 'b')
        {
            posB = i;
        }
    }
    return posA < posB;
}

void stabilityLine(const char* name, void (*sortFunction)(Item[], int))
{
    Item items[3] = {{2, 'a'}, {2, 'b'}, {1, 'c'}};   // 2a comes before 2b
    sortFunction(items, 3);
    cout << left << setw(16) << name << "result: ";
    printItems(items, 3);
    cout << "  equal keys keep their order: " << (keepsOrder(items, 3) ? "yes (stable)" : "no (not stable)") << endl;
}

int main()
{
    // ----- part 1: selection sort on the same arrays as Task 4
    // hand trace of the first array: pass 1 swaps 1 into place 0 (1 5 4 2 8),
    // pass 2 swaps 2 into place 1 (1 2 4 5 8), passes 3 and 4 find the minimum
    // already in place, so there are 2 swaps and 10 comparisons.
    int a[] = {5, 1, 4, 2, 8};
    runSelection("TEST 1: {5, 1, 4, 2, 8}", a, 5);

    int b[] = {1, 2, 4, 5, 8};
    runSelection("TEST 2: {1, 2, 4, 5, 8} (already sorted)", b, 5);

    int c[] = {8, 5, 4, 2, 1};
    runSelection("TEST 3: {8, 5, 4, 2, 1} (reverse sorted)", c, 5);

    int repeated[] = {3, 1, 3, 2, 1};
    runSelection("TEST 4: repeated values {3, 1, 3, 2, 1}", repeated, 5);

    int empty[1] = {0};
    runSelection("TEST 5: empty array (n = 0)", empty, 0);

    int single[] = {7};
    runSelection("TEST 6: single element (n = 1)", single, 1);

    // ----- part 2: counts for all three implementations
    cout << endl << "COMPARISON OF COUNTS (comparisons / swaps)" << endl;
    cout << left << setw(24) << "Input"
         << setw(16) << "Bubble" << setw(16) << "Optimized" << setw(16) << "Selection" << endl;
    cout << left << setw(24) << ""
         << setw(8) << "comp" << setw(8) << "swaps"
         << setw(8) << "comp" << setw(8) << "swaps"
         << setw(8) << "comp" << setw(8) << "swaps" << endl;
    tableRow("{5, 1, 4, 2, 8}", a, 5);
    tableRow("{1, 2, 4, 5, 8}", b, 5);
    tableRow("{8, 5, 4, 2, 1}", c, 5);

    cout << endl << "What the table shows:" << endl;
    cout << "  Selection sort always makes 10 comparisons for 5 elements, even on sorted input." << endl;
    cout << "  Selection sort never makes more than n - 1 = 4 swaps, bubble sort can make 10." << endl;
    cout << "  Optimized bubble sort needs only 4 comparisons on sorted input." << endl;

    // ----- part 3: stability with equal-key records
    cout << endl << "STABILITY: sorting 2a, 2b, 1c by key (2a was before 2b at the start)" << endl;
    stabilityLine("Bubble sort", bubbleSortItems);
    stabilityLine("Selection sort", selectionSortItems);
    stabilityLine("Heap sort", heapSortItems);

    return 0;
}
