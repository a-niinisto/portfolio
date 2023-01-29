#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <iomanip>
#include <QGraphicsLineItem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graphicSceneSize_(GRAPHICSCENESIZE)
    , gameStopped_(true)
    , seed_(0)
{
    ui->setupUi(this);
    scene_ = new QGraphicsScene(this);
    initGrid();
    currentSquare_ = {0,0};
    ui->seedSpinBox->setMinimum(0);
    ui->seedSpinBox->setMaximum(400);
    ui->amountSpinBox->setMinimum(1);
    ui->amountSpinBox->setMaximum(81);

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::onSolve);

    ui->solveButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (gameStopped_)
    {
        return;
    }



    if (DIRECTIONS.find(event->key()) == DIRECTIONS.end() and (toascii(event->key()) < 48
                                                               or toascii(event->key()) > 57))
    {
        return;
    }
    if (DIRECTIONS.find(event->key()) != DIRECTIONS.end())
    {
        rectsLabels_.at(currentSquare_.first).at(currentSquare_.second).first->setBrush(Qt::NoBrush);
        Coords dir = DIRECTIONS.at(event->key());
        currentSquare_.second += dir.second;
        currentSquare_.first += dir.first;
        if (currentSquare_.first < 0 or currentSquare_.first >= GRIDSIDE)
        {
            currentSquare_.first -= dir.first;
        }
        if (currentSquare_.second < 0 or currentSquare_.second >= GRIDSIDE)
        {
            currentSquare_.second -= dir.second;
        }
        rectsLabels_.at(currentSquare_.first).at(currentSquare_.second).first->setBrush(Qt::red);
    }
    else if (seed_ == 0)
    {
        int number = event->key() - 48;
        board_.setNum(currentSquare_, number);
        updateScreen();
    }
    else
    {
        int number = event->key() - 48;
        if (board_.move(currentSquare_, number))
        {
            ui->infoTextBrowser->insertPlainText("You solved the sudoku!");
            gameStopped_ = true;
        }
        updateScreen();
    }
}

void MainWindow::onStart()
{
    ui->infoTextBrowser->clear();
    seed_ = ui->seedSpinBox->value();
    int showAmount = ui->amountSpinBox->value();
    if (seed_ == 0)
    {
        board_.resetBoard();
    }

    else
    {
        board_.initBoard(seed_, showAmount);
    }
    updateScreen();

    gameStopped_ = false;
    ui->solveButton->setEnabled(true);
}

void MainWindow::onSolve()
{
    board_.solve();
    updateScreen();
    ui->infoTextBrowser->insertPlainText("Sudoku solved!");
    ui->solveButton->setEnabled(false);
}

void MainWindow::updateScreen()
{
    for (int y = 0; y < GRIDSIDE; y++)
    {
        for (int x = 0; x < GRIDSIDE; x++)
        {
            Coords current = {y, x};
            numbertile* currentTile = board_.getTile(current);
            int shownNumber = currentTile->getShownNum();


            rectsLabels_.at(y).at(x).second->clear();
            if (shownNumber == 0)
            {
                continue;
            }
            std::string numberStr = std::to_string(shownNumber);
            QString numberQStr = QString::fromStdString(numberStr);
            rectsLabels_.at(y).at(x).second->setText(numberQStr);
        }
    }
    rectsLabels_.at(currentSquare_.first).at(currentSquare_.second).first->setBrush(Qt::red);
}

void MainWindow::initGrid()
{
    // Placing the graphicsView in to the gui:
    ui->graphicsView->setGeometry(LEFTMARGIN, TOPMARGIN,
                                   graphicSceneSize_ + 2, graphicSceneSize_ + 2);
    ui->graphicsView->setScene(scene_);

    // Initializing scene as a square:
    scene_->setSceneRect(0, 0, graphicSceneSize_ - 1, graphicSceneSize_ - 1);

    // calculating size of a individual square (tile) in the scene:
    double squareSide = (graphicSceneSize_ - 2)/ GRIDSIDE - 1;

    // Setting pen for the square's edges:
    QPen pen = QPen(Qt::transparent);
    pen.setWidth(1);

    // Creating all the rectItems and labels inside the scene:
    for (int y = 0; y < GRIDSIDE; y++)
    {
        rectsLabels_.push_back({});
        for (int x = 0; x < GRIDSIDE; x++)
        {
            rectsLabels_.at(y).push_back({});

            // Creating the rectItem:
            rectsLabels_.at(y).at(x).first = scene_->addRect(x * (squareSide + 1) + 2,
                            y * (squareSide + 1) + 2, squareSide, squareSide, pen);

            // Creating the label:
            QLabel* newLabel = new QLabel(this);
            newLabel->setFrameStyle(QFrame::Panel);
            newLabel->setAlignment(Qt::AlignCenter);
            newLabel->setGeometry(QRect(x * (squareSide + 1) + 3 + LEFTMARGIN,
                                        y * (squareSide + 1) + 3 + TOPMARGIN,
                                        squareSide, squareSide));
            newLabel->setStyleSheet("border: 0px; background-color:rgba(0,0,0,0%);");
            rectsLabels_.at(y).at(x).second = newLabel;
        }
    }
    QPen penB = QPen(Qt::black);
    penB.setWidth(2);
    for (int y = 0; y < GRIDSIDE + 1; y++)
    {
        if (y % 3 == 0)
        {
            penB.setColor(Qt::black);
        }
        else
        {
            penB.setColor(Qt::gray);
        }
        QGraphicsItem* parentY;
        QGraphicsItem* parentX;
        if (y == GRIDSIDE)
        {
            parentY = rectsLabels_.at(8).at(2).first;
            parentX = rectsLabels_.at(2).at(8).first;
        }
        else
        {
            parentY = rectsLabels_.at(y).at(0).first;
            parentX = rectsLabels_.at(1).at(y).first;
        }

        QGraphicsLineItem* newLine = new QGraphicsLineItem(parentY);
        newLine->setPen(penB);
        newLine->setLine(2,y *(squareSide + 1) + 2, graphicSceneSize_-2 ,y * (squareSide + 1) + 2);

        QGraphicsLineItem* otherLine = new QGraphicsLineItem(parentX);
        otherLine->setPen(penB);
        otherLine->setLine(y * (squareSide + 1) + 2,2,y *(squareSide + 1) + 2 ,graphicSceneSize_-2);
    }

}

