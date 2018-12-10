#ifndef IGAME_H
#define IGAME_H

#include "Numem.h"
#include "NumPairs.h"
#include <QMainWindow>

/*!
 * \file igame.h
 * \brief this file contains Creator and ConcreteCreators according to Factory Method pattern
 *
 * in order to add a new game it's necessary to design a new Class (gameName)Game as the IGame subclass
 * and override virtual method createGame(QWidget* _gameWidget) where _gameWidget is to be initialized
 * using a gameClass constructor in the new expression
 */
class IGame: public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief IGame Interface class
     * \param [in] parent is used to use Qt memory menagement system,
     *             and as a _parent initializer
     */
    IGame(QMainWindow *parent): QWidget(parent), _parent(parent) {}
    virtual ~IGame() {}
    /*!
     * \brief getName returns a Name to display in the MainWindow's menubar
     * \return QString - the Game's name
     */
    virtual QString getName() const = 0;
public slots:
    /*!
     * \brief playGame responsible for game launching
     *
     * is used as a getProduct() method in Fabric Method pattern
     */
    void playGame()
    {
        QWidget* _gameWidget = nullptr;
        createGame(_gameWidget);            ///< a game pointer initializing

        if (_gameWidget) {
            _gameWidget->setAttribute(Qt::WA_DeleteOnClose);
            _parent->setCentralWidget(_gameWidget);
        } else {
            //todo: add an error handler
        }
    }
protected:
    /*!
     * \brief createGame a custom gameClass' object's initializer
     * \param [out] _gameWidget is used to transmit a gameClass object
     */
    virtual void createGame(QWidget* &_gameWidget) = 0;
    QMainWindow *_parent;   ///< is used to get the method setCentralWindow() of the MainWndow object
};

class NumemGame : public IGame
{
    Q_OBJECT
public:
    NumemGame(QMainWindow *parent): IGame(parent) {}
    ~NumemGame() override {}
    QString getName() const override
    {
        return QString("Numem");
    }
protected:
    void createGame(QWidget* &_gameWidget) override
    {
        _gameWidget = new Numem(_parent);
    }

};

class NumPairsGame : public IGame
{
    Q_OBJECT
public:
    NumPairsGame(QMainWindow *parent): IGame(parent) {}
    ~NumPairsGame() override {}
    QString getName() const override
    {
        return QString("NumPairs");
    }
protected:
    void createGame(QWidget* &_gameWidget) override
    {
        _gameWidget = new NumPairs(_parent);
    }

};

#endif // IGAME_H
