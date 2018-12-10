#ifndef NUMPAIRS_H
#define NUMPAIRS_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QSpinBox>
#include <QTimer>
#include <QTime>


class Plate;

/*!
 * \brief a game
 *
 * user has several couples of numbers
 *       shown in the widget as clickable buttons
 *
 * number of such couples depends on the chosen difficulty
 * the initial state of Plates (buttons) is closed ("X" is shown)
 * user can open (to see the value of a Plate) a Plate
 * only two Plates can be opened in the same time
 * if user opens a third Plate 2 previous ones are to be closed (setText("X))
 * if user opens two Plates with the same value they remain opened and get disabled for clicks
 * if there are no closed Plate user wins
 * clicks and time are being counted
 *
 * see NumPairs.cpp
 */
class NumPairs : public QWidget
{
    Q_OBJECT
public:
    NumPairs(QWidget *parent = nullptr);
    ~NumPairs() override;
private slots:
    void plateClicked();
    void startButtonClicked();
    void passedTimeLblUpdate();
private:
    void platesCreator();
    void platesFiller();
    void checker(Plate*);

    QHBoxLayout *resultLay, *adjustLay;
    QGridLayout *platesLay;
    QVBoxLayout *mainLay;
    QLabel *difficultLbl, *clicksNumLbl, *passedTimeLbl, *statusLbl;
    QSpinBox *difficultSpinBox;
    QPushButton *startButton;
    QVector<Plate*> plates;
    QTimer *timer;
    QTime time;
    bool isOn;
    int clicksCounter;
};

/*!
 * \brief Plate is a QPushButton with some value
 *
 * it has a number witch can be shown(opened) or
 *      replaced with 'X' (closed) throu this->setText(("X"/"number"))
 */
class Plate: public QPushButton
{
    Q_OBJECT
public:
    /*!
     * \brief a Plate object's attributes initializator
     * \param [in] parent just to implement a memory management
     * \param [in] value what is to be shown on the Button
     */
    Plate(QWidget *parent = nullptr, int value = -1):
        QPushButton(parent), _value(value), _isOpened(false)
    {
        this->setFixedSize(QSize(50,50));
    }
    /*!
     * \brief Java stylish getter
     * \return the value of the Plate
     */
    int getValue() const {return _value;}

    /*!
     * \brief Java stylish setter
     * \param [in] a value to set
     */
    void setValue(const int value) {_value = value;}

    /*!
     * \brief Java stylish getter
     * \return is a Plate's text the value or "X"
     */
    bool isOpened() {return _isOpened;}

    /*!
     * \brief change "X" to the value to display as the Plate's text
     */
    void open()
    {
        this->setText(QString::number(this->getValue()));
        _isOpened = true;
    }

    /*!
     * \brief change the value to "X" to display as the Plate's text
     */
    void close()
    {
        this->setText(QString("X"));
         _isOpened = false;
    }

private:
    int _value;
    bool _isOpened;
};

#endif // NUMPAIRS_H
