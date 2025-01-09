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
    } else {
        statusLabel->setText("Algorithm not implemented yet.");
    }
}

// Perform a step in the sorting animation
void MainWindow::performStep() {
    if (algorithmSelector->currentText() == "Bubble Sort") {
        bubbleSortStep();
    }
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
