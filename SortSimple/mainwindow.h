#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sorts.h"

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
    void startSorting();
    void performStep();
    void resetSorting();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    void applyStepVisuals(const sorts::StepResult &result);
    void finishSortedUI();

    QWidget *m_centralWidget;
    QComboBox *algorithmSelector;
    QPushButton *startButton;
    QPushButton *resetButton;
    QLabel *statusLabel;
    QLabel *paragraphLabel;

    std::vector<int> data;
    std::vector<QLabel *> bars;
    int currentIndex;
    QTimer *animationTimer;

    sorts::BubbleSort bubbleSort;
    sorts::QuickSort quickSort;
    sorts::MergeSort mergeSort;
    sorts::InsertionSort insertionSort;
    sorts::SelectionSort selectionSort;
};

#endif // MAINWINDOW_H
