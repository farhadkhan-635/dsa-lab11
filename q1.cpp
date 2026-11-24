// Name    : Farhad Khan
// Reg No  : 549034
// Section : BSCS-15E
// Lab 11, Task 1: Building a max heap

// A heap is stored in a plain array (zero based). For the node at index i:
//   left child  is at 2 * i + 1
//   right child is at 2 * i + 2
//   parent      is at (i - 1) / 2
// In a max heap every parent is at least as large as its children. The array
// of a heap is not sorted, only the parent and child rule is guaranteed.

#include <iostream>
using namespace std;

const int MAX = 20;   // largest array used in the tests

void swapInts(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// pushes the value at index i down until neither child is larger than it.
// It expects the subtrees below i to be max heaps already. n is the number of
// elements that belong to the heap, and every child index is checked against
// n first so we never read outside the heap.
// A loop is used instead of recursion, so no extra memory is needed.
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
            return;                      // the parent is already the biggest, done
        }

        swapInts(arr[i], arr[largest]);  // move the small value down
        i = largest;                     // and keep checking from its new place
    }
}

// builds a max heap from any array.
// Leaves are already heaps of size one, so we start at the last node that has
// a child, which is n / 2 - 1, and go back to index 0. Going backwards makes
// sure both subtrees of a node are heaps before maxHeapify is called on it.
// For n = 0 or n = 1 the start index is -1, so the loop does not run at all.
void buildMaxHeap(int arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        maxHeapify(arr, n, i);
    }
}

// ---------------------------------------------------------------- helpers

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

// draws the tree turned on its side: root at the left, the right child is
// drawn above its parent and the left child below it
void printHeapTree(const int arr[], int n, int index, int depth)
{
    if (index >= n)
    {
        return;
    }
    printHeapTree(arr, n, 2 * index + 2, depth + 1);
    for (int i = 0; i < depth; i++)
    {
        cout << "      ";
    }
    cout << arr[index] << endl;
    printHeapTree(arr, n, 2 * index + 1, depth + 1);
}

void drawTree(const int arr[], int n)
{
    if (n == 0)
    {
        cout << "(empty heap)" << endl;
        return;
    }
    printHeapTree(arr, n, 0, 0);
}

// checks the parent and child rule at every non leaf node (index 0 up to
// n / 2 - 1). With verbose on, it prints what it found at each node.
bool isMaxHeap(const int arr[], int n, bool verbose)
{
    bool ok = true;
    int lastNonLeaf = n / 2 - 1;

    if (lastNonLeaf < 0 && verbose)
    {
        cout << "  no non-leaf nodes, nothing to check" << endl;
    }

    for (int i = 0; i <= lastNonLeaf; i++)
    {
        int left = 2 * i + 1;        // always inside the array for a non leaf node
        int right = 2 * i + 2;       // may be missing, so it is checked against n
        bool leftOk = arr[i] >= arr[left];
        bool rightOk = (right >= n) || (arr[i] >= arr[right]);

        if (!leftOk || !rightOk)
        {
            ok = false;
        }

        if (verbose)
        {
            cout << "  node " << arr[i] << " (index " << i << "): left child "
                 << arr[left] << (leftOk ? " ok" : " WRONG");
            if (right < n)
            {
                cout << ", right child " << arr[right] << (rightOk ? " ok" : " WRONG");
            }
            else
            {
                cout << ", no right child";
            }
            cout << endl;
        }
    }
    return ok;
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
    cout << "Array before : ";
    printArray(arr, n);
    cout << endl;
    cout << "Tree before (root at the left, right child above, left child below):" << endl;
    drawTree(arr, n);
    cout << "Already a max heap: " << (isMaxHeap(arr, n, false) ? "yes" : "no") << endl;

    buildMaxHeap(arr, n);

    cout << "Array after  : ";
    printArray(arr, n);
    cout << endl;
    cout << "Tree after:" << endl;
    drawTree(arr, n);
    cout << "Checking every non-leaf node:" << endl;
    bool ok = isMaxHeap(arr, n, true);
    cout << "Heap property holds: " << (ok ? "yes" : "NO") << endl;
}

int main()
{
    // the array given in the manual
    // expected result: 51 35 43 8 12 27 19
    // (12 swaps with 51 at index 1, then 35 swaps with 51 at the root and the
    // 35 stays at index 1 because its children 8 and 12 are smaller)
    int manual[] = {35, 12, 43, 8, 51, 27, 19};
    runTest("TEST 1: array from the manual", manual, 7);

    int empty[1] = {0};
    runTest("TEST 2: empty array (n = 0)", empty, 0);

    int single[] = {42};
    runTest("TEST 3: single element", single, 1);

    int same[] = {6, 6, 6, 6, 6};
    runTest("TEST 4: all values equal", same, 5);

    int repeated[] = {5, 9, 5, 9, 1, 9};
    runTest("TEST 5: repeated values", repeated, 6);

    int ascending[] = {1, 2, 3, 4, 5, 6, 7};
    runTest("TEST 6: already sorted ascending (not a max heap)", ascending, 7);

    return 0;
}
