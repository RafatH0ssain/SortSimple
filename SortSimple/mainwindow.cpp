#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QRandomGenerator>
#include <qcoreapplication.h>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    algorithmSelector(new QComboBox(this)),
    startButton(new QPushButton("Start", this)),
    statusLabel(new QLabel("Select an algorithm and start", this)),
    currentIndex(0),
    animationTimer(new QTimer(this)) {

    setupUI();

    // Populate dropdown with sorting algorithms
    algorithmSelector->addItem("Bubble Sort");
    algorithmSelector->addItem("Quick Sort");
    algorithmSelector->addItem("Merge Sort");
    algorithmSelector->addItem("Insertion Sort"); // Placeholder, implement later
    algorithmSelector->addItem("Selection Sort"); // Placeholder, implement later

    // Connect signals to slots
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSorting);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::performStep);
}

// Destructor
MainWindow::~MainWindow() {}

// Set up the UI
void MainWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Dropdown and Start button layout
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(algorithmSelector);
    controlsLayout->addWidget(startButton);

    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(statusLabel);

    // Generate random data for bars
    data = {23, 41, 25, 54, 18, 14};
    QHBoxLayout *barsLayout = new QHBoxLayout;
    for (int value : data) {
        QLabel *bar = new QLabel;
        bar->setStyleSheet("background-color: blue;");
        bar->setFixedSize(80, 80);
        bar->setText(QString::number(value));
        bar->setAlignment(Qt::AlignCenter);
        barsLayout->addWidget(bar);
        bars.push_back(bar);
    }

    mainLayout->addLayout(barsLayout);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

// Start sorting animation
void MainWindow::startSorting() {
    QString selectedAlgorithm = algorithmSelector->currentText();
    if (selectedAlgorithm == "Bubble Sort") {
        statusLabel->setText("Sorting using Bubble Sort...");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Quick Sort"){
        statusLabel->setText("Sorting using Quick Sort...");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Merge Sort"){
        statusLabel->setText("Sorting using Merge Sort...");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else {
        statusLabel->setText("Algorithm not implemented yet.");
    }
}

// Perform a step in the sorting animation
void MainWindow::performStep() {
    if (algorithmSelector->currentText() == "Bubble Sort") {
        bubbleSortStep();
    }
    if ((algorithmSelector->currentText() == "Quick Sort")) {
        quickSortStep();
    }
    if (algorithmSelector->currentText() == "Merge Sort") {
        mergeSortStep();
    }
}

// Check if data is sorted
bool MainWindow::isSorted() {
    for (int i = 0; i < data.size() - 1; ++i) {
        if (data[i] > data[i + 1]) {
            return false; // Not sorted in ascending order
        }
    }
    return true; // Sorted in ascending order
}

void MainWindow::bubbleSortStep() {
    static int i = 0; // Loop variable
    static int j = 0; // Inner loop variable
    static bool swapped = false;

    if (i < data.size()) {
        if (j < data.size() - 1 - i) {
            if (data[j] > data[j + 1]) {
                int currentJ = j; // Capture the value of j

                // Highlight the current swap
                bars[currentJ]->setStyleSheet("background-color: red;");
                bars[currentJ + 1]->setStyleSheet("background-color: red;");
                QCoreApplication::processEvents();

                swapped = true;

                // Swap data after a small delay to visualize the swap
                QTimer::singleShot(700, this, [this, currentJ] {
                    std::swap(data[currentJ], data[currentJ + 1]);

                    // Update visuals after swap
                    bars[currentJ]->setText(QString::number(data[currentJ]));
                    bars[currentJ + 1]->setText(QString::number(data[currentJ + 1]));

                    QTimer::singleShot(300, this, [this, currentJ] {
                        // Restore original color after the swap
                        bars[currentJ]->setStyleSheet("background-color: blue;");
                        bars[currentJ + 1]->setStyleSheet("background-color: blue;");
                        // Continue to the next iteration
                        bubbleSortStep();
                    });
                });
                return; // Exit to allow the timer to trigger the next step
            }
            ++j; // Move to the next pair
            // bubbleSortStep(); // Continue to the next iteration
        } else {
            if (!swapped) {
                // If no swaps were made, the sorting is complete
                animationTimer->stop();
                statusLabel->setText("Sorting complete!");
            }
            ++i; // Move to the next pass
            j = 0; // Reset inner loop variable
            swapped = false; // Reset swap flag
            bubbleSortStep(); // Continue to the next pass
        }
    }
}

void MainWindow::quickSortStep() {
    static QVector<QPair<int, int>> stack; // Stack to store sub-array ranges
    static int left = -1, right = -1, pivotIndex = -1;
    static int start = -1, end = -1; // Track current range explicitly

    // Initialize stack with the full range during the first call
    if (stack.isEmpty()) {
        stack.push_back({0, data.size() - 1});
    }

    // If no current partitioning step is active, set up a new range
    if (pivotIndex == -1 && !stack.isEmpty()) {
        auto range = stack.takeLast();
        start = range.first;
        end = range.second;

        if (start < end) {
            pivotIndex = end; // Choose the last element as pivot
            left = start;
            right = start;
            bars[pivotIndex]->setStyleSheet("background-color: green;"); // Highlight pivot
            QCoreApplication::processEvents();
        } else {
            // Reset and skip invalid ranges
            pivotIndex = -1;
            left = -1;
            right = -1;
        }
    }

    // If partitioning is active, perform a single step
    if (pivotIndex != -1) {
        if (right < pivotIndex) {
            if (data[right] <= data[pivotIndex]) { // Ascending order comparison
                // Swap elements
                std::swap(data[left], data[right]);

                // Update visuals
                bars[left]->setText(QString::number(data[left]));
                bars[right]->setText(QString::number(data[right]));

                // Restore colors after a short delay
                QTimer::singleShot(700, this, [this, left = left, right = right] {
                    bars[left]->setStyleSheet("background-color: blue;");
                    bars[right]->setStyleSheet("background-color: blue;");
                    QCoreApplication::processEvents();
                });

                ++left; // Increment left pointer
            }
            ++right; // Increment right pointer
        } else {
            // Place pivot in its correct position
            std::swap(data[left], data[pivotIndex]);

            // Update visuals
            bars[left]->setText(QString::number(data[left]));
            bars[pivotIndex]->setText(QString::number(data[pivotIndex]));

            // Highlight pivot for a moment before returning to normal color
            QTimer::singleShot(300, this, [this, left = left, pivotIndex = pivotIndex] {
                bars[pivotIndex]->setStyleSheet("background-color: blue;");
                bars[left]->setStyleSheet("background-color: green;"); // Final pivot position
                QCoreApplication::processEvents();
            });

            // Push new sub-ranges to stack
            if ((start < left - 1)) {
                stack.push_back({start, left - 1});
            }
            if ((left + 1 < end)) {
                stack.push_back({left + 1, end});
            }

            // Reset for next partitioning
            pivotIndex = left = right = -1;
        }
    }

    if(isSorted()){
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
        for (QLabel* bar : bars) {
            bar->setStyleSheet("background-color: blue;"); // Final color for sorted bars
        }

        // Reset static variables for future sorting
        stack.clear();
        left = right = pivotIndex = start = end = -1;
    }
}

void MainWindow::mergeSortStep() {
    static QVector<QPair<int, int>> stack;  // Stack to store sub-array ranges
    static QVector<int> tempData;           // Temporary array for merging
    static int start = -1, end = -1, mid = -1;

    // Initialize stack with the full range during the first call
    if (stack.isEmpty()) {
        stack.push_back({0, data.size() - 1});
    }

    // If no current partitioning step is active, set up a new range
    if (start == -1 && !stack.isEmpty()) {
        // Pop the range from the stack
        auto range = stack.takeLast();
        start = range.first;
        end = range.second;

        // Calculate mid-point of the range
        mid = (start + end) / 2;

        // Push new sub-ranges onto the stack for further processing
        if (start < mid) {
            stack.push_back({start, mid});
        }
        if (mid + 1 < end) {
            stack.push_back({mid + 1, end});
        }

        // Animate division of the array (highlight the current sub-array being processed)
        for (int i = start; i <= end; ++i) {
            if (i >= 0 && i < bars.size()) {
                bars[i]->setStyleSheet("background-color: red;");  // Highlight the subarray being processed
            }
        }
        QCoreApplication::processEvents();  // Ensure UI updates

        return;  // Exit to allow the next event loop to execute
    }

    // Perform merge step once the range is split
    if (start <= mid && mid + 1 <= end) {
        int i = start, j = mid + 1, k = start;

        // Prepare tempData for merging
        tempData.clear();
        for (int i = start; i <= end; ++i) {
            tempData.push_back(data[i]);
        }

        // Merge process: merge left and right halves into tempData
        while (i <= mid && j <= end) {
            if (tempData[i] <= tempData[j]) {
                data[k++] = tempData[i++];
            } else {
                data[k++] = tempData[j++];
            }
        }

        // If any remaining elements in the left sub-array, add them
        while (i <= mid) {
            data[k++] = tempData[i++];
        }

        // If any remaining elements in the right sub-array, add them
        while (j <= end) {
            data[k++] = tempData[j++];
        }

        // Update visuals after merging
        for (int i = start; i <= end; ++i) {
            if (i >= 0 && i < bars.size()) {
                bars[i]->setText(QString::number(data[i]));
                bars[i]->setStyleSheet("background-color: blue;");  // Reset the bar color after merge
            }
        }

        QCoreApplication::processEvents();  // Ensure UI updates
    }

    // Check if sorting is complete
    if (isSorted()) {
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");

        // Update all bars to show sorted state
        for (QLabel* bar : bars) {
            bar->setStyleSheet("background-color: green;");
        }

        // Reset static variables for future sorting
        stack.clear();
        start = end = mid = -1;  // Reset the partitioning variables
    }
}
