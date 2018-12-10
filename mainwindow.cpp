#include "igame.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QMenuBar>

/*!
 * \brief MainWindow::MainWindow initializes the mainWindow's attributes
 * \param [in] parent for Qt memory management using
 *
 * provides interface to produce games
 * all producers are stored in the vector "_game"
 * add a new game producer object in _game initializer {} to add a new game
 *    using a new instruction of memory allocating
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _games = {new NumPairsGame(this), new NumemGame(this)};  ///< games producers: add a new game producer here
    setWindowTitle(QString("Games of Memory"));
    setFixedSize(QSize(270, 400));

    createMenuBar();        ///> see createMenuBar() implementation
    totalConnect();         ///> see totalConnect() implementation
}

/*!
 * \brief MainWindow::createMenuBar creates menu and situates actions buttons
 */
void MainWindow::createMenuBar()
{
    about = new QMenu(QString("About"),this);                   ///> about authors action button in about menu
        Authors = new QAction(QString("&Authors"), this);
        Authors->setShortcut(Qt::Key_F1);
        about->addAction(Authors);

    selectGame = new QMenu(QString("&Select game"),this);       ///> menu to choose a game to play
        _gamesActions.reserve(_games.size());                   ///> the number of games is not hardcoded
        for (auto iter: _games) {
            QString text("play " + QString(iter->getName()));   ///> get a name of each game
            QAction *newGameAction = new QAction(text, this);   ///> create an action to choose a game to play
            _gamesActions.append(newGameAction);                ///> save the generated action button in actions' container
            selectGame->addAction(newGameAction);               ///> add an action button to choose a game
        }

   menuBar()->addMenu(selectGame);
   menuBar()->addMenu(about);
}

/*!
 * \brief MainWindow::totalConnect all signals and slots connected in this method
 */
void MainWindow::totalConnect()
{
    /// connect gameAction buttons and correspondant member of the _games vector
    /// one to one connection
    for (int i = 0; i < _games.size(); ++i) {                           ///> necessary connect each game with it's own action button
        connect(_gamesActions[i], &QAction::triggered, _games[i],       ///> an action for a game
                [this, i](){_games[i]->playGame();});                   ///> playGame() is a producing method
    }
    connect(Authors, SIGNAL(triggered(bool)), this, SLOT(authorsSlot())); ///> an action to show about authors message box
}

/*!
 * \brief MainWindow::authorsSlot shows a message box with info about authors
 */
void  MainWindow::authorsSlot()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->setWindowTitle(QString("Authors"));
    msg->setText(QString("designed and developed by IU-6's student"));
    msg->show(); // or exec()?
}

MainWindow::~MainWindow()
{

}
