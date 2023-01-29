    /*  COMP.CS.110 Project 4: game 2048
 *
 *  Mainwindow-luokka toteuttaa graafisen käyttöliittymän peliin 2048.
 *  Luokka kysyy käyttäjältä seed-arvon, jolla ruudukko alustetaan, pelin
 *  tavoitearvon ja siirtojen määrän, jonka jälkeen käyttäjä voi poistaa yhden numeron.
 *  Näiden asettamisen jälkeen käyttäjä voi painaa ready-nappia, minkä jälkeen
 *  ohjelma täyttää ruudukon käyttöliittymässä. Tämän jälkeen käyttäjä voi tehdä
 *  siirtoja käyttäen wasd-näppäimiä. Pelin aloittamisen jälkeen käyttäjä voi muuttaa
 *  seed-arvoa, tavoitetta sekä removal-arvoa ja painaa reset-nappia
 *  aloittaakseen pelin alusta.
 *
 *  Ohjelma toteuttaa myös graafiseen käyttöliittymään aikalaskurin, joka mitta
 *  pelaajan käyttämää aikaa. Ohjelma toteuttaa myös ruudun, jossa esitetään pistelaskuri,
 *  ja ruudun jossa ilmoitetaan voitosta, häviöstä ja pause-tilasta.
 *
 *  Käyttäjä voi pausettaa pelin painamalla pause-nappia, minkä jälkeen aikalaskuri
 *  pysähtyy ja peli pysähtyy. Painamalla pause-nappia uudelleen käyttäjä käynnistää
 *  aikalaskurin ja pelin uudelleen.
 *
 *  Kun peli päättyy voittoon, käyttöliittymän taustaväri muuttuu vihreäksi.
 *  Kun peli päättyy häviöön, käyttöliittymän taustaväri muuttuu punaiseksi.
 *  Reset-napin painamisen jälkeen peli käynnistyy uudelleen ja taustaväri
 *  palautuu normaaliksi.
 *
 *
 *  Ohjelman kirjoittaja:
 *  Nimi: Aaro Niinistö
 *  E-Mail: aaro.niinisto@aalto.fi
 *
 * */
#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , graphicSceneSize_(GRAPHICSCENESIDE)
    , gridSize_(SIZE)
    , moveCounter_(0)
    , removalAt_(0)
    , gameStopped_(true)
    , removable_({0, 0})
    , time_(0)
    , goal_(0)
{
    ui_->setupUi(this);

    timer_ = new QTimer(this);
    delayTimer_ = new QTimer(this);
    scene_ = new QGraphicsScene(this);

    // initializing of the gameboard and the graphical grid:
    gameboard_.init_empty();
    initGrid();
    // joo

    // Settings for the spinBoxes.
    ui_->goalSpinBox->setMinimum(0);
    ui_->goalSpinBox->setMaximum(gridSize_*gridSize_);
    ui_->seedSpinBox->setMinimum(0);
    ui_->moveCounterSpinBox->setMinimum(0);

    timer_->setInterval(1000);
    delayTimer_->setInterval(DELAY);

    // readyPushButtonin kanssa käytettävä release-signaalia, koska kun
    // button disabloidaan silloin, kun button on painettuna, siirtyy
    // cursori spinboxiin ja estää keybindien käytön.
    connect(ui_->readyButton, &QPushButton::released, this, &MainWindow::onReady);
    connect(ui_->resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(ui_->pauseButton, &QPushButton::clicked, this, &MainWindow::onPause);
    connect(timer_, &QTimer::timeout, this, &MainWindow::onTimeOut);
    connect(delayTimer_, &QTimer::timeout, this, &MainWindow::onDelay);

    // Reset button set to disabled in the beginning:
    ui_->resetButton->setEnabled(false);

    // Setting colours for the timer lcd numbers:
    ui_->lcdNumberMin->setPalette(QPalette(Qt::red));
    ui_->lcdNumberSec->setPalette(QPalette(Qt::blue));
}


MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // Keybinds not in use when game is stopped:
    if (gameStopped_)
    {
        return;
    }

    // if moveCounter is at the value which user has selected as the removal point:
    if (moveCounter_ == removalAt_ and moveCounter_> 0)
    {
        numberRemoval(event);
        return;
    }

    // if key not in the map of available keybinds (wasd):
    if (DIRECTIONS.find(event->key()) == DIRECTIONS.end())
    {
        return;
    }

    moveCounter_ ++;

    // making of the move and if-clause for whether the goal is achieved:
    if (gameboard_.move(DIRECTIONS.at(event->key()), goal_))
    {
        ui_->infoTextBrowser->insertPlainText("You have won!");
        gameStopped_ = true;
        timer_->stop();
        updateScreen();

        // background to green when winning:
        this->setPalette(Qt::green);
    }

    else if (gameboard_.is_full())
    {
        ui_->infoTextBrowser->insertPlainText("You have lost!");
        gameStopped_ = true;
        timer_->stop();
        updateScreen();

        // background to red when losing:
        this->setPalette(Qt::red);
    }

    else
    {
        updateScreen();

        // delayTimer_ will handle updating the screen after DELAY
        // so that adding of the new_value will come later to the screen:
        delayTimer_->start(DELAY);
        gameboard_.new_value(false);
    }

}

void MainWindow::onReady()
{
    int seed = ui_->seedSpinBox->value();    
    goal_ = pow(2, ui_->goalSpinBox->value());
    removalAt_ = ui_->moveCounterSpinBox->value();

    // if no goal chosen:
    if (goal_ == 1)
    {
        goal_ = DEFAULT_GOAL;
    }

    // if seed not set:
    if (seed == 0)
    {
        return;
    }

    gameboard_.fill(seed);

    gameStopped_ = false;

    // reset button is enabled and readybutton disabled:
    ui_->resetButton->setEnabled(true);
    ui_->readyButton->setEnabled(false);

    timer_->start(1000);

    updateScreen();
}

void MainWindow::onReset()
{
    // for the case that removal is in process when reset happens:
    rectsLabels_.at(removable_.first).at(removable_.second).first->setBrush(Qt::NoBrush);
    moveCounter_ = 0;
    removable_.first = 0;
    removable_.second = 0;

    // for if game is paused when reseted:
    ui_->pauseButton->setText("Pause");

    // retrieving the settings for reset:
    goal_ = pow(2, ui_->goalSpinBox->value());
    removalAt_ = ui_->moveCounterSpinBox->value();

    // if no goal chosen:
    if (goal_ == 1)
    {
        goal_ = DEFAULT_GOAL;
    }

    time_ = 0;
    // Reseting background color to default:
    this->setPalette(QGuiApplication::palette());

    int seed = ui_->seedSpinBox->value();
    // if seed not chosen:
    if (seed == 0)
    {
        return;
    }

    gameboard_.fill(seed, true);

    gameStopped_ = false;

    timer_->start(1000);

    ui_->infoTextBrowser->clear();

    updateScreen();
}

void MainWindow::onPause()
{
    // if game in pause-mode:
    if (ui_->infoTextBrowser->toPlainText() == "Game paused!")
    {
        gameStopped_ = false;
        ui_->infoTextBrowser->clear();
        ui_->pauseButton->setText("Pause");
        timer_->start(1000);
    }

    // if game not paused and not stopped for any other reason:
    else if (not gameStopped_)
    {
        gameStopped_ = true;
        ui_->infoTextBrowser->insertPlainText("Game paused!");
        ui_->pauseButton->setText("Unpause");
        timer_->stop();
    }
}


void MainWindow::updateScreen()
{
    NumberTile* current;
    for (int y = 0; y < gridSize_; y ++)
    {
        for (int x = 0; x < gridSize_; x ++)
        {
            Coords cords = Coords(y, x);
            current = gameboard_.get_item(cords);
            int number = current->getNumber();

            // Clearing label:
            rectsLabels_.at(y).at(x).second->clear();

            // Default brush to rectItem:
            rectsLabels_.at(y).at(x).first->setBrush(Qt::NoBrush);

            if (number == 0)
            {
                continue;   
            }

            // Color that corresponds to the number of the tile set as the
            // brush of the rectItem:
            rectsLabels_.at(y).at(x).first->setBrush(findColour(number));

            // Number as QString to the label:
            QString numberQstring = QString::fromStdString(std::to_string(number));
            rectsLabels_.at(y).at(x).second->setText(numberQstring);

        }
    }
    ui_->scoreTextBrowser->clear();
    std::string score = std::to_string(gameboard_.getPoints());
    QString scoreText = "Score: " + QString::fromStdString(score);
    ui_->scoreTextBrowser->insertPlainText(scoreText);
}

void MainWindow::initGrid()
{
    // Placing the graphicsView in to the gui:
    ui_->graphicsView->setGeometry(LEFTMARGIN, TOPMARGIN,
                                   graphicSceneSize_ + 2, graphicSceneSize_ + 2);
    ui_->graphicsView->setScene(scene_);

    // Initializing scene as a square:
    scene_->setSceneRect(0, 0, graphicSceneSize_ - 1, graphicSceneSize_ - 1);

    // calculating size of a individual square (tile) in the scene:
    double squareSide = (graphicSceneSize_ - 2)/ gridSize_ - 2;

    // Setting pen for the square's edges:
    QPen pen = QPen(Qt::black);
    pen.setWidth(2);

    // Creating all the rectItems and labels inside the scene:
    for (int y = 0; y < gridSize_; y++)
    {
        rectsLabels_.push_back({});
        for (int x = 0; x < gridSize_; x++)
        {            
            rectsLabels_.at(y).push_back({});

            // Creating the rectItem:
            rectsLabels_.at(y).at(x).first = scene_->addRect(x * (squareSide + 2) + 2,
                            y * (squareSide + 2) + 2, squareSide, squareSide, pen);

            // Creating the label:
            QLabel* newLabel = new QLabel(this);
            newLabel->setFrameStyle(QFrame::Panel);
            newLabel->setAlignment(Qt::AlignCenter);
            newLabel->setGeometry(QRect(x * (squareSide + 2) + 3 + LEFTMARGIN,
                                        y * (squareSide + 2) + 3 + TOPMARGIN,
                                        squareSide, squareSide));
            rectsLabels_.at(y).at(x).second = newLabel;
        }
    }
}

void MainWindow::numberRemoval(QKeyEvent* event)
{
    // Key not a usable key:
    if (event->key() != Qt::Key_Return and DIRECTIONS.find(event->key()) == DIRECTIONS.end())
    {
        return;
    }

    int number = gameboard_.get_item(removable_)->getNumber();

    // current removable-square to default color:
    rectsLabels_.at(removable_.first).at(removable_.second).first
            ->setBrush(findColour(number));

    if (event->key() == Qt::Key_Return)
    {
        gameboard_.remove(removable_);

        // Resetting moveCounter and removable_
        moveCounter_ = 0;
        removable_.first = 0;
        removable_.second = 0;

        updateScreen();
        return;
    }
    else
    {
        Coords dir = DIRECTIONS.at(event->key());

        // moving removable_ to the chosen direction:
        removable_.first += dir.first;
        removable_.second += dir.second;

        // if new location of removable_ outside of the grid, then
        // withdrawing the move:
        if (removable_.first < 0 or removable_.first >= gridSize_ or
            removable_.second < 0 or removable_.second >= gridSize_)
        {
            removable_.first -= dir.first;
            removable_.second -= dir.second;
        }
        // Removable-square to red-color:
        rectsLabels_.at(removable_.first).at(removable_.second).first->setBrush(Qt::red);
    }
}

QBrush MainWindow::findColour(int number)
{
    // Emptysquare, no color:
    if (number == 0)
    {
        return QBrush(Qt::NoBrush);
    }

    int exponent = log2(number);

    // red-colour needs to be skipped since it is used in the removal of the number.
    if (exponent + 4 >= Qt::red)
    {
        exponent ++;
    }
    if (exponent >= 14) // from 16384 colors are the same:
    {
        return QBrush(Qt::GlobalColor(14 + 4));
    }

    else
    {
        return QBrush(Qt::GlobalColor(exponent + 4));
    }

}

void MainWindow::onTimeOut()
{
    time_++;

    int mins = time_/60;
    int secs = time_%60;

    ui_->lcdNumberMin->display(mins);
    ui_->lcdNumberSec->display(secs);
}

void MainWindow::onDelay()
{
    delayTimer_->stop();
    updateScreen();
}

