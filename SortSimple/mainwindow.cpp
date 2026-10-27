#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <qcoreapplication.h>
#include <QTextStream>
#include <QApplication>
#include <QResizeEvent>
#include <QScrollArea>
#include <QFontDatabase>

namespace {

QString fontPath(const QString &fileName)
{
    return QCoreApplication::applicationDirPath() + QLatin1Char('/') + fileName;
}

} // namespace

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

    int fontIdAll = QFontDatabase::addApplicationFont(fontPath("Nasa21-l23X.ttf"));
    if (fontIdAll == -1)
    {
        qWarning() << "Failed to load font!";
    }
    else
    {
        QString fontFamilyAll = QFontDatabase::applicationFontFamilies(fontIdAll).at(0);
        QFont fontAll(fontFamilyAll);
        algorithmSelector->setFont(fontAll);
        startButton->setFont(fontAll);
        resetButton->setFont(fontAll);
        statusLabel->setFont(fontAll);
    }

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
    data = sorts::demoData();
    bubbleSort.reset();
    quickSort.reset();
    mergeSort.reset();
    insertionSort.reset();
    selectionSort.reset();

    // Reset the visualization: all bars back to blue
    for (int i = 0; i < bars.size(); ++i)
    {
        bars[i]->setText(QString::number(data[i]));        // Restore original data value on each bar
        bars[i]->setStyleSheet("background-color: blue;"); // Reset the color of the bars to blue
    }

    // Stop the timer
    animationTimer->stop();

    int fontIdAll = QFontDatabase::addApplicationFont(fontPath("Nasa21-l23X.ttf"));
    if (fontIdAll == -1)
    {
        qWarning() << "Failed to load font!";
    }
    else
    {
        QString fontFamilyAll = QFontDatabase::applicationFontFamilies(fontIdAll).at(0);
        QFont fontAll(fontFamilyAll);
        statusLabel->setFont(fontAll);
    }

    // Reset the status label
    statusLabel->setText("Select an algorithm and start");

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
    data = sorts::demoData();
    barsLayout->setObjectName("barsLayout");

    // Load the custom font from the application directory
    int fontIdAll = QFontDatabase::addApplicationFont(fontPath("Nasa21-l23X.ttf"));
    if (fontIdAll == -1)
    {
        qWarning() << "Failed to load font!";
    }

    statusLabel->setObjectName("statusLabel");

    QString fontFamilyAll;
    if (fontIdAll != -1)
    {
        fontFamilyAll = QFontDatabase::applicationFontFamilies(fontIdAll).at(0);
    }

    // Set the font for the whole application when available
    QFont fontAll(fontFamilyAll);
    if (!fontFamilyAll.isEmpty())
    {
        fontAll.setPixelSize(14);
        qApp->setFont(fontAll);
    }

    int fontIdAcc = QFontDatabase::addApplicationFont(fontPath("SuperComic-qZg62.ttf"));

    if (fontIdAcc == -1)
    {
        qWarning() << "Failed to load font!";
    }

    QString fontFamilyAcc;
    if (fontIdAcc != -1)
    {
        fontFamilyAcc = QFontDatabase::applicationFontFamilies(fontIdAcc).at(0);
    }
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
    bubbleSort.reset();
    quickSort.reset();
    mergeSort.reset();
    insertionSort.reset();
    selectionSort.reset();

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
    sorts::StepResult result;
    const QString selected = algorithmSelector->currentText();
    if (selected == "Bubble Sort")
    {
        result = bubbleSort.step(data);
    }
    else if (selected == "Quick Sort")
    {
        result = quickSort.step(data);
    }
    else if (selected == "Merge Sort")
    {
        result = mergeSort.step(data);
    }
    else if (selected == "Insertion Sort")
    {
        result = insertionSort.step(data);
    }
    else
    {
        result = selectionSort.step(data);
    }

    applyStepVisuals(result);
    if (result.done)
    {
        finishSortedUI();
    }
}

void MainWindow::applyStepVisuals(const sorts::StepResult &result)
{
    for (int k = 0; k < static_cast<int>(bars.size()) && k < static_cast<int>(data.size()); ++k)
    {
        bars[k]->setText(QString::number(data[k]));
        bars[k]->setStyleSheet("background-color: blue;");
    }
    if (result.hi1 >= 0 && result.hi1 < static_cast<int>(bars.size()))
    {
        bars[result.hi1]->setStyleSheet("background-color: red;");
    }
    if (result.hi2 >= 0 && result.hi2 < static_cast<int>(bars.size()))
    {
        bars[result.hi2]->setStyleSheet("background-color: red;");
    }
}

void MainWindow::finishSortedUI()
{
    animationTimer->stop();
    statusLabel->setText("Sorting complete!");
    for (auto *bar : bars)
    {
        bar->setStyleSheet("background-color: green;");
    }
}
