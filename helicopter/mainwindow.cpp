#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    helicopter_.first = new helicopter();

    setLabel();

    QColor color(173,216,230);
    this->setPalette(QPalette(color));

    mainT_ = new QTimer(this);
    mainT_->setInterval(100);
    connect(mainT_, &QTimer::timeout, this, &MainWindow::onTimeOut);

    mainT_->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (DIRECTIONS.find(event->key()) == DIRECTIONS.end())
    {
        return;
    }


    currentDirs_.push_back(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (DIRECTIONS.find(event->key()) == DIRECTIONS.end())
    {
        return;
    }

    std::vector<int>::iterator iter = std::find(currentDirs_.begin(), currentDirs_.end()
                                                , event->key());
    currentDirs_.erase(iter);
}


void MainWindow::setLabel()
{
    const std::string PREFIX(":/");
    const std::string SUFFIX(".png");

        // Converting image (png) to a pixmap
        std::string filename = PREFIX + "hulilupteri" + SUFFIX;
        QPixmap image(QString::fromStdString(filename));

        // Scaling the pixmap
        image = image.scaled(200, 200);

        // Setting the pixmap for a label
        QLabelR* label = new QLabelR(this);
        label->setGeometry(55,
                           55,
                           140, 140);
        label->setPermPic(image);

        label->setPixmap(image);

        helicopter_.second = label;
}

void MainWindow::updateScreen()
{
    int hulilupterInc = int(helicopter_.first->getXAccel() * 5 / 2);
    Coords loc = helicopter_.first->getLocation();
    helicopter_.second->setInclination(hulilupterInc);
    helicopter_.second->setGeometry(loc.second, loc.first, 140, 140);
}

void MainWindow::onTimeOut()
{
    if (currentDirs_.size() == 0)
    {
        helicopter_.first->changeAccel({0,0});
        updateScreen();
        return;
    }
    Coords currentAcc = {0,0};
    for (int c : currentDirs_)
    {
        Coords current = DIRECTIONS.at(c);
        currentAcc.first += current.first;
        currentAcc.second += current.second;
    }
    helicopter_.first->changeAccel(currentAcc);
    updateScreen();
}
