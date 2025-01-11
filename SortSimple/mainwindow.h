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

private:
    void setupUI();      // Function to set up the UI
    void bubbleSortStep(); // Step for bubble sort animation
    void quickSortStep(); // Step for bubble sort animation
    bool isSorted();

    QWidget *centralWidget;
    QComboBox *algorithmSelector;
    QPushButton *startButton;
    QLabel *statusLabel;

    std::vector<int> data;       // Array to sort
    std::vector<QLabel *> bars;  // Bar widgets for visualization
    int currentIndex;            // Index for sorting steps
    int currentSwapIndex;
    QTimer *animationTimer;      // Timer for step animation
};

#endif // MAINWINDOW_H
