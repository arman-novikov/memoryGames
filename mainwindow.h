#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "igame.h"
#include <QMenu>
#include <QAction>

/*!
 * \brief The MainWindow class is used as GameClasses' container
 *
 * provides user's interface to choose game, and triggers launching of the choosen game
 * also, can provide some additional info (about and etc.)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief MainWindow see mainwindow.cpp
     *
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();                   ///> see mainwindow.cpp
    void totalConnect();                    ///> see mainwindow.cpp
    QMenu *selectGame;
        QVector<QAction*> _gamesActions;    ///> for game choosing menu
        QVector<IGame*> _games;             ///> contains games' producers
    QMenu *about;                           ///> provides a menu to get about info
        QAction *Authors;                   ///> provides info about authors
private slots:
        void authorsSlot();                 ///> a slot displaying info about authors
};

#endif // MAINWINDOW_H
