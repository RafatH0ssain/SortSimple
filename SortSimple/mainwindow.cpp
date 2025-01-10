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
        animationTimer->start(2000);
    }
    else if (selectedAlgorithm == "Quick Sort"){
        statusLabel->setText("Sorting using Quick Sort...");
        currentIndex = 0;
        animationTimer->start(2000);
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
    else if (algorithmSelector->currentText() == "Quick Sort") {
        quickSortStep();
    }
}

void MainWindow::bubbleSortStep() {
    bool swapped = false;

    // Traverse through all array elements
    for (int j = 0; j < data.size() - 1 - currentIndex; ++j) {
        if (data[j] > data[j + 1]) {
            // Highlight the current swap
            bars[j]->setStyleSheet("background-color: red;");
            bars[j]->repaint();
            bars[j + 1]->setStyleSheet("background-color: red;");
            bars[j + 1]->repaint();
            QCoreApplication::processEvents();

            // Swap data
            std::swap(data[j], data[j + 1]);

            swapped = true;

            // Update visuals after a small delay
            QTimer::singleShot(1000, this, [this, j] {
                bars[j]->setText(QString::number(data[j]));
                bars[j + 1]->setText(QString::number(data[j + 1]));
                QCoreApplication::processEvents();
            });

            // Restore original color after a small delay
            QTimer::singleShot(1000, this, [this, j] {
                bars[j]->setStyleSheet("background-color: blue;");
                bars[j + 1]->setStyleSheet("background-color: blue;");
                QCoreApplication::processEvents();
            });
        }
    }

    if (!swapped) {
        // If no swaps were made, the sorting is complete
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
    }

    // Increment currentIndex to reduce the range for the next pass
    ++currentIndex;
}

void MainWindow::quickSortStep() {
    static QVector<QPair<int, int>> stack; // To store sub-array ranges
    static int pivotIndex = -1, left = -1, right = -1;

    // Initialize the stack with the full range during the first call
    if (stack.isEmpty()) {
        stack.push_back({0, data.size() - 1});
        pivotIndex = left = right = -1; // Reset indices
    }

    // If stack is empty, sorting is complete
    if (stack.isEmpty()) {
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
        return;
    }

    // If no current partitioning step is active, set up a new range
    if (pivotIndex == -1 && !stack.isEmpty()) {
        auto range = stack.takeLast();
        int start = range.first, end = range.second;

        if (start < end) {
            pivotIndex = end;  // Choose pivot (last element)
            left = start;
            right = start;
            bars[pivotIndex]->setStyleSheet("background-color: green;"); // Highlight pivot
        } else {
            pivotIndex = left = right = -1; // Reset for next range
        }
        QCoreApplication::processEvents();
    }

    // Perform a single step of partitioning
    if (pivotIndex != -1) {
        if (right < pivotIndex) {
            if (data[right] < data[pivotIndex]) {
                // Highlight elements being swapped
                bars[left]->setStyleSheet("background-color: red;");
                bars[right]->setStyleSheet("background-color: red;");
                QCoreApplication::processEvents();

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

            // Add new sub-ranges to stack
            stack.push_back({stack.size() > 0 ? stack.last().first : 0, left - 1});
            stack.push_back({left + 1, pivotIndex - 1});

            // Reset for next partitioning
            pivotIndex = left = right = -1;
        }
    }
}

