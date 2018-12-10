#ifndef NUMEM_H
#define NUMEM_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QTimer>

/*!
 * \class Numem
 * \brief a game
 *
 * you should choose a difficulty and memorize an int
 * the size of the int depends on the chosen difficulty
 * after a while the randomly generated int is hidden
 * and you should input the number you remember
 * after user submitted the result is shown
 */

class Numem : public QWidget
{
    Q_OBJECT

public:
    Numem(QWidget *parent = nullptr, unsigned rand = 5); ///< see Numem.cpp
    ~Numem();
private:
    QVBoxLayout *mainLay;                           ///< contains all the other layouts, used in this->setLayout()
    QHBoxLayout *serviceLay, *inputLay, *memLay;
    QLabel *numToRemember, *adjustLbl, *resultLbl;
    QSpinBox *difficulty;                           ///< defficulty level corresponds to a number of digits to remember (f.i. 2 level -> 2 digits to remember)
    QLineEdit *numInput;                            ///< for input the memorized number to check
    QPushButton *actionButton;                      ///< to generate a new number or submit your input
    QTimer *memorizeTimer;                          ///< implements time restriction for memorizing a generated number

    bool isGenerated;                               ///< is used to check an internal state of the game to define actionButton logics
    unsigned randSize;                              ///< size of the generated number, is used to truncate gotten random int
    QVector<QChar> curNum,                          ///< what to memorize
                   clientInput;                     ///< user's input
private slots:
    void actionButtonClicked();                     ///< see Numem.cpp
    void memorizeTimeOut();                         ///< see Numem.cpp
    void setRandSize(int rsize);                    ///< the number of digits to remember
};

#endif // NUMEM_H
