#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameboard.h"
#include "numbertile.h"
#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QLabel>
#include <utility>
#include <map>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const int LEFTMARGIN = 40;
const int TOPMARGIN = 100;
const int GRAPHICSCENESIZE = 400;
const int GRIDSIDE = 9;

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

private slots:
    void onStart();
    void onSolve();

private:
    void updateScreen();
    void initGrid();
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    gameboard board_;

    int graphicSceneSize_;

    std::vector<std::vector<std::pair<QGraphicsRectItem*, QLabel*>>> rectsLabels_;

    Coords currentSquare_;

    bool gameStopped_;

    int seed_;

};
#endif // MAINWINDOW_H
