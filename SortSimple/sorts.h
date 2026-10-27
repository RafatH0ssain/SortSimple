#ifndef SORTS_H
#define SORTS_H

#include <utility>
#include <vector>

// Pure sort steppers used by the UI and by tests. No Qt types here.
namespace sorts {

std::vector<int> demoData();
bool isSorted(const std::vector<int> &data);

struct StepResult {
    bool done = false;
    bool swapped = false;
    int hi1 = -1;
    int hi2 = -1;
};

class BubbleSort {
public:
    void reset();
    StepResult step(std::vector<int> &data);

private:
    int i = 0;
    int j = 0;
    bool swapped = false;
    bool finished = false;
};

class QuickSort {
public:
    void reset();
    StepResult step(std::vector<int> &data);

private:
    std::vector<std::pair<int, int>> stack;
    int left = -1;
    int right = -1;
    int pivotIndex = -1;
    int start = -1;
    int end = -1;
    bool started = false;
    bool finished = false;
};

class MergeSort {
public:
    void reset();
    StepResult step(std::vector<int> &data);

private:
    struct Job {
        bool isMerge;
        int start;
        int mid;
        int end;
    };
    std::vector<Job> jobs;
    bool started = false;
    bool finished = false;
};

class InsertionSort {
public:
    void reset();
    StepResult step(std::vector<int> &data);

private:
    int i = 1;
    int j = -1;
    int key = -1;
    bool finished = false;
};

class SelectionSort {
public:
    void reset();
    StepResult step(std::vector<int> &data);

private:
    int i = 0;
    int minIdx = -1;
    int scan = -1;
    bool finished = false;
};

} // namespace sorts

#endif // SORTS_H
