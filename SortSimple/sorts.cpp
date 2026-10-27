#include "sorts.h"

#include <algorithm>

namespace sorts {

std::vector<int> demoData()
{
    return {23, 41, 25, 54, 18, 14, 9, 10};
}

bool isSorted(const std::vector<int> &data)
{
    for (size_t i = 0; i + 1 < data.size(); ++i)
    {
        if (data[i] > data[i + 1])
        {
            return false;
        }
    }
    return true;
}

void BubbleSort::reset()
{
    i = 0;
    j = 0;
    swapped = false;
    finished = false;
}

StepResult BubbleSort::step(std::vector<int> &data)
{
    const int n = static_cast<int>(data.size());
    StepResult r;
    if (n <= 1)
    {
        finished = true;
        r.done = true;
        return r;
    }
    if (i >= n - 1)
    {
        finished = true;
        r.done = true;
        return r;
    }

    if (j < n - 1 - i)
    {
        r.hi1 = j;
        r.hi2 = j + 1;
        if (data[j] > data[j + 1])
        {
            std::swap(data[j], data[j + 1]);
            swapped = true;
            r.swapped = true;
        }
        ++j;
        return r;
    }

    if (!swapped)
    {
        finished = true;
        r.done = true;
        return r;
    }
    ++i;
    j = 0;
    swapped = false;
    return r;
}

void QuickSort::reset()
{
    stack.clear();
    left = right = pivotIndex = start = end = -1;
    started = false;
    finished = false;
}

StepResult QuickSort::step(std::vector<int> &data)
{
    const int n = static_cast<int>(data.size());
    StepResult r;
    if (n <= 1)
    {
        finished = true;
        r.done = true;
        return r;
    }

    if (pivotIndex == -1)
    {
        if (stack.empty())
        {
            if (started)
            {
                finished = true;
                r.done = true;
                return r;
            }
            stack.push_back({0, n - 1});
            started = true;
        }
        const auto range = stack.back();
        stack.pop_back();
        start = range.first;
        end = range.second;
        if (start < end)
        {
            pivotIndex = end;
            left = start;
            right = start;
            r.hi1 = pivotIndex;
        }
        return r;
    }

    if (right < pivotIndex)
    {
        r.hi1 = left;
        r.hi2 = right;
        if (data[right] <= data[pivotIndex])
        {
            std::swap(data[left], data[right]);
            r.swapped = true;
            ++left;
        }
        ++right;
        return r;
    }

    std::swap(data[left], data[pivotIndex]);
    r.hi1 = left;
    r.hi2 = pivotIndex;
    r.swapped = (left != pivotIndex);
    const int p = left;
    if (start < p - 1)
    {
        stack.push_back({start, p - 1});
    }
    if (p + 1 < end)
    {
        stack.push_back({p + 1, end});
    }
    pivotIndex = left = right = start = end = -1;
    return r;
}

void MergeSort::reset()
{
    jobs.clear();
    started = false;
    finished = false;
}

StepResult MergeSort::step(std::vector<int> &data)
{
    const int n = static_cast<int>(data.size());
    StepResult r;
    if (n <= 1)
    {
        finished = true;
        r.done = true;
        return r;
    }

    if (jobs.empty())
    {
        if (started)
        {
            finished = true;
            r.done = true;
            return r;
        }
        jobs.push_back({false, 0, -1, n - 1});
        started = true;
        return r;
    }

    const Job job = jobs.back();
    jobs.pop_back();

    if (!job.isMerge)
    {
        const int s = job.start;
        const int e = job.end;
        r.hi1 = s;
        r.hi2 = e;
        if (s < e)
        {
            const int m = s + (e - s) / 2;
            jobs.push_back({true, s, m, e});
            jobs.push_back({false, m + 1, -1, e});
            jobs.push_back({false, s, -1, m});
        }
        return r;
    }

    const int s = job.start;
    const int m = job.mid;
    const int e = job.end;
    r.hi1 = s;
    r.hi2 = e;
    r.swapped = true;

    std::vector<int> tmp(data.begin() + s, data.begin() + e + 1);
    int i = 0;
    int j = m - s + 1;
    int k = s;
    while (i <= m - s && j <= e - s)
    {
        if (tmp[i] <= tmp[j])
        {
            data[k++] = tmp[i++];
        }
        else
        {
            data[k++] = tmp[j++];
        }
    }
    while (i <= m - s)
    {
        data[k++] = tmp[i++];
    }
    while (j <= e - s)
    {
        data[k++] = tmp[j++];
    }
    return r;
}

void InsertionSort::reset()
{
    i = 1;
    j = -1;
    key = -1;
    finished = false;
}

StepResult InsertionSort::step(std::vector<int> &data)
{
    const int n = static_cast<int>(data.size());
    StepResult r;
    if (n <= 1)
    {
        finished = true;
        r.done = true;
        return r;
    }
    if (i >= n)
    {
        finished = true;
        r.done = true;
        return r;
    }

    if (key == -1 && j == -1)
    {
        key = data[i];
        j = i - 1;
    }

    if (j >= 0 && data[j] > key)
    {
        r.hi1 = j;
        r.hi2 = j + 1;
        r.swapped = true;
        data[j + 1] = data[j];
        --j;
        return r;
    }

    data[j + 1] = key;
    r.hi1 = j + 1;
    ++i;
    key = -1;
    j = -1;
    return r;
}

void SelectionSort::reset()
{
    i = 0;
    minIdx = -1;
    scan = -1;
    finished = false;
}

StepResult SelectionSort::step(std::vector<int> &data)
{
    const int n = static_cast<int>(data.size());
    StepResult r;
    if (n <= 1)
    {
        finished = true;
        r.done = true;
        return r;
    }
    if (i >= n - 1)
    {
        finished = true;
        r.done = true;
        return r;
    }

    if (minIdx == -1)
    {
        minIdx = i;
        scan = i + 1;
    }

    if (scan < n)
    {
        r.hi1 = i;
        r.hi2 = scan;
        if (data[scan] < data[minIdx])
        {
            minIdx = scan;
        }
        ++scan;
        return r;
    }

    if (minIdx != i)
    {
        std::swap(data[i], data[minIdx]);
        r.swapped = true;
        r.hi1 = i;
        r.hi2 = minIdx;
    }
    ++i;
    minIdx = -1;
    scan = -1;
    return r;
}

} // namespace sorts
