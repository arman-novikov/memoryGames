#include "NumPairs.h"
#include <ctime>
#include <QList>

static const int MAX_PLATES_COUNT = 20;
static const int COLUMN_COUNT = 4; ///< number of Plates columns
static const QString INITIAL_TIME_LBL_VALUE("00:00:00");
static const QString INITIAL_CLICK_LBL_VALUE("clicks: 0");

/*!
 * \brief generates a vector of 'size' size with values for Plates
 * \param <T> a type of a Plate's value (int, float or anything else)
 * \param [out] container an empty vector to save and transmit generated values
 * \param [in] size how many values is required
 */
template <typename T = int>
static void platesValuesGenerator(QVector<T> &container, const size_t size)
{
    for (size_t i = 0; i < size; ++i)
        container.append(T(i));
}

/*!
 * \brief initialize a NumPairs object's attributes
 * \param [in] parent just to use Qt memory menagement system
 */
NumPairs::NumPairs(QWidget *parent)
    : QWidget(parent), isOn(false), clicksCounter(0)
{
    timer = new QTimer(this);

    resultLay = new QHBoxLayout();
        passedTimeLbl = new QLabel(INITIAL_TIME_LBL_VALUE);
        clicksNumLbl = new QLabel(INITIAL_CLICK_LBL_VALUE);

        resultLay->addWidget(passedTimeLbl);
        resultLay->addWidget(clicksNumLbl);

    adjustLay = new QHBoxLayout();
        difficultLbl = new QLabel(QString("choose difficulty: "));
        difficultSpinBox = new QSpinBox();
            difficultSpinBox->setMinimum(1);
            difficultSpinBox->setMaximum(5);
            difficultSpinBox->setValue(1);
        startButton = new QPushButton("start");

        adjustLay->addWidget(difficultLbl);
        adjustLay->addWidget(difficultSpinBox);
        adjustLay->addWidget(startButton);

    platesLay = new QGridLayout();

    statusLbl = new QLabel(QString("click \'start\' to begin"));
    statusLbl->setAlignment(Qt::AlignCenter);
    mainLay = new QVBoxLayout();
        mainLay->addWidget(statusLbl);
        mainLay->addLayout(resultLay);
        mainLay->addLayout(adjustLay);
        mainLay->addLayout(platesLay);

    setLayout(mainLay);

    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startButtonClicked()));
    connect(timer, SIGNAL(timeout()), this, SLOT(passedTimeLblUpdate()));

    setFixedSize(QSize(270, 150));
}

/*!
 * \brief generates and emplace Plates for the game
 *
 * according to a choosen difficulty up to 20 Plates are to be
 *      generated and situated (4 columns and several rows)
 *
 * in result up to 20 Plates (without any values) are generated and situated
 */
void NumPairs::platesCreator()
{
    for (auto &plate: plates)                   ///< first of all delete all the previous Plates
        this->platesLay->removeWidget(plate);

    this->plates.clear();                       ///< clean Plates container
    this->plates.reserve(MAX_PLATES_COUNT);     ///< and reserve memory for new ones

    /// generating Plates: 4 columns and several rows (depends on difficulty)
    for(int i = 0; i < difficultSpinBox->value() * COLUMN_COUNT; ++i) {
        Plate *plate = new Plate();
        plates.append(plate);
        platesLay->addWidget(plates[i], i / COLUMN_COUNT, i % COLUMN_COUNT);
        plate->close();                                                     ///< the initial Plate's state is closed
        plate->setEnabled(false);                                           ///< before clicking start Plates are disabled for clicking
        connect(plate, SIGNAL(clicked(bool)), this, SLOT(plateClicked()));  ///< set a Plate clicker processor
    }
}

/*!
 * \brief a private slot to process clicks on the startButton
 */
void NumPairs::startButtonClicked()
{
    const int mainWindowWidth = 270;
    const int mainWindowHeigth = 125 + 50 * this->difficultSpinBox->value();
    const QSize mainWindowSize(mainWindowWidth, mainWindowHeigth);

    for (auto &plate: plates)       ///< hide all the previous (if exist) Plates
      plate->hide();

    this->platesCreator();          ///< create new Plates
    this->platesFiller();           ///< fill them with values
    this->isOn = true;              ///< set flag that the game is launched
    for (auto &plate: plates)       ///< let user click the Plates
        plate->setEnabled(true);

    this->setFixedSize(mainWindowSize);             ///> expand NumPairs widget to a MainWindow's size
    passedTimeLbl->setText(INITIAL_TIME_LBL_VALUE); ///> set initial values of measuring widgets
    clicksNumLbl->setText(INITIAL_CLICK_LBL_VALUE);
    statusLbl->setText(QString(""));
    clicksCounter = 0;
    startButton->setText("restart");                ///> user can start a new game clicking startButton
    time.restart();                                 ///> launch the timer
    timer->start(100);
}

void NumPairs::plateClicked()
{
    static int openedCount = 0;                                    ///> how many Plates are currently opened
    const QString clicksCounterLblValue = QString("clicks: %1")
                                            .arg(++clicksCounter); ///> represent number of clicks on Plates
    Plate *clickedPlate = qobject_cast<Plate*>(sender());          ///> get a signal sender

    if (openedCount >= 2) {         ///> if there are 2 or more currently opened they must be closed
        for(auto &plate: plates)
            if (plate->isEnabled())
                plate->close();

        openedCount = 0;            ///> now no one is opened
    }

    if (clickedPlate->isOpened()) { ///> if was opened should be closed
        clickedPlate->close();
        --openedCount;
    } else {                        ///> if was closed should be opened
        clickedPlate->open();
        ++openedCount;
    }

    checker(clickedPlate);                          ///> check opened Plates whether some of them are matched
    clicksNumLbl->setText(clicksCounterLblValue);   ///> show how many clicks have been done
}

/*!
 * \brief checks opened Plate whether their values are the same
 * \param [in] plate a Plate object that was clicked and is to be checked
 *      whether one more with the same value is opened
 *
 * if there are no Plates left to open and match
 *  the play is done
 */
void NumPairs::checker(Plate *plate)
{
    bool isDone = true;

    for (auto &iter: plates) {      ///> check whether there is one more Plate with the same value opened
        if (iter->isOpened() &&
            iter->isEnabled() &&
            iter != plate &&
            iter->getValue() == plate->getValue())
        {
            iter->setEnabled(false);    ///> if found
            plate->setEnabled(false);   ///> make both opened and disabled (a Pair is done)
        }

        if ( !(iter->isOpened()) ) ///> if at least one Plate is closed
            isDone = false;        ///> the game is not completed
    }

    if (isDone) {                                       ///> if isDone == true the game is done
        timer->stop();                                  ///> stop the timer
        this->startButton->setText(QString("start"));   ///> offer a new game
        statusLbl->setText(QString("you\'ve done"));    ///> let user know they have won
    }
}

/*!
 * \brief set Plates' values
 *
 *  randomly chooses a Plate and set its value
 *  in result there are several pairs of Plates with the same values
 *  situated in different (each time) places of the grid of the main Layout
 */
void NumPairs::platesFiller()
{
    const int placesCount = this->plates.size();    ///> how many Plates to fill with values
    const int valuesCount = placesCount / 2;        ///> how many Pairs of Plates there are to be
    QList<int> places;                              ///> a container of Plates' places (for random choosing and setting its value)
    QVector<int> values;                            ///> generated values container

    places.reserve(placesCount);                    ///> reverse memory for append operations
    values.reserve(valuesCount);

    for (int i = 0; i < placesCount; ++i)           ///> initiate all the places of Plates
        places.append(i);

    platesValuesGenerator(values, size_t(valuesCount));     ///> get values for Plate::setValue()

    srand(unsigned(std::time(nullptr)));            ///> set seed for rand()
    for (auto it: values)                           ///> have to set all the values twice (Pairs)
        for (int i = 0; i < 2; ++i) {               ///> there are to be 2 randomly chosen Plates with the same value
            int place = rand() % places.size();     ///> choose a random cell in places containing a place
            int plateNum = places[place];           ///> take the randomly choosen cell's value (a place)

            plates[plateNum]->setValue(it);         ///> set value for a randomly choosen Plate
            places.erase(places.begin() + place);   ///> and delete the choosen place from queue of places of Plates
        }
}

/*!
 * \brief evaluates passed time and shows it to user in passedTimeLbl widget
 */
void NumPairs::passedTimeLblUpdate()
{
    const qint64 timePassed_sec = time.elapsed() / 1000;                    ///> get elapsed time from the beginig in secs
    const qint64 hours = timePassed_sec / 3600;                             ///> get hours
    const qint64 minutes = (timePassed_sec - hours * 3600) / 60;            ///> get minutes
    const qint64 seconds = timePassed_sec - hours * 3600 - minutes * 60;    ///> get seconds

    const QString res = QString("%1:%2:%3")                                 ///> parse a QString for passedTimeLbl->setText()
                    .arg(hours, 2, 10, QChar('0'))
                    .arg(minutes, 2, 10, QChar('0'))
                    .arg(seconds, 2, 10, QChar('0'));                       ///> something like this 07:08:09

    passedTimeLbl->setText(res);
}

NumPairs::~NumPairs()
{

}
