// Name    : Farhad Khan
// Reg No  : 549034
// Section : BSCS-15E
// Lab 11, Task 3: A heap based priority queue (computer lab job queue)

// The jobs are kept in a max heap, ordered by priority. A bigger priority
// number means a more important job, so the root is always the next job to run.
//
// Time of the operations (n is the number of jobs waiting):
//   insert : O(log n) in the worst case. The new job is added at the end and
//            can move up at most as many levels as the height of the tree.
//   remove : O(log n) in the worst case. The last job is moved to the root and
//            can sink at most as many levels as the height of the tree.
//   peek   : O(1). The next job is always at index 0, nothing is searched.
//
// Difference from a normal FIFO queue: a FIFO queue serves the job that
// arrived first, no matter how important it is. This queue serves the job with
// the highest priority first, no matter when it arrived. A job that arrives
// late can jump ahead of older jobs with lower priority.

#include <iostream>
#include <iomanip>
using namespace std;

const int CAPACITY = 20;   // the queue can hold at most 20 jobs

struct Job
{
    int id;
    int priority;          // bigger number means higher priority
};

void swapJobs(Job& a, Job& b)
{
    Job temp = a;
    a = b;
    b = temp;
}

// moves the job at index up while its priority is bigger than its parent's.
// Whole Job records are swapped, so an id always stays with its priority.
void siftUp(Job heap[], int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;
        if (heap[index].priority <= heap[parent].priority)
        {
            break;                        // the parent is bigger, the job is in place
        }
        swapJobs(heap[index], heap[parent]);
        index = parent;
    }
}

// adapted from maxHeapify in Task 1: pushes the job at index i down inside a
// heap of n jobs. Child indices are checked against n before they are used.
void heapifyDown(Job heap[], int n, int i)
{
    while (true)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && heap[left].priority > heap[largest].priority)
        {
            largest = left;
        }
        if (right < n && heap[right].priority > heap[largest].priority)
        {
            largest = right;
        }

        if (largest == i)
        {
            return;
        }

        swapJobs(heap[i], heap[largest]);
        i = largest;
    }
}

// adds a job at the end of the heap and moves it up into place.
// Returns false (and changes nothing) if the queue is full.
bool insertJob(Job heap[], int& size, int capacity, Job item)
{
    if (size >= capacity)
    {
        return false;
    }

    heap[size] = item;       // first free place
    siftUp(heap, size);
    size++;
    return true;
}

// copies the highest priority job into item without removing it.
// Returns false if the queue is empty. The queue is never changed.
bool peekJob(const Job heap[], int size, Job& item)
{
    if (size == 0)
    {
        return false;
    }

    item = heap[0];
    return true;
}

// removes the highest priority job and gives it back in item.
// Returns false if the queue is empty.
bool removeJob(Job heap[], int& size, Job& item)
{
    if (size == 0)
    {
        return false;
    }

    item = heap[0];                  // save the root before it is overwritten
    heap[0] = heap[size - 1];        // the last job takes the root's place
    size--;
    if (size > 0)
    {
        heapifyDown(heap, size, 0);  // and sinks to where it belongs
    }
    return true;
}

// ---------------------------------------------------------------- helpers

// true when every job has a priority at least as big as its children's
bool isHeapOrdered(const Job heap[], int size)
{
    for (int i = 1; i < size; i++)
    {
        if (heap[(i - 1) / 2].priority < heap[i].priority)
        {
            return false;
        }
    }
    return true;
}

// shows the jobs in the order they sit in the array
void displayHeap(const Job heap[], int size)
{
    if (size == 0)
    {
        cout << "The queue is empty." << endl;
        return;
    }

    cout << "Heap order (the order inside the array, NOT the service order):" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "  [" << i << "] job ID " << heap[i].id
             << ", priority " << heap[i].priority << endl;
    }
    cout << "Jobs waiting: " << size << endl;
}

// the manual asks for unique ids and unique priorities, so the menu refuses
// a job whose id or priority is already in the queue (no tie rule is needed)
bool alreadyUsed(const Job heap[], int size, Job item)
{
    for (int i = 0; i < size; i++)
    {
        if (heap[i].id == item.id || heap[i].priority == item.priority)
        {
            return true;
        }
    }
    return false;
}

// keeps asking until a number is typed. If the input has ended it returns 0.
int readInt(const char* prompt)
{
    int value = 0;
    cout << prompt;
    while (!(cin >> value))
    {
        if (cin.eof())
        {
            return 0;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "That is not a number, try again: ";
    }
    return value;
}

// ------------------------------------------------------- menu operations

void menuInsert(Job heap[], int& size)
{
    Job item;
    item.id = readInt("Enter job ID: ");
    item.priority = readInt("Enter priority (bigger number = more important): ");

    if (alreadyUsed(heap, size, item))
    {
        cout << "Job ID or priority is already used by a waiting job, job not inserted." << endl;
        return;
    }

    if (insertJob(heap, size, CAPACITY, item))
    {
        cout << "Job " << item.id << " (priority " << item.priority
             << ") inserted. Jobs waiting: " << size << endl;
    }
    else
    {
        cout << "Queue is full (" << CAPACITY << " jobs), job " << item.id
             << " not inserted." << endl;
    }
}

void menuPeek(const Job heap[], int size)
{
    Job item;
    if (peekJob(heap, size, item))
    {
        cout << "Next job: ID " << item.id << ", priority " << item.priority
             << " (not removed, jobs waiting: " << size << ")" << endl;
    }
    else
    {
        cout << "The queue is empty, no job to peek at." << endl;
    }
}

void menuRemove(Job heap[], int& size)
{
    Job item;
    if (removeJob(heap, size, item))
    {
        cout << "Removed job: ID " << item.id << ", priority " << item.priority
             << ". Jobs waiting: " << size << endl;
    }
    else
    {
        cout << "The queue is empty, no job to remove." << endl;
    }
}

// ------------------------------------------------------ built in tests

// runs the whole test sequence of the manual on its own queue, so jobs typed
// in the menu are not touched
void runTests()
{
    Job heap[CAPACITY];
    int size = 0;
    Job item;

    cout << endl << "STEP 1: operations on an empty queue" << endl;
    cout << "peekJob returns " << (peekJob(heap, size, item) ? "true" : "false") << endl;
    cout << "removeJob returns " << (removeJob(heap, size, item) ? "true" : "false") << endl;
    displayHeap(heap, size);

    cout << endl << "STEP 2: insert (101, 3), (102, 5), (103, 1), (104, 4)" << endl;
    Job first[] = {{101, 3}, {102, 5}, {103, 1}, {104, 4}};
    for (int i = 0; i < 4; i++)
    {
        insertJob(heap, size, CAPACITY, first[i]);
        cout << "  inserted job " << first[i].id << " (priority " << first[i].priority << ")" << endl;
    }
    // hand trace: 101 stays at the root, 102 (5) moves above it, 103 (1) stays
    // at the bottom, 104 (4) moves above 101 but not above 102
    // heap order expected: 102 104 103 101
    displayHeap(heap, size);
    cout << "Heap property holds: " << (isHeapOrdered(heap, size) ? "yes" : "NO") << endl;

    cout << endl << "STEP 3: peek, the queue must not change" << endl;
    int sizeBefore = size;
    peekJob(heap, size, item);
    cout << "Peek gives job " << item.id << " with priority " << item.priority << endl;
    cout << "Size before peek: " << sizeBefore << ", size after peek: " << size << endl;
    cout << "Size unchanged: " << (sizeBefore == size ? "yes" : "NO") << endl;

    cout << endl << "STEP 4: remove one job" << endl;
    removeJob(heap, size, item);
    cout << "Removed job " << item.id << " with priority " << item.priority
         << " (expected job 102, priority 5)" << endl;
    displayHeap(heap, size);

    cout << endl << "STEP 5: insert (105, 6), the new job has the highest priority" << endl;
    Job late = {105, 6};
    insertJob(heap, size, CAPACITY, late);
    displayHeap(heap, size);             // job 105 moves all the way to the root

    cout << endl << "STEP 6: remove all remaining jobs and check the service order" << endl;
    int expectedId[] = {105, 104, 101, 103};
    int expectedPriority[] = {6, 4, 3, 1};
    bool orderOk = true;
    int count = 0;
    while (removeJob(heap, size, item))
    {
        cout << "  served job " << item.id << " (priority " << item.priority << ")" << endl;
        if (count >= 4 || item.id != expectedId[count] || item.priority != expectedPriority[count])
        {
            orderOk = false;             // wrong job, or the id lost its priority
        }
        count++;
    }
    if (count != 4)
    {
        orderOk = false;
    }
    cout << "Service order is 105, 104, 101, 103 with matching priorities: "
         << (orderOk ? "yes" : "NO") << endl;

    cout << endl << "STEP 7: empty queue again" << endl;
    cout << "removeJob returns " << (removeJob(heap, size, item) ? "true" : "false") << endl;
    cout << "peekJob returns " << (peekJob(heap, size, item) ? "true" : "false") << endl;
    cout << "Size is still " << size << endl;

    cout << endl << "STEP 8: fill the queue to its capacity of " << CAPACITY
         << " and try one more insertion" << endl;
    for (int i = 0; i < CAPACITY; i++)
    {
        Job j;
        j.id = 201 + i;
        j.priority = (i * 7) % 20 + 1;   // 7 and 20 share no factor, so these are 1 to 20, all different
        insertJob(heap, size, CAPACITY, j);
    }
    cout << "Jobs waiting: " << size << endl;
    Job extra = {221, 99};
    bool added = insertJob(heap, size, CAPACITY, extra);
    cout << "insertJob for job 221 returns " << (added ? "true" : "false")
         << ", jobs waiting: " << size << endl;
    peekJob(heap, size, item);
    cout << "Next job is still priority " << item.priority << " (expected 20)" << endl;
    cout << "Heap property holds: " << (isHeapOrdered(heap, size) ? "yes" : "NO") << endl;

    cout << endl << "Built-in tests finished." << endl;
}

int main()
{
    Job heap[CAPACITY];
    int size = 0;
    int choice = 0;

    do
    {
        cout << endl;
        cout << "----- Job Priority Queue -----" << endl;
        cout << "1. Insert a job" << endl;
        cout << "2. Peek at the next job" << endl;
        cout << "3. Remove the next job" << endl;
        cout << "4. Display the queue (heap order)" << endl;
        cout << "5. Run the built-in test sequence" << endl;
        cout << "6. Exit" << endl;
        choice = readInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            menuInsert(heap, size);
            break;
        case 2:
            menuPeek(heap, size);
            break;
        case 3:
            menuRemove(heap, size);
            break;
        case 4:
            displayHeap(heap, size);
            break;
        case 5:
            runTests();
            break;
        case 6:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice, please enter a number from 1 to 6." << endl;
        }
    } while (choice != 6 && !cin.eof());

    return 0;
}
