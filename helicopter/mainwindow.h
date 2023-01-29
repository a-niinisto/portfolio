#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabelr.h"
#include "helicopter.h"
#include <QMainWindow>
#include <QTimer>
#include <vector>
#include <QKeyEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
const std::map<int, Coords> DIRECTIONS{
    {87, {-1, 0}},
    {65, {0, -1}},
    {83, {1, 0}},
    {68, {0, 1}}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void onTimeOut();

private:
    void setLabel();
    void updateScreen();

    Ui::MainWindow *ui;

    QTimer* mainT_;

    std::vector<int> currentDirs_;

    std::pair<helicopter*, QLabelR*> helicopter_;
};
#endif // MAINWINDOW_H
