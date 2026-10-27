#include "sorts.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace {

void fail(const char *name, const char *msg)
{
    std::cerr << name << ": " << msg << "\n";
    std::exit(1);
}

template <typename Stepper>
void runFixedDemo(const char *name)
{
    std::vector<int> data = sorts::demoData();
    const std::vector<int> original = data;
    Stepper stepper;
    stepper.reset();

    const int kMaxSteps = 100000;
    int steps = 0;
    for (;;)
    {
        const sorts::StepResult r = stepper.step(data);
        ++steps;
        if (r.done)
        {
            break;
        }
        if (steps > kMaxSteps)
        {
            fail(name, "exceeded max steps");
        }
    }

    if (!sorts::isSorted(data))
    {
        fail(name, "not sorted");
    }
    if (data.size() != original.size())
    {
        fail(name, "size changed");
    }

    std::vector<int> a = original;
    std::vector<int> b = data;
    std::sort(a.begin(), a.end());
    if (a != b)
    {
        fail(name, "values changed");
    }

    std::cout << name << ": ok (" << steps << " steps)\n";
}

} // namespace

int main()
{
    runFixedDemo<sorts::BubbleSort>("bubble");
    runFixedDemo<sorts::QuickSort>("quick");
    runFixedDemo<sorts::MergeSort>("merge");
    runFixedDemo<sorts::InsertionSort>("insertion");
    runFixedDemo<sorts::SelectionSort>("selection");
    std::cout << "all sort stepper tests passed\n";
    return 0;
}
