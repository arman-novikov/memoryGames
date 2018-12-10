#include "Numem.h"
#include <cstdlib>
#include <ctime>

const int MEMORIZING_TIME = 5000; ///< time for user to memorize the number in mlsec

/*!
 * \brief initialize widgets and other attributes of a Numem object
 * \param [in] parent is used to delegate memory management
 * \param [in] rand default size of a number to remember
 */
Numem::Numem(QWidget *parent, unsigned rand)
    : QWidget(parent), isGenerated(false), randSize(rand)
{
    memorizeTimer = new QTimer(this);

    difficulty = new QSpinBox(this);
    difficulty->setDisplayIntegerBase(10);
    difficulty->setMaximum(10);
    difficulty->setFixedWidth(50);
    difficulty->setValue(int(rand));

    adjustLbl = new QLabel("difficulty", this);
    numToRemember = new QLabel(this);
    resultLbl = new QLabel(this);
    numInput = new QLineEdit(this);

    actionButton = new QPushButton(this);
    actionButton->setText("generate a number");
    actionButton->setFixedWidth(150);

    serviceLay = new QHBoxLayout();
    serviceLay->addWidget(adjustLbl);
    serviceLay->addWidget(difficulty);

    memLay = new QHBoxLayout();
    memLay->addWidget(numToRemember);
    memLay->addWidget(resultLbl);

    inputLay = new QHBoxLayout();
    inputLay->addWidget(numInput);
    inputLay->addWidget(actionButton);

    mainLay = new QVBoxLayout(this);
    mainLay->addLayout(serviceLay);
    mainLay->addLayout(memLay);
    mainLay->addLayout(inputLay);

    this->setLayout(mainLay);

    connect(actionButton, SIGNAL(clicked(bool)), this, SLOT(actionButtonClicked()));
    connect(difficulty, SIGNAL(valueChanged(int)), this, SLOT(setRandSize(int)));
    connect(memorizeTimer, SIGNAL(timeout()), this, SLOT(memorizeTimeOut()));

    setFixedSize(QSize(270, 150));
}

/*!
 * \brief a function generating a random int as a QString
 * \param [in] size how many digits is required in return
 * \return QString object containing a random number
 */
static QString getRandomString(unsigned size)
{
    srand(unsigned(std::time(nullptr)));    ///< system time as a seed
    QString res = QString::number(rand());  ///< get a random int and save it as a QString object
    res.truncate(int(size));                ///< adjust the size (rand() returns 10 digits number)
    return res;
}

/*!
 * \brief checks time given a user to memorize the number
 *
 * after time expires memorizeTimeOut() hiddens the number
 *      by replacing the number with '*'s
 * enables widgets for interaction
 */
void Numem::memorizeTimeOut()
{
    QString forFill;
    forFill.reserve(10); ///< the max size is 10

    for (int i = 0; i < curNum.size(); ++i)
        forFill.append("*");
    numToRemember->setText(forFill);

    numInput->setEnabled(true);
    actionButton->setEnabled(true);
    memorizeTimer->stop();
}

/*!
 * \brief just a Java styled setter
 * \param [in] rsize size of the current number for memorizing in digits
 */
void Numem::setRandSize(int rsize)
{
    randSize = unsigned(rsize);
}

/*!
 * \brief implements the main logics of the game
 * \todo split the function to several, more 'singleResponsible' ones
 *
 * if actionButton is clickled after the number was generated:
 *      (functions behaves as a checker)
 *      shows the number ('*'s are replaced by digits of the number)
 *      count errors
 *      output the result (f.i. 'excellent')
 *      set interface ready for another game playing
 *
 * else (if the number wasn't generated and button is pushed)
 *      (function behaves as a playmaker)
 *      generates a number
 *      sets widgets to their correspondant states
 *      launches timer measuring time to memorize
 *
 */
void Numem::actionButtonClicked()
{
    if (isGenerated) {
        QString forFill;                        ///< QWidget::setText takes QString objects only
        forFill.reserve(10);                    ///< prepare a place for a number
        for (int i = 0; i < curNum.size(); ++i)
            forFill.append(curNum[i]);          ///< QVector<QChar> into QString
        numToRemember->setText(forFill);        ///< finally set text to show a number that was generated

        int errorsCounter = 0; ///< checking number of errors
        for (int i = 0; i < curNum.size(); ++i)
            if (curNum[i] != numInput->text()[i])
                ++errorsCounter;

        if (errorsCounter == 1)                                            ///< if one then 'error'
            resultLbl->setText(QString::number(errorsCounter) + " error");
        else if (errorsCounter)                                            ///< if plural then 'errors'
            resultLbl->setText(QString::number(errorsCounter) + " errors");
        else                                                               ///< if no errors then just excellent
            resultLbl->setText("excellent");

        /// prepare widgets for a next playing
        actionButton->setText("generate a number"); ///< now actionButton is responsible for generation, not checking
        actionButton->setEnabled(true);             ///< ready to generate a new number
        numInput->setEnabled(false);                ///< user can't input anything, because nothing was generated
        difficulty->setEnabled(true);               ///<  user can change difficulty
        isGenerated = false;                        ///< sets flag == 'nothing is generated'
    } else {
        QString random = getRandomString(randSize); ///< get a new generated number for memorising
        curNum.erase(curNum.begin(), curNum.end()); ///< erase the previous one

        for (int i = 0; i < random.size(); ++i)     ///< save it
            curNum.append(random[i]);

        numToRemember->setText(random);             ///< show the generated number to user
        numInput->setText("");                      ///< set user's widget for input clear
        numInput->setEnabled(false);                ///< user can't input while the timer doesn't expire

        difficulty->setEnabled(false);              ///< after generating can't change difficulty
        actionButton->setText("check");             ///< now actionButton is responsible for checking, not generation
        actionButton->setEnabled(false);            ///< user can't submit while the timer doesn't expire
        resultLbl->setText("");                     ///< clear result's label
        isGenerated = true;                         ///< set flag == 'the number was generated'
        memorizeTimer->start(MEMORIZING_TIME);      ///< launch the timer
    }
}

Numem::~Numem()
{

}
