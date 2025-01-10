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
    algorithmSelector->addItem("Quick Sort"); // Placeholder, implement later
    algorithmSelector->addItem("Merge Sort"); // Placeholder, implement later
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
    data = {10, 20, 5, 15, 30, 25, 35};
    QHBoxLayout *barsLayout = new QHBoxLayout;
    for (int value : data) {
        QLabel *bar = new QLabel;
        bar->setStyleSheet("background-color: blue;");
        bar->setFixedSize(30, value * 5); // Bar height proportional to value
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
        animationTimer->start(500); // 500ms delay for each step
    }
    else if (selectedAlgorithm == "Quick Sort"){
        statusLabel->setText("Sorting using Quick Sort...");
        currentIndex = 0;
        animationTimer->start(500); // 500ms delay for each step
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
    else if ((algorithmSelector->currentText() == "Quick Sort")) {
        quickSortStep();
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
    bool swapped = false;
    for (int j = 0; j < data.size() - 1 - currentIndex; ++j) {
        if (data[j] > data[j + 1]) {
            // Highlight the current swap
            bars[j]->setStyleSheet("background-color: red;");
            bars[j + 1]->setStyleSheet("background-color: red;");
            QCoreApplication::processEvents();

            // Swap data
            std::swap(data[j], data[j + 1]);

            // Update visuals
            bars[j]->setFixedHeight(data[j] * 5);
            bars[j + 1]->setFixedHeight(data[j + 1] * 5);

            swapped = true;

            // Restore colors
            bars[j]->setStyleSheet("background-color: blue;");
            bars[j + 1]->setStyleSheet("background-color: blue;");
            QCoreApplication::processEvents();

            break; // Perform one swap per timer step
        }
    }

    if (!swapped) {
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
    }

    ++currentIndex;
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
                // Highlight elements being swapped
                bars[left]->setStyleSheet("background-color: red;");
                bars[right]->setStyleSheet("background-color: red;");
                QCoreApplication::processEvents();

                // Swap elements
                std::swap(data[left], data[right]);

                // Update visuals
                bars[left]->setFixedHeight(data[left] * 5);
                bars[right]->setFixedHeight(data[right] * 5);

                // Restore colors
                bars[left]->setStyleSheet("background-color: blue;");
                bars[right]->setStyleSheet("background-color: blue;");
                QCoreApplication::processEvents();

                ++left; // Increment left pointer
            }
            ++right; // Increment right pointer
        } else {
            // Place pivot in its correct position
            std::swap(data[left], data[pivotIndex]);

            // Update visuals
            bars[left]->setFixedHeight(data[left] * 5);
            bars[pivotIndex]->setFixedHeight(data[pivotIndex] * 5);

            bars[pivotIndex]->setStyleSheet("background-color: blue;");
            bars[left]->setStyleSheet("background-color: green;"); // Final pivot position
            QCoreApplication::processEvents();

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
            bar->setStyleSheet("background-color: green;"); // Final color for sorted bars
        }

        // Reset static variables for future sorting
        stack.clear();
        left = right = pivotIndex = start = end = -1;
    }
}
