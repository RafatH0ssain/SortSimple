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
#include <QScrollArea>
#include <QFontDatabase>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_centralWidget(new QWidget(this)),
    algorithmSelector(new QComboBox(this)),
    startButton(new QPushButton("Start", this)),
    resetButton(new QPushButton("Reset", this)),
    statusLabel(new QLabel("Select an algorithm and start", this)),
    currentIndex(0),
    animationTimer(new QTimer(this))
{

    setupUI();
    setMinimumSize(800, 600);

    // Populate dropdown with sorting algorithms
    algorithmSelector->addItem("Bubble Sort");
    algorithmSelector->addItem("Merge Sort");
    algorithmSelector->addItem("Insertion Sort");
    algorithmSelector->addItem("Quick Sort");
    algorithmSelector->addItem("Selection Sort");

    int fontIdAll = QFontDatabase::addApplicationFont("Nasa21-l23X.ttf");
    if (fontIdAll == -1)
    {
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

void MainWindow::resetSorting()
{
    // Reset the data to its initial unsorted state
    data = {23, 41, 25, 54, 18, 14, 9, 10};
    resetBool = true;

    // Reset the visualization: all bars back to blue
    for (int i = 0; i < bars.size(); ++i)
    {
        bars[i]->setText(QString::number(data[i]));        // Restore original data value on each bar
        bars[i]->setStyleSheet("background-color: blue;"); // Reset the color of the bars to blue
    }

    // Stop the timer
    animationTimer->stop();

    int fontIdAll = QFontDatabase::addApplicationFont("Nasa21-l23X.ttf");
    if (fontIdAll == -1)
    {
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
void MainWindow::setupUI()
{

    QHBoxLayout *barsLayout = new QHBoxLayout;
    barsLayout->setContentsMargins(10, 0, 10, 0);
    barsLayout->setSpacing(4);
    data = {23, 41, 25, 54, 18, 14, 9, 10};
    barsLayout->setObjectName("barsLayout");

    // Load the custom font from the resources or file system
    int fontIdAll = QFontDatabase::addApplicationFont("Nasa21-l23X.ttf");
    if (fontIdAll == -1)
    {
        qWarning() << "Failed to load font!";
        return;
    }

    statusLabel->setObjectName("statusLabel");

    // Get the font family name
    QString fontFamilyAll = QFontDatabase::applicationFontFamilies(fontIdAll).at(0);

    // Set the font for the whole application
    QFont fontAll(fontFamilyAll);
    fontAll.setPixelSize(14);
    qApp->setFont(fontAll);

    int fontIdAcc = QFontDatabase::addApplicationFont("SuperComic-qZg62.ttf");

    if (fontIdAcc == -1)
    {
        qWarning() << "Failed to load font!";
        return;
    }

    QString fontFamilyAcc = QFontDatabase::applicationFontFamilies(fontIdAcc).at(0);
    QFont fontAcc(fontFamilyAcc);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(20, 15, 20, 15); // Keep existing
    mainLayout->setSpacing(15);

    m_centralWidget->setStyleSheet("background: black;");  // Set entire app background to black

    QWidget *headerContainer = new QWidget(this);
    headerContainer->setObjectName("headerContainer");
    headerContainer->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1a1a2e, stop:1 #3a0c60);"
        "border-radius: 15px;"
        "padding: 35px 30px;"
        "border: 1px solid rgba(255,255,255,0.15);"
        "            inset 0 4px 8px rgba(255,255,255,0.08);"
        "margin-bottom: 25px;"
        );

    QVBoxLayout *headerLayout = new QVBoxLayout(headerContainer);
    headerLayout->setSpacing(8);  // Reduce spacing between elements
    headerLayout->setContentsMargins(15, 15, 15, 15);  // Consistent internal padding

    QFrame *headerDecoration = new QFrame(headerContainer);
    headerDecoration->setFixedHeight(4);
    headerDecoration->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3a86ff, stop:1 #8338ec);"
        "border-radius: 2px;"
        "margin: 15px 0;"
        );

    QLabel *h1 = new QLabel(this);
    QLabel *h2 = new QLabel(this);
    QLabel *p = new QLabel(this);

    // Remove individual style sheets from labels and set alignment only
    h1->setAlignment(Qt::AlignCenter);
    h2->setAlignment(Qt::AlignCenter);
    p->setAlignment(Qt::AlignCenter);

    // Set text properties
    h1->setText("<span style='font-size: 32px; font-weight: bold; color: #ffffff; "
                "text-shadow: 0 2px 4px rgba(0,0,0,0.5);'>SortSimple - Dynamic Sorting Visualization</span>");
    h2->setText("<span style='font-size: 17px; color: rgba(255,255,255,0.9); "
                "text-shadow: 0 1px 2px rgba(0,0,0,0.3); line-height: 1.4;'>"
                "An interactive app that visually demonstrates the step-by-step process of "
                "sorting algorithms like Bubble Sort, Merge Sort,<br>and Quick Sort, offering "
                "a hands-on way to understand algorithmic behavior.</span>");
    p->setText("<span style='font-size: 14px; color: #BDC3C7;'>Project contributors: Nafisah Nubah, Rafat Hossain</span>");

    // Make labels transparent to show parent gradient
    h1->setAttribute(Qt::WA_TranslucentBackground);
    h2->setAttribute(Qt::WA_TranslucentBackground);
    p->setAttribute(Qt::WA_TranslucentBackground);

    // Use rich text formatting instead of CSS
    headerLayout->addWidget(h1);
    headerLayout->insertWidget(1, headerDecoration);
    headerLayout->addWidget(h2);

    // Add this after setting up all other elements
    headerContainer->adjustSize();

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(algorithmSelector);
    algorithmSelector->setStyleSheet(
        "QComboBox {"
        "  background: rgba(255,255,255,0.05);"
        "  color: white;"
        "  border: 1px solid rgba(255,255,255,0.15);"
        "  border-radius: 6px;"
        "  padding: 10px;"
        "  min-width: 220px;"
        "}"
        "QComboBox:hover { border-color: rgba(255,255,255,0.3); }"
        "QComboBox::drop-down { border: none; width: 30px; }"
        "QComboBox QAbstractItemView {"
        "  background: #1a1a2e;"
        "  color: white;"
        "  selection-background-color: #3a86ff;"
        "}"
        );
    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(resetButton);
    startButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00b4d8, stop:1 #0077b6);"
        "  border-radius: 8px;"
        "  padding: 14px 28px;"
        "  color: white;"
        "  font-weight: 500;"
        "  border: none;"
        "  transition: transform 0.2s;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #48cae4, stop:1 #0096c7);"
        "  transform: translateY(-1px);"
        "}"
        "QPushButton:pressed { transform: translateY(0); }"
        );

    resetButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E74C3C, stop:1 #C0392B);"
        "  border-radius: 8px;"
        "  padding: 12px 24px;"
        "  color: white;"
        "}"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF6B6B, stop:1 #EE5253); }"
        "QPushButton:pressed { background: #C0392B; }"
        );

    QHBoxLayout *descriptionLayout = new QHBoxLayout;
    paragraphLabel = new QLabel(this);
    paragraphLabel->setText("<p></p>");
    paragraphLabel->setFont(fontAll);
    paragraphLabel->setObjectName("algoDescription");
    descriptionLayout->addWidget(paragraphLabel);
    paragraphLabel->setStyleSheet(
        "background: rgba(255,255,255,0.03);"
        "color: #caf0f8;"
        "border-radius: 12px;"
        "padding: 20px;"
        "border: 1px solid rgba(255,255,255,0.08);"
        "margin-top: 15px;"
        "font-size: 14px;"
        "line-height: 1.5;"
        );

    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(15);
    controlsLayout->setSpacing(10);
    barsLayout->setSpacing(3);

    mainLayout->addWidget(headerContainer);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(statusLabel);
    statusLabel->setStyleSheet(
        "color: #a8dadc;"
        "padding: 12px 20px;"
        "font-size: 15px;"
        "background: rgba(255,255,255,0.05);"
        "border-radius: 8px;"
        "border: 1px solid rgba(255,255,255,0.1);"
        "margin: 15px 40px;"
        );

    // Store bars in a container for easy manipulation
    for (int value : data)
    {
        QLabel *bar = new QLabel;
        bar->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3498DB, stop:1 #2980B9);"
            "border-radius: 8px 8px 0 0;"
            "color: white;"
            "border: 1px solid #2C3E50;"
            );
        bar->setObjectName("bar");
        bar->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8338ec, stop:1 #3a86ff);"
            "border-radius: 6px 6px 0 0;"
            "color: white;"
            "border: 1px solid rgba(255,255,255,0.15);"
            "font-weight: bold;"
            "text-shadow: 0 1px 2px rgba(0,0,0,0.3);"
            );
        bar->setText(QString::number(value));
        bar->setFont(fontAcc);
        bar->setAlignment(Qt::AlignCenter);
        barsLayout->addWidget(bar);
        bars.push_back(bar);
    }

    mainLayout->addLayout(barsLayout);
    mainLayout->addLayout(descriptionLayout);
    m_centralWidget->setLayout(mainLayout);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // Make the scroll area resize with its contents
    scrollArea->setWidget(m_centralWidget); // Set the central widget as the scroll area's content
    setCentralWidget(scrollArea); // Set the scroll area as the main window's central widget

    // Footer
    QWidget *footerContainer = new QWidget(this);
    footerContainer->setObjectName("footerContainer");
    footerContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    footerContainer->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1a1a2e, stop:1 #3a0c60);"
        "border-radius: 15px;"
        "padding: 25px;"
        "border: 1px solid rgba(255,255,255,0.15);"
        "box-shadow: 0 8px 32px rgba(0,0,0,0.3);"
        "margin-top: 30px;"
        );

    QVBoxLayout *footerLayout = new QVBoxLayout(footerContainer);
    footerLayout->setSpacing(8);
    footerLayout->setContentsMargins(20, 15, 20, 15);

    QFrame *footerSeparator = new QFrame(footerContainer);
    footerSeparator->setFixedHeight(2);
    footerSeparator->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3a86ff, stop:1 #8338ec);"
        "margin: 0 30px 15px 30px;"
        );
    footerLayout->addWidget(footerSeparator);

    QLabel *footerText = new QLabel(this);
    footerText->setText("© 2025 SortSimple - All rights reserved.<br>"
                        "<span style='font-size: 12px; color: rgba(200,200,200,0.8);'>"
                        "Project contributors: Nafisah Nubah, Rafat Hossain</span>");
    footerText->setStyleSheet(
        "color: rgba(224,224,224,0.9);"
        "font-size: 13px;"
        "qproperty-alignment: AlignCenter;"
        "text-shadow: 0 1px 3px rgba(0,0,0,0.4);"
        "letter-spacing: 0.5px;"
        );
    footerLayout->addWidget(footerText);

    mainLayout->addWidget(footerContainer);
    m_centralWidget->setStyleSheet(
        "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 #1a1a2e, stop:1 #16213e);"
        );

    // Add this at the end of setupUI()
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, true);
    QApplication::setEffectEnabled(Qt::UI_FadeMenu, true);
}

// Resize event to dynamically adjust bar size based on window size
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Calculate bar dimensions based on available space
    int barCount = bars.size();
    int contentWidth = m_centralWidget->width() - 40; // Account for margins
    // In resizeEvent, modify bar spacing
    int barWidth = qMax(40, contentWidth / (barCount + 4)); // Increased minimum width

    // Calculate height based on window proportions
    int barHeight = qBound(80, event->size().height() / 4, 200);

    for (QLabel *bar : bars) {
        bar->setFixedSize(barWidth, barHeight);
    }

    // Ensure proper layout update
    m_centralWidget->layout()->activate();
}

// Start sorting animation
void MainWindow::startSorting()
{
    QString selectedAlgorithm = algorithmSelector->currentText();
    resetBool = false;

    if (selectedAlgorithm == "Bubble Sort")
    {
        statusLabel->setText("Sorting using Bubble Sort...");
        statusLabel->setStyleSheet(
            "font-size: 16px;"
            "color: #1ABC9C;"
            "font-weight: bold;"
            "margin: 10px 0;"
            );
        paragraphLabel->setText("<p>Bubble Sort repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. For this array: {23, 41, 25, 54, 18, 14, 9, 10}, the algorithm will take the following steps:</p>"
                                "<p>1. First pass: Compare each adjacent pair and swap if necessary. The algorithm compares 23 and 41, then 41 and 25, swapping these to get {23, 25, 41, 54, 18, 14, 9, 10}. The next swaps will continue through the rest of the array.</p>"
                                "<p>2. Second pass: After the first pass, the largest element (54) has 'bubbled' to the end. The algorithm repeats the process for the remaining unsorted part of the list, gradually moving the next largest element to its correct position.</p>"
                                "<p>3. This process continues until no more swaps are needed, meaning the array is fully sorted. If no swaps are made in a pass, the algorithm stops early, marking the sorting process as complete.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Quick Sort")
    {
        statusLabel->setText("Sorting using Quick Sort...");
        paragraphLabel->setText("<p>Quick Sort selects a pivot element and partitions the array into elements less than or equal to the pivot and greater than the pivot. For the array {23, 41, 25, 54, 18, 14, 9, 10}:</p>"
                                "<p>1. First partition: Choose last element (10) as pivot. Rearrange elements so all values ≤10 come before it. The array becomes {9, 10, 25, 54, 18, 14, 23, 41} with 10 in correct position.</p>"
                                "<p>2. Recursively process left subarray {9} (already sorted) and right subarray {25,54,18,14,23,41}. New pivot 41 results in {25,23,18,14,41,54}.</p>"
                                "<p>3. Repeat partitioning until all subarrays are single elements. Final sorted array emerges through recursive recombination of sorted partitions.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Merge Sort")
    {
        statusLabel->setText("Sorting using Merge Sort...");
        paragraphLabel->setText("<p>Merge Sort divides the array into halves, sorts them recursively, then merges sorted halves. For {23,41,25,54,18,14,9,10}:</p>"
                                "<p>1. Split into [23,41,25,54] and [18,14,9,10]. Recursively split until single elements.</p>"
                                "<p>2. Merge pairs: [23,41] & [25,54] become [23,25,41,54], [14,18] & [9,10] become [9,10,14,18].</p>"
                                "<p>3. Final merge combines [23,25,41,54] and [9,10,14,18] by comparing elements sequentially, resulting in the sorted array.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else if (selectedAlgorithm == "Insertion Sort")
    {
        statusLabel->setText("Sorting using Insertion Sort...");
        paragraphLabel->setText("<p>Insertion Sort builds the sorted array by inserting one element at a time. Starting with {23,41,25,54,18,14,9,10}:</p>"
                                "<p>1. First element (23) is sorted. Insert 41 → {23,41}. Insert 25 → {23,25,41}. Insert 54 → {23,25,41,54}.</p>"
                                "<p>2. Insert 18: Shift elements 23-54 right to make space → {18,23,25,41,54}. Continue with 14 → {14,18,23,25,41,54}.</p>"
                                "<p>3. Final insertions place 9 and 10 at the beginning through successive shifts, completing the sort.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
    else
    {
        statusLabel->setText("Sorting using Selection Sort...");
        paragraphLabel->setText("<p>Selection Sort finds the minimum element repeatedly. For {23,41,25,54,18,14,9,10}:</p>"
                                "<p>1. First iteration: Find minimum (9 at index 6). Swap with first element → {9,41,25,54,18,14,23,10}.</p>"
                                "<p>2. Second iteration: Find minimum in remaining elements (10 at index 7). Swap with second position → {9,10,25,54,18,14,23,41}.</p>"
                                "<p>3. Continue selecting next smallest elements (14,18,23,...) and swap them into position until the array is fully sorted.</p>");
        currentIndex = 0;
        animationTimer->start(1000);
    }
}

// Perform a step in the sorting animation
void MainWindow::performStep()
{
    if (algorithmSelector->currentText() == "Bubble Sort")
    {
        bubbleSortStep();
    }
    else if ((algorithmSelector->currentText() == "Quick Sort"))
    {
        quickSortStep();
    }
    else if (algorithmSelector->currentText() == "Merge Sort")
    {
        mergeSortStep();
    }
    else if ((algorithmSelector->currentText() == "Insertion Sort"))
    {
        insertionSortStep();
    }
    else
    {
        selectionSortStep();
    }
}

// Check if data is sorted
bool MainWindow::isSorted()
{
    for (int i = 0; i < data.size() - 1; ++i)
    {
        if (data[i] > data[i + 1])
        {
            return false; // Not sorted in ascending order
        }
    }
    return true; // Sorted in ascending order
}

void MainWindow::bubbleSortStep()
{
    static int i = 0; // Loop variable
    static int j = 0; // Inner loop variable
    static bool swapped = false;

    if (resetBool)
    {
        j = 0;
        i = 0;
        swapped = false;
        int newBars[] = {23, 41, 25, 54, 18, 14, 9, 10};
        swapped = !swapped;
        resetBool = !resetBool;
        for (int i = 0; i < 8; i++)
        {
            bars[i]->setText(QString::number(newBars[i]));
        }
    }

    if (i < data.size())
    {
        if (j < data.size() - 1 - i)
        {
            if (data[j] > data[j + 1])
            {
                int currentJ = j; // Capture the value of j

                // Highlight the current swap
                bars[currentJ]->setStyleSheet("background-color: red;");
                bars[currentJ + 1]->setStyleSheet("background-color: red;");
                QCoreApplication::processEvents();

                swapped = true;

                // Swap data after a small delay to visualize the swap
                QTimer::singleShot(700, this, [this, currentJ]
                                   {
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
                                       }); });
                return; // Exit to allow the timer to trigger the next step
            }
            ++j; // Move to the next pair
        }
        else
        {
            if (!swapped)
            {
                // If no swaps were made, the sorting is complete
                animationTimer->stop();
                statusLabel->setText("Sorting complete!");
                i = 0;
                j = 0;

                for (auto *bar : bars)
                {
                    bar->setStyleSheet("background-color: green;");
                }
            }
            ++i;              // Move to the next pass
            j = 0;            // Reset inner loop variable
            swapped = false;  // Reset swap flag
            bubbleSortStep(); // Continue to the next pass
        }
    }
}

void MainWindow::quickSortStep()
{
    static QVector<QPair<int, int>> stack; // Stack to store sub-array ranges
    static int left = -1, right = -1, pivotIndex = -1;
    static int start = -1, end = -1; // Track current range explicitly

    // Initialize stack with the full range during the first call
    if (stack.isEmpty())
    {
        stack.push_back({0, data.size() - 1});
    }

    // If no current partitioning step is active, set up a new range
    if (pivotIndex == -1 && !stack.isEmpty())
    {
        auto range = stack.takeLast();
        start = range.first;
        end = range.second;

        if (start < end)
        {
            pivotIndex = end; // Choose the last element as pivot
            left = start;
            right = start;
            bars[pivotIndex]->setStyleSheet("background-color: red;"); // Highlight pivot
            QCoreApplication::processEvents();
        }
        else
        {
            // Reset and skip invalid ranges
            pivotIndex = -1;
            left = -1;
            right = -1;
        }
    }

    // If partitioning is active, perform a single step
    if (pivotIndex != -1)
    {
        if (right < pivotIndex)
        {
            if (data[right] <= data[pivotIndex])
            { // Ascending order comparison
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
                QTimer::singleShot(1000, this, [this, left = left, right = right]
                                   {
                                       bars[left]->setStyleSheet("background-color: blue;");
                                       bars[right]->setStyleSheet("background-color: blue;");
                                       QCoreApplication::processEvents(); });

                ++left; // Increment left pointer
            }
            ++right; // Increment right pointer
        }
        else
        {
            // Place pivot in its correct position
            std::swap(data[left], data[pivotIndex]);

            // Update visuals
            bars[left]->setText(QString::number(data[left]));
            bars[pivotIndex]->setText(QString::number(data[pivotIndex]));

            // Highlight final pivot position temporarily before resetting color
            bars[left]->setStyleSheet("background-color: red;");
            bars[pivotIndex]->setStyleSheet("background-color: red;");
            QCoreApplication::processEvents();

            QTimer::singleShot(1000, this, [this, left = left, pivotIndex = pivotIndex]
                               {
                                   bars[left]->setStyleSheet("background-color: blue;");
                                   bars[pivotIndex]->setStyleSheet("background-color: blue;");
                                   QCoreApplication::processEvents(); });

            // Push new sub-ranges to stack
            if ((start < left - 1))
            {
                stack.push_back({start, left - 1});
            }
            if ((left + 1 < end))
            {
                stack.push_back({left + 1, end});
            }

            // Reset for next partitioning
            pivotIndex = left = right = -1;
        }
    }

    if (isSorted())
    {

        QTimer::singleShot(2000, this, [this]
                           {
                               animationTimer->stop();
                               statusLabel->setText("Sorting complete!");
                               for (QLabel* bar : bars) {
                                   bar->setStyleSheet("background-color: green;");
                               } });

        // Reset static variables for future sorting
        stack.clear();
        left = right = pivotIndex = start = end = -1;
    }
}

void MainWindow::mergeSortStep()
{
    static QVector<QPair<int, int>> stack; // Stack to store sub-array ranges
    static QVector<int> tempData;
    static int start = -1, end = -1, mid = -1;

    // Check if sorting is complete
    if (isSorted())
    {
        statusLabel->setText("Sorting complete!");

        // Update all bars to show sorted state
        for (QLabel *bar : bars)
        {
            setStyleSheet("background-color: green;");
        }

        // Reset static variables for future sorting
        stack.clear();
        start = end = mid = -1; // Reset the partitioning variables
        animationTimer->stop();
    }

    // Initialize stack with the full range during the first call
    if (stack.isEmpty() || resetBool)
    {
        stack.push_back({0, data.size() - 1});
        resetBool = false;
    }

    // If no current partitioning step is active, set up a new range
    if (start == -1 && !stack.isEmpty())
    {
        // Pop the range from the stack
        auto range = stack.takeLast();
        start = range.first;
        end = range.second;

        // Calculate mid-point of the range
        mid = (start + end) / 2;

        // Push new sub-ranges onto the stack for further processing
        if (start < mid)
        {
            stack.push_back({start, mid});
        }
        if (mid + 1 < end)
        {
            stack.push_back({mid + 1, end});
        }

        // Animate division of the array (highlight the current sub-array being processed)
        for (int i = start; i <= end; ++i)
        {
            if (i >= 0 && i < bars.size() && !isSorted())
            {
                bars[i]->setStyleSheet("background-color: red;");
            }
        }
        QCoreApplication::processEvents(); // UI update
        return;
    }

    // Perform merge step once the range is split
    if (start <= mid && mid + 1 <= end)
    {
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
        while (i < n1 && j < n2)
        {
            if (L[i] <= R[j])
            {
                data[k] = L[i];
                i++;
            }
            else
            {
                data[k] = R[j];
                j++;
            }
            k++;
        }

        // Copy the remaining elements of L[], if there are any
        while (i < n1)
        {
            data[k] = L[i];
            i++;
            k++;
        }

        // Copy the remaining elements of R[], if there are any
        while (j < n2)
        {
            data[k] = R[j];
            j++;
            k++;
        }

        // Update visuals after merging
        for (int i = start; i <= end; ++i)
        {
            if (i >= 0 && i < bars.size())
            {
                bars[i]->setText(QString::number(data[i]));
                bars[i]->setStyleSheet("background-color: blue;");
            }
        }

        QCoreApplication::processEvents(); // Ensure UI updates

        // Reset indices to process the next range
        start = end = mid = -1; // Reset after a merge step
        return;
    }
}

void MainWindow::insertionSortStep()
{
    static int i = 1;
    static int j = -1;
    static int key = -1;

    // Base case: if sorting is done, stop the timer and set status
    if (i >= data.size())
    {
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
        for (QLabel *bar : bars)
        {
            bar->setStyleSheet("background-color: green;"); // Final color for sorted bars
        }
        return;
    }

    // Initialize key and j for the first time
    if (key == -1 && j == -1)
    {
        key = data[i];
        j = i - 1;
    }

    // Reset all bars' colors to blue before the new comparison
    for (int k = 0; k < data.size(); ++k)
    {
        bars[k]->setStyleSheet("background-color: blue;");
    }

    // Highlight the key
    bars[i]->setStyleSheet("background-color: yellow;");

    // Find the correct position for the key in the sorted part of the array
    if (j >= 0 && data[j] > key)
    {
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
    }
    else
    {
        // Insert the key at the correct position
        data[j + 1] = key;
        bars[j + 1]->setText(QString::number(key));
        bars[j + 1]->setStyleSheet("background-color: blue;");

        // Move to the next element to be inserted
        ++i;
        key = -1; // Reset key
        j = -1;   // Reset j
    }

    QCoreApplication::processEvents(); // Ensure UI updates
}

void MainWindow::selectionSortStep()
{
    static int i = 0;        // Start from the first element
    static int min_idx = -1; // To store the index of the current minimum element

    // Base case: if sorting is done, stop the timer and set status
    if (i >= data.size() - 1)
    {
        animationTimer->stop();
        statusLabel->setText("Sorting complete!");
        for (QLabel *bar : bars)
        {
            bar->setStyleSheet("background-color: green;"); // Final color for sorted bars
        }
        return;
    }

    // Reset all bars' colors to blue before the new comparison
    for (int k = 0; k < data.size(); ++k)
    {
        bars[k]->setStyleSheet("background-color: blue;");
    }

    // Highlight the current element in the unsorted portion
    bars[i]->setStyleSheet("background-color: yellow;");

    // Initialize min_idx at the start of the unsorted portion
    if (min_idx == -1)
    {
        min_idx = i;
    }

    // Iterate through the unsorted portion to find the actual minimum element
    if (min_idx == i)
    {
        // Highlight the element being compared
        for (int j = i + 1; j < data.size(); ++j)
        {
            bars[j]->setStyleSheet("background-color: red;");
        }
        QCoreApplication::processEvents();

        for (int j = i + 1; j < data.size(); ++j)
        {
            if (data[j] < data[min_idx])
            {
                min_idx = j; // Update min_idx if a smaller element is found
            }
        }
    }

    // Swap if necessary (after the full loop for minimum element)
    if (min_idx != i)
    {
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
        QTimer::singleShot(700, this, [this, i = i, min_idx = min_idx]
                           {
                               // Reset colors to blue after the swap
                               bars[i]->setStyleSheet("background-color: blue;");
                               bars[min_idx]->setStyleSheet("background-color: blue;");
                               QCoreApplication::processEvents(); });
    }

    // Move to the next element
    ++i;
    min_idx = -1; // Reset min_idx for the next round

    QCoreApplication::processEvents(); // Ensure UI updates
}
