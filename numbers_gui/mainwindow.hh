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
 *  Tiedoston kirjoittaja:
 *  Nimi: Aaro Niinistö
 *  E-Mail: aaro.niinisto@aalto.fi
 *
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH


#include "gameboard.hh"
#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QLabel>
#include <utility>
#include <map>
#include <QTimer>

// time differrence (ms) between moving the numbers and
// placing a new 2.
const int DELAY = 150;
const int LEFTMARGIN = 40;
const int TOPMARGIN = 100;
const int GRAPHICSCENESIDE = 300;

const std::map<int, Coords> DIRECTIONS{
    {87, {-1, 0}},
    {65, {0, -1}},
    {83, {1, 0}},
    {68, {0, 1}}
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event) override;


private slots:
    // handles starting of the game.
    void onReady();

    // handles resetting of the game.
    void onReset();

    // handles pausing and unpausing of the game.
    void onPause();

    // handles the timer in the game.
    void onTimeOut();

    // handles the delay of moving the numbers and creating new number-2's.
    void onDelay();



private:
    // Makes the gui-gameboard match the gameboard in the gameboard_-object.
    void updateScreen();

    // Initializes the grid which potrays the gameboard.
    void initGrid();

    // Handles gui of the number removal.
    void numberRemoval(QKeyEvent* event);

    // finds colour of a specific number which is given as a parameter.
    // Returns QBrush-class item of that color.
    QBrush findColour(int number);

    Ui::MainWindow *ui_;

    // GraphicsScene for the gamegrid:
    QGraphicsScene *scene_;

    // RectItems and QLabels which form the gamegrid:
    std::vector<std::vector<std::pair<QGraphicsRectItem*, QLabel*>>> rectsLabels_;

    // Sidelength of the gamegrid:
    const int graphicSceneSize_;

    // sqrt of the number of tiles in the gamegrid:
    int gridSize_;

    // counts moves for the removal-option:
    unsigned int moveCounter_;

    // the moveCount at which removal happens:
    unsigned int removalAt_;

    // records the current state of the game:
    GameBoard gameboard_;

    bool gameStopped_;

    // coordinates of the tile which is currently to be removed:
    Coords removable_;

    // QTimer-object for the timer in the game:
    QTimer* timer_;

    // QTimer-object for the delay of the adding of the number-2's:
    QTimer* delayTimer_;

    // records current time for the timer-featur:
    int time_;

    // records the target value of the game:
    int goal_;




};
#endif // MAINWINDOW_HH
