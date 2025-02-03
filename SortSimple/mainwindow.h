#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTimer>
#include <vector>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSorting(); // Slot to handle sorting
    void performStep();  // Slot to handle animation steps
    void resetSorting();  // Slot for resetting the sorting

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();      // Function to set up the UI
    void bubbleSortStep(); // Step for bubble sort animation
    void quickSortStep(); // Step for quick sort animation
    void mergeSortStep(); // Step for merge sort animation
    void insertionSortStep(); // Step for insertion sort animation
    void selectionSortStep(); // Step for selection sort animation
    bool isSorted();
    void applyStyles();

    QWidget *m_centralWidget;
    QComboBox *algorithmSelector;
    QPushButton *startButton;
    QPushButton *resetButton;
    QLabel *statusLabel;
    QLabel *paragraphLabel;

    std::vector<int> data;       // Array to sort
    std::vector<QLabel *> bars;  // Bar widgets for visualization
    int currentIndex;            // Index for sorting steps
    int currentSwapIndex;
    bool resetBool = false;
    QTimer *animationTimer;      // Timer for step animation
};

#endif // MAINWINDOW_H
