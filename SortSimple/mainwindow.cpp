#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QRandomGenerator>
#include <qcoreapplication.h>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QResizeEvent>
#include <QFontDatabase>

// Applies CSS styling to the window
void MainWindow::applyStyles() {
    QFile file("styles.css");  // Use the path to your external CSS file
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString css = stream.readAll();
        qApp->setStyleSheet(css);  // Apply the stylesheet to the whole app
    }
    file.close();
}

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    algorithmSelector(new QComboBox(this)),
    startButton(new QPushButton("Start", this)),
    resetButton(new QPushButton("Reset", this)),
    statusLabel(new QLabel("Select an algorithm and start", this)),
    currentIndex(0),
    animationTimer(new QTimer(this)) {

    applyStyles();
    setupUI();

    // Populate dropdown with sorting algorithms
    algorithmSelector->addItem("Bubble Sort");
    algorithmSelector->addItem("Merge Sort");
    algorithmSelector->addItem("Insertion Sort");
    algorithmSelector->addItem("Quick Sort");
    algorithmSelector->addItem("Selection Sort");

    int fontIdAll = QFontDatabase::addApplicationFont("Nasa21-l23X.ttf");
    if (fontIdAll == -1) {
        qWarning() << "Failed to load font!";
        return;
    }
    QString fontFamilyAll = QFontDatabase::applicationFontFamilies(fontIdAll).at(0);
    QFont fontAll(fontFamilyAll);
    algorithmSelector->setFont(fontAll);
    startButton->setFont(fontAll);
    resetButton->setFont(fontAll);
    statusLabel->setFont(fontAll);

    // Connect signals to slots
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSorting);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetSorting);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::performStep);
}

// Destructor
MainWindow::~MainWindow() {}

void MainWindow::resetSorting() {
    // Reset the data to its initial unsorted state
    data = {23, 41, 25, 54, 18, 14, 9, 10};
    resetBool = true;

    // Reset the visualization: all bars back to blue
    for (int i = 0; i < bars.size(); ++i) {
        bars[i]->setText(QString::number(data[i]));  // Restore original data value on each bar
        bars[i]->setStyleSheet("background-color: blue;");  // Reset the color of the bars to blue
    }

    // Stop the timer
    animationTimer->stop();

    int fontIdAll = QFontDatabase::addApplicationFont("Nasa21-l23X.ttf");
    if (fontIdAll == -1) {
        qWarning() << "Failed to load font!";
        return;
    }
    QString fontFamilyAll = QFontDatabase::applicationFontFamilies(fontIdAll).at(0);
    QFont fontAll(fontFamilyAll);

    // Reset the status label
    statusLabel->setText("Select an algorithm and start");
    statusLabel->setFont(fontAll);

    // Reset the algorithm selection (optional)
    algorithmSelector->setCurrentIndex(0);

    // Reset all sorting state variables
    currentIndex = 0;
}

// Set up the UI
void MainWindow::setupUI() {

    // Load the custom font from the resources or file system
    int fontIdAll = QFontDatabase::addApplicationFont("Nasa21-l23X.ttf");
    if (fontIdAll == -1) {
        qWarning() << "Failed to load font!";
        return;
    }

    // Get the font family name
    QString fontFamilyAll = QFontDatabase::applicationFontFamilies(fontIdAll).at(0);

    // Set the font for the whole application
    QFont fontAll(fontFamilyAll);
    qApp->setFont(fontAll);

    int fontIdAcc = QFontDatabase::addApplicationFont("SuperComic-qZg62.ttf");

    if (fontIdAcc == -1) {
        qWarning() << "Failed to load font!";
        return;
    }

    QString fontFamilyAcc = QFontDatabase::applicationFontFamilies(fontIdAcc).at(0);
    QFont fontAcc(fontFamilyAcc);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // All layout
    QWidget *headerContainer = new QWidget(this);
    headerContainer->setObjectName("headerContainer");

    QVBoxLayout *headerLayout = new QVBoxLayout(headerContainer);
    headerContainer->setLayout(headerLayout);
    QLabel *h1 = new QLabel(this);
    QLabel *h2 = new QLabel(this);
    QLabel *p = new QLabel(this);
    h1->setObjectName("h1");
    h2->setObjectName("h2");
    p->setObjectName("p");
    h1->setAlignment(Qt::AlignCenter);
    h2->setAlignment(Qt::AlignCenter);
    p->setAlignment(Qt::AlignCenter);
    h1->setText("SortSimple - Dynamic Sorting Visualization");
    h2->setText("An interactive app that visually demonstrates the step-by-step process of sorting algorithms like Bubble Sort, Merge Sort,\n and Quick Sort, offering a hands-on way to understand algorithmic behavior.");
    p->setText("Project contributors: Nafisah Nubah, Muhammad Rafat Hossain");
    headerLayout->addWidget(h1);
    headerLayout->addWidget(h2);
    headerLayout->addWidget(p);

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(algorithmSelector);
    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(resetButton);

    QHBoxLayout *descriptionLayout = new QHBoxLayout;
    paragraphLabel = new QLabel(this);
    paragraphLabel->setText("<p>This is placeholder text.</p>");
    paragraphLabel->setFont(fontAll);
    paragraphLabel->setObjectName("algoDescription");
    descriptionLayout->addWidget(paragraphLabel);

    mainLayout->addWidget(headerContainer);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(statusLabel);

    // Generate random data for bars
    data = {23, 41, 25, 54, 18, 14, 9, 10};
    QHBoxLayout *barsLayout = new QHBoxLayout;
    barsLayout->setObjectName("barsLayout");

    // Store bars in a container for easy manipulation
    for (int value : data) {
        QLabel *bar = new QLabel;
        bar->setStyleSheet("background-color: blue; color: white; font-weight: thin");
        bar->setText(QString::number(value));
        bar->setFont(fontAcc);
        bar->setAlignment(Qt::AlignCenter);
        barsLayout->addWidget(bar);
        bars.push_back(bar);
    }

    mainLayout->addLayout(barsLayout);
    mainLayout->addLayout(descriptionLayout);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Footer
    QWidget *footerContainer = new QWidget(this);
    footerContainer->setObjectName("footerContainer");

    QVBoxLayout *footerLayout = new QVBoxLayout(footerContainer);
    footerContainer->setLayout(footerLayout);
    QLabel *footerText = new QLabel(this);
    footerText->setObjectName("footerText");
    footerText->setText("© 2025 SortSimple - All rights reserved.");
    footerLayout->addWidget(footerText);

    mainLayout->addWidget(footerContainer);
}

// Resize event to dynamically adjust bar size based on window size
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // Get the width of the window minus the margins (adjust if needed)
    int windowWidth = event->size().width();
    int barCount = bars.size();

    // Calculate the width of each bar based on the window width
    int barWidth = windowWidth / (barCount + 2);

    // Adjust the size of each bar dynamically
    for (int i = 0; i < barCount; ++i) {
        bars[i]->setFixedSize(barWidth, 100);
    }
}

// Start sorting animation
void MainWindow::startSorting() {
    QString selectedAlgorithm = algorithmSelector->currentText();
    resetBool = false;

    if (selectedAlgorithm == "Bubble Sort") {
        statusLabel->setText("Sorting using Bubble Sort...");
        paragraphLabel->setText("<p>Bubble Sort repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. For this array: {23, 41, 25, 54, 18, 14, 9, 10}, the algorithm will take the following steps:</p>"
                                "<p>1. First pass: Compare each adjacent pair and swap if necessary. The algorithm compares 23 and 41, then 41 and 25, swapping these to get {23, 25, 41, 54, 18, 14, 9, 10}. The next swaps will continue through the rest of the array.</p>"
                                "<p>2. Second pass: After the first pass, the largest element (54) has 'bubbled' to the end. The algorithm repeats the process for the remaining unsorted part of the list, gradually moving the next largest element to its correct position.</p>"
                                "<p>3. This process continues until no more swaps are needed, meaning the array is fully sorted. If no swaps are made in a pass, the algorithm stops early, marking the sorting process as complete.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Quick Sort"){
        statusLabel->setText("Sorting using Quick Sort...");
        paragraphLabel->setText("<p>Quick Sort works by selecting a pivot element and partitioning the array around the pivot, recursively sorting the subarrays.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Merge Sort"){
        statusLabel->setText("Sorting using Merge Sort...");
        paragraphLabel->setText("<p>Merge Sort divides the array into two halves, recursively sorts each half, and then merges them back together.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Insertion Sort"){
        statusLabel->setText("Sorting using Insertion Sort...");
        paragraphLabel->setText("<p>Insertion Sort builds the final sorted array one element at a time by repeatedly picking the next element and inserting it into the sorted part of the array.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else {
        statusLabel->setText("Sorting using Selection Sort...");
        paragraphLabel->setText("<p>Selection Sort repeatedly selects the smallest element from the unsorted part of the array and swaps it with the first unsorted element.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
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
    else if (algorithmSelector->currentText() == "Merge Sort") {
        mergeSortStep();
    }
    else if ((algorithmSelector->currentText() == "Insertion Sort")) {
        insertionSortStep();
    }
    else{
        selectionSortStep();
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

    if (resetBool) {
        j = 0;
        i = 0;
        swapped = false;
        int newBars[] = {23, 41, 25, 54, 18, 14, 9, 10};
        swapped = !swapped;
        resetBool = !resetBool;
        for (int i = 0; i < 8; i++) {
            bars[i] -> setText(QString::number(newBars[i]));
        }

    }

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
        } else {
            if (!swapped) {
                // If no swaps were made, the sorting is complete
                animationTimer->stop();
                statusLabel->setText("Sorting complete!");
                i = 0;
                j = 0;

                for (auto* bar : bars) {
                    bar->setStyleSheet("background-color: green;");
                }
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
            bars[pivotIndex]->setStyleSheet("background-color: red;"); // Highlight pivot
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

                // Temporarily color swapped bars red
                bars[left]->setStyleSheet("background-color: red;");
                bars[right]->setStyleSheet("background-color: red;");
                QCoreApplication::processEvents();

                // Restore colors after a short delay
                QTimer::singleShot(1000, this, [this, left = left, right = right] {
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

            // Highlight final pivot position temporarily before resetting color
            bars[left]->setStyleSheet("background-color: red;");
            bars[pivotIndex]->setStyleSheet("background-color: red;");
            QCoreApplication::processEvents();

            QTimer::singleShot(1000, this, [this, left = left, pivotIndex = pivotIndex] {
                bars[left]->setStyleSheet("background-color: blue;");
                bars[pivotIndex]->setStyleSheet("background-color: blue;");
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

        QTimer::singleShot(2000, this, [this] {
            animationTimer->stop();
            statusLabel->setText("Sorting complete!");
            for (QLabel* bar : bars) {
                bar->setStyleSheet("background-color: green;");
            }
        });

        // Reset static variables for future sorting
        stack.clear();
        left = right = pivotIndex = start = end = -1;
    }
}

void MainWindow::mergeSortStep() {
    static QVector<QPair<int, int>> stack;  // Stack to store sub-array ranges
    static QVector<int> tempData;
    static int start = -1, end = -1, mid = -1;

    // Check if sorting is complete
    if (isSorted()) {
        statusLabel->setText("Sorting complete!");

        // Update all bars to show sorted state
        for (QLabel* bar : bars) {
            bar->setStyleSheet("background-color: green;");
        }

        // Reset static variables for future sorting
        stack.clear();
        start = end = mid = -1;  // Reset the partitioning variables
        animationTimer->stop();
    }

    // Initialize stack with the full range during the first call
    if (stack.isEmpty() || resetBool) {
        stack.push_back({0, data.size() - 1});
        resetBool = false;
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
            if (i >= 0 && i < bars.size() && !isSorted()) {
                bars[i]->setStyleSheet("background-color: red;");
            }
        }
        QCoreApplication::processEvents();  // UI update
        return;
    }

    // Perform merge step once the range is split
    if (start <= mid && mid + 1 <= end) {
        int n1 = mid - start + 1;
        int n2 = end - mid;

        // Create temp vectors
        QVector<int> L(n1), R(n2);

        // Copy data to temp vectors L[] and R[]
        for (int i = 0; i < n1; ++i)
            L[i] = data[start + i];
        for (int j = 0; j < n2; ++j)
            R[j] = data[mid + 1 + j];

        int i = 0, j = 0, k = start;

        // Merge the temp vectors back into data[start..end]
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                data[k] = L[i];
                i++;
            } else {
                data[k] = R[j];
                j++;
            }
            k++;
        }

        // Copy the remaining elements of L[], if there are any
        while (i < n1) {
            data[k] = L[i];
            i++;
            k++;
        }

        // Copy the remaining elements of R[], if there are any
        while (j < n2) {
            data[k] = R[j];
            j++;
            k++;
        }

        // Update visuals after merging
        for (int i = start; i <= end; ++i) {
            if (i >= 0 && i < bars.size()) {
                bars[i]->setText(QString::number(data[i]));
                bars[i]->setStyleSheet("background-color: blue;");
            }
        }

        QCoreApplication::processEvents();  // Ensure UI updates

        // Reset indices to process the next range
        start = end = mid = -1;  // Reset after a merge step
        return;
    }
}

void MainWindow::insertionSortStep() {
    static int i = 1;
    static int j = -1;
    static int key = -1;

    // Base case: if sorting is done, stop the timer and set status
    if (i >= data.size()) {
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
        for (QLabel* bar : bars) {
            bar->setStyleSheet("background-color: green;");  // Final color for sorted bars
        }
        return;
    }

    // Initialize key and j for the first time
    if (key == -1 && j == -1) {
        key = data[i];
        j = i - 1;
    }

    // Reset all bars' colors to blue before the new comparison
    for (int k = 0; k < data.size(); ++k) {
        bars[k]->setStyleSheet("background-color: blue;");
    }

    // Highlight the key
    bars[i]->setStyleSheet("background-color: yellow;");

    // Find the correct position for the key in the sorted part of the array
    if (j >= 0 && data[j] > key) {
        // Highlight the elements being compared
        bars[j]->setStyleSheet("background-color: red;");
        bars[j + 1]->setStyleSheet("background-color: red;");
        QCoreApplication::processEvents();

        // Shift the larger element to the right (move operation)
        int temp = data[j];
        data[j] = data[j + 1];
        data[j + 1] = temp;

        // Update the UI
        bars[j + 1]->setText(QString::number(data[j + 1]));
        bars[j]->setText(QString::number(data[j]));

        --j; // Move to the next element to the left
        return;
    } else {
        // Insert the key at the correct position
        data[j + 1] = key;
        bars[j + 1]->setText(QString::number(key));
        bars[j + 1]->setStyleSheet("background-color: blue;");

        // Move to the next element to be inserted
        ++i;
        key = -1; // Reset key
        j = -1;  // Reset j
    }

    QCoreApplication::processEvents();  // Ensure UI updates
}

void MainWindow::selectionSortStep() {
    static int i = 0;  // Start from the first element
    static int min_idx = -1; // To store the index of the current minimum element

    // Base case: if sorting is done, stop the timer and set status
    if (i >= data.size() - 1) {
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
        for (QLabel* bar : bars) {
            bar->setStyleSheet("background-color: green;");  // Final color for sorted bars
        }
        return;
    }

    // Reset all bars' colors to blue before the new comparison
    for (int k = 0; k < data.size(); ++k) {
        bars[k]->setStyleSheet("background-color: blue;");
    }

    // Highlight the current element in the unsorted portion
    bars[i]->setStyleSheet("background-color: yellow;");

    // Initialize min_idx at the start of the unsorted portion
    if (min_idx == -1) {
        min_idx = i;
    }

    // Iterate through the unsorted portion to find the actual minimum element
    if (min_idx == i) {
        // Highlight the element being compared
        for (int j = i + 1; j < data.size(); ++j) {
            bars[j]->setStyleSheet("background-color: red;");
        }
        QCoreApplication::processEvents();

        for (int j = i + 1; j < data.size(); ++j) {
            if (data[j] < data[min_idx]) {
                min_idx = j; // Update min_idx if a smaller element is found
            }
        }
    }

    // Swap if necessary (after the full loop for minimum element)
    if (min_idx != i) {
        // Highlight the bars that are being swapped (red before swap)
        bars[i]->setStyleSheet("background-color: red;");
        bars[min_idx]->setStyleSheet("background-color: red;");
        QCoreApplication::processEvents();

        // Swap elements
        std::swap(data[i], data[min_idx]);

        // Update visuals
        bars[i]->setText(QString::number(data[i]));
        bars[min_idx]->setText(QString::number(data[min_idx]));

        // Keep them red for a moment after the swap
        QTimer::singleShot(700, this, [this, i = i, min_idx = min_idx] {
            // Reset colors to blue after the swap
            bars[i]->setStyleSheet("background-color: blue;");
            bars[min_idx]->setStyleSheet("background-color: blue;");
            QCoreApplication::processEvents();
        });
    }

    // Move to the next element
    ++i;
    min_idx = -1;  // Reset min_idx for the next round

    QCoreApplication::processEvents();  // Ensure UI updates
}

