# DSA Lab 11: Heaps and Sorting Algorithms

Data Structures and Algorithms, Department of Computing

| | |
|---|---|
| Name | Farhad Khan |
| CMS ID | 549034 |
| Class | BSCS-15E |
| Instructor | Sara Tariq Sheikh |
| Lab Engineer | Hassan Mohtashim |
| Lab Date | 24 November 2026 |

## About

This lab uses zero-based arrays to build a max heap, sort with heap sort, run a heap-based priority queue, and compare bubble sort and selection sort by counting operations. Everything is written by hand. `sort`, `make_heap`, `push_heap`, `pop_heap` and `priority_queue` are not used. Each task is one standalone source file with tests in `main()`.

## Files

| Name | Task | What it does |
|---|---|---|
| q1 | Building a max heap | `maxHeapify()` and `buildMaxHeap()` on {35, 12, 43, 8, 51, 27, 19}. Draws the tree before and after, prints the final array (51 35 43 8 12 27 19) and checks the parent and child rule at every non-leaf node. Also tests n = 0, one element, repeated values and a sorted input. |
| q2 | Heap sort | `heapSort()` builds the heap once, then swaps the root with the last active element, shrinks the heap and repairs it. Prints the array after every exchange and every repair with a bar between the active heap and the sorted part. Tests the manual array, sorted, reverse sorted, repeated values, n = 0 and n = 1. |
| q3 | Heap based priority queue | `insertJob()`, `peekJob()` and `removeJob()` on an array of `Job` records with capacity 20. Menu with Insert, Peek, Remove, Display, a built-in test sequence and Exit. Display is labelled heap order, not service order. |
| q4 | Bubble sort | `bubbleSort()` and `bubbleSortOptimized()` print the array after every pass and count comparisons and swaps. Tests {5, 1, 4, 2, 8}, {1, 2, 4, 5, 8}, {8, 5, 4, 2, 1}, repeated values, n = 0 and n = 1. |
| q5 | Selection sort and comparison | `selectionSort()` with counts and per-pass output on the same arrays. Prints a table of comparisons and swaps for bubble, optimized bubble and selection sort, and a stability demonstration with equal-key records for bubble, selection and heap sort. |

Each one is a single source file (q1 is `q1.cpp`, q2 is `q2.cpp`, and so on).

## How to run

Each file is a complete program on its own, so run them one at a time.

**Using an IDE (easiest)**

1. Open the file, for example `q1.cpp`, in your IDE.
2. Press the compile and run button.
   - Dev-C++: F11
   - Code::Blocks: F9
   - Visual Studio: Ctrl + F5
   - VS Code with a C++ extension: the play button at the top right
3. A console window opens. q1, q2, q4 and q5 run their tests on their own. q3 shows a menu, and option 5 runs the full test sequence of the manual.

**Using the terminal (optional)**

```
g++ q1.cpp -o q1
./q1
```

On Windows use `.\q1` for the second line. For the other tasks replace q1 with q2 to q5.

## Results to check

Counts for the three bubble and selection sort test arrays (comparisons and swaps):

| Input | Bubble | Optimized bubble | Selection |
|---|---|---|---|
| {5, 1, 4, 2, 8} | 10 and 4 | 9 and 4 | 10 and 2 |
| {1, 2, 4, 5, 8} | 10 and 0 | 4 and 0 | 10 and 0 |
| {8, 5, 4, 2, 1} | 10 and 10 | 10 and 10 | 10 and 2 |

Priority queue test sequence from the manual: after inserting (101, 3), (102, 5), (103, 1), (104, 4) the heap order is 102, 104, 103, 101. Peek gives job 102 and the size stays 4. Remove gives job 102. After inserting (105, 6) the remaining jobs are served in the order 105, 104, 101, 103. A 21st insertion into a full queue of 20 returns false.

## Sample output

q2, the first two steps of heap sort:

```
After building the max heap: 51 35 43 8 12 27 19 |
Step 1: swap root 51 with arr[6] = 19
  after exchange : 19 35 43 8 12 27 | 51
  after repair   : 43 35 27 8 12 19 | 51
Step 2: swap root 43 with arr[5] = 19
  after exchange : 19 35 27 8 12 | 43 51
  after repair   : 35 19 27 8 12 | 43 51
```

q5, stability with equal keys (2a was before 2b at the start):

```
Bubble sort     result: 1c 2a 2b   equal keys keep their order: yes (stable)
Selection sort  result: 1c 2b 2a   equal keys keep their order: no (not stable)
Heap sort       result: 1c 2b 2a   equal keys keep their order: no (not stable)
```

## Memory safety

- No dynamic memory is used in this lab, all arrays are fixed size and zero-based.
- Every child index in `maxHeapify()` and `heapifyDown()` is checked against the current heap size before it is used, and heap sort never includes the sorted part in later repairs.
- The priority queue refuses insertion into a full queue and peek or removal from an empty queue (they return false) and leaves the queue unchanged.
- All five programs were built with AddressSanitizer and UndefinedBehaviorSanitizer and ran with no errors. The heap, heap sort, priority queue, bubble sort and selection sort were also compared against independent references over thousands of random inputs, including exact comparison and swap counts.

## Concepts covered

- **Heap layout.** For index i the children are at 2i + 1 and 2i + 2, and the parent is at (i - 1) / 2. A max heap needs every parent to be at least as big as its children. The array of a heap is not sorted.
- **Building a heap.** `buildMaxHeap()` starts at the last non-leaf node, n / 2 - 1, and goes back to index 0, so both subtrees of a node are heaps before it is repaired.
- **Why heap sort gives ascending output.** The root is the biggest value. Moving it to the end of the active heap puts it in its final place, and the next biggest becomes the new root.
- **Why the heap is built only once.** After the swap only the root can break the rule, so one repair at index 0 is enough.
- **Time of heap sort.** Bottom-up construction is O(n), because most nodes are near the bottom and sink very little. The n - 1 extractions cost O(log n) each, so the worst case is O(n log n).
- **Priority queue.** Insert is O(log n) (the job moves up), remove is O(log n) (the last job sinks from the root) and peek is O(1). A FIFO queue serves the oldest job first, while this queue serves the most important job first.
- **Bubble sort.** The optimized version stops after a pass with no swaps, so sorted input needs one pass and 4 comparisons for 5 elements.
- **Selection sort.** It always makes n(n - 1) / 2 comparisons, because it cannot know the rest is sorted without looking. It makes at most n - 1 swaps, because each pass swaps at most once.
- **Stability and space.** Bubble sort is stable. Selection sort and heap sort are not, because a swap can jump an equal value over another. Worst-case time is O(n log n) for heap sort and O(n^2) for bubble and selection sort. My `maxHeapify` uses a loop, so its extra space is O(1). A recursive version would need O(log n) for the call stack.

## Reference

Goodrich, M. T., Tamassia, R., & Mount, D. M. (2011). *Data Structures and Algorithms in C++* (2nd ed.). Wiley.
