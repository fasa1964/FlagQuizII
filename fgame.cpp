#include "fgame.h"

#include <QLocale>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QRandomGenerator>

#include <QDebug>

FGame::FGame(QObject *parent)
    : QObject{parent}
{
    if(getFlagsCount() > 0)
        setFlagsAvailable(true);
    else
        setFlagsAvailable(false);


    QFile file("data.json");
    if(file.exists()){
        setBordersAvailable(true);
        setAreasAvailable(false);
        setCapitalsAvailable(true);
    }else{
        setBordersAvailable(false);
        setAreasAvailable(false);
        setCapitalsAvailable(false);
    }

    // Type of game
    gameflags = false;
    gameborders = false;
    gameareas = false;
    gamecapitals = false;

    setQuestion("This is my question");
    setAnswerA("Antwort A");
    setAnswerB("Antwort B");
    setAnswerC("Antwort C");
    setAnswerD("Antwort D");
    setSolution("Antwort C");
    setFlagPath("");

    counter = 0;
    setquestionCounter(counter);
    maxQuestion = 15;
    gamePoints = 0;

    // Setup points
    pointMap.insert(1, "50€");
    pointMap.insert(2, "100€");
    pointMap.insert(3, "200€");
    pointMap.insert(4, "500€");
    pointMap.insert(5, "1.000€");
    pointMap.insert(6, "2.000€");
    pointMap.insert(7, "4.000€");
    pointMap.insert(8, "8.000€");
    pointMap.insert(9, "16.000€");
    pointMap.insert(10, "32.000€");
    pointMap.insert(11, "64.000€");
    pointMap.insert(12, "128.000€");
    pointMap.insert(13, "256.000€");
    pointMap.insert(14, "500.000€");
    pointMap.insert(15, "1.000.000€");

}

void FGame::startFlagsGame()
{
    gameflags = true;
    gameborders = false;
    gameareas = false;
    gamecapitals = false;
    gamePoints = 0;

    questionList.clear();

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    generateQuestion();
}

void FGame::startBordersGame()
{
    gameflags = false;
    gameborders = true;
    gameareas = false;
    gamecapitals = false;
}

void FGame::startCapitalsGame()
{
    gameflags = false;
    gameborders = false;
    gameareas = false;
    gamecapitals = true;

    questionList.clear();

    if(capitalMap.isEmpty())
        capitalMap = generateCapitalMap();
}

void FGame::startNextQuestion()
{
    generateQuestion();
}

void FGame::cancelGame()
{
    gameflags = false;
    gameborders = false;
    gameareas = false;
    gamecapitals = false;
    gamePoints = 0;

    questionList.clear();
    answerList.clear();

    counter = 0;
    setquestionCounter(counter);

}

void FGame::setJoker50()
{
    QList<int> randomList;
    int counter = 0;

    while (counter < 2) {

        int r = getRandomNumber(4);
        while (randomList.contains(r)) {
            r =  getRandomNumber(4);
        }

        if(r == 0 && m_answerA != m_solution){
            setAnswerA("");
            counter++;
        }

        if(r == 1 && m_answerB != m_solution){
            setAnswerB("");
            counter++;
        }

        if(r == 2 && m_answerC != m_solution){
            setAnswerC("");
            counter++;
        }

        if(r == 3 && m_answerD != m_solution){
            setAnswerD("");
            counter++;
        }

        randomList << r;
    }
}

void FGame::setJokerPub()
{
    if(m_answerA == m_solution)
        emit selectButton("A");

    if(m_answerB == m_solution)
        emit selectButton("B");

    if(m_answerC == m_solution)
        emit selectButton("C");

    if(m_answerD == m_solution)
        emit selectButton("D");
}

void FGame::setJokerTel()
{
    if(m_questionCounter <= 6){

        if(m_answerA == m_solution)
            emit selectButton("A");

        if(m_answerB == m_solution)
            emit selectButton("B");

        if(m_answerC == m_solution)
            emit selectButton("C");

        if(m_answerD == m_solution)
            emit selectButton("D");
    }

    if(m_questionCounter > 6){

        int r = getRandomNumber(2);

        if(r == 0){
            if(m_answerA == m_solution)
                emit selectButton("A");

            if(m_answerB == m_solution)
                emit selectButton("B");

            if(m_answerC == m_solution)
                emit selectButton("C");

            if(m_answerD == m_solution)
                emit selectButton("D");
        }

        if(r == 1){

            int v = getRandomNumber(4);
            if(v == 0)
                emit selectButton("A");

            if(v == 1)
                emit selectButton("B");

            if(v == 2)
                emit selectButton("C");

            if(v == 3)
                emit selectButton("D");
        }
    }
}

void FGame::setAnswer(const QString &answer)
{

    if(answer == solution()){
        gamePoints++;
        emit credits( pointMap.value(gamePoints) );
    }

}

bool FGame::flagsAvailable() const
{
    return m_flagsAvailable;
}

void FGame::setFlagsAvailable(bool newFlagsAvailable)
{
    if (m_flagsAvailable == newFlagsAvailable)
        return;
    m_flagsAvailable = newFlagsAvailable;
    emit flagsAvailableChanged();
}

bool FGame::bordersAvailable() const
{
    return m_bordersAvailable;
}

void FGame::setBordersAvailable(bool newBordersAvailable)
{
    if (m_bordersAvailable == newBordersAvailable)
        return;
    m_bordersAvailable = newBordersAvailable;
    emit bordersAvailableChanged();
}

bool FGame::areasAvailable() const
{
    return m_areasAvailable;
}

void FGame::setAreasAvailable(bool newAreasAvailable)
{
    if (m_areasAvailable == newAreasAvailable)
        return;
    m_areasAvailable = newAreasAvailable;
    emit areasAvailableChanged();
}

bool FGame::capitalsAvailable() const
{
    return m_capitalsAvailable;
}

void FGame::setCapitalsAvailable(bool newCapitalsAvailable)
{
    if (m_capitalsAvailable == newCapitalsAvailable)
        return;
    m_capitalsAvailable = newCapitalsAvailable;
    emit capitalsAvailableChanged();
}

QVariant FGame::question() const
{
    return m_question;
}

void FGame::setQuestion(const QVariant &newQuestion)
{
    if (m_question == newQuestion)
        return;
    m_question = newQuestion;
    emit questionChanged();
}

QString FGame::flagPath() const
{
    return m_flagPath;
}

void FGame::setFlagPath(const QString &newFlagPath)
{
    if (m_flagPath == newFlagPath)
        return;
    m_flagPath = newFlagPath;
    emit flagPathChanged();
}

QString FGame::solution() const
{
    return m_solution;
}

void FGame::setSolution(const QString &newSolution)
{
    if (m_solution == newSolution)
        return;
    m_solution = newSolution;
    emit solutionChanged();
}

QString FGame::answerA() const
{
    return m_answerA;
}

void FGame::setAnswerA(const QString &newAnswerA)
{
    if (m_answerA == newAnswerA)
        return;
    m_answerA = newAnswerA;
    emit answerAChanged();
}

QString FGame::answerB() const
{
    return m_answerB;
}

void FGame::setAnswerB(const QString &newAnswerB)
{
    if (m_answerB == newAnswerB)
        return;
    m_answerB = newAnswerB;
    emit answerBChanged();
}

QString FGame::answerC() const
{
    return m_answerC;
}

void FGame::setAnswerC(const QString &newAnswerC)
{
    if (m_answerC == newAnswerC)
        return;
    m_answerC = newAnswerC;
    emit answerCChanged();
}

QString FGame::answerD() const
{
    return m_answerD;
}

void FGame::setAnswerD(const QString &newAnswerD)
{
    if (m_answerD == newAnswerD)
        return;
    m_answerD = newAnswerD;
    emit answerDChanged();
}

int FGame::questionCounter() const
{
    return m_questionCounter;
}

void FGame::setquestionCounter(int newquestionCounter)
{
    if (m_questionCounter == newquestionCounter)
        return;
    m_questionCounter = newquestionCounter;
    emit questionCounterChanged();
}

int FGame::getFlagsCount()
{
    QDir dir;
    QStringList list = dir.entryList( QStringList() << "*.svg", QDir::Files);
    return list.count();
}

QMap<QString, QString> FGame::generateCountrieCodesMap()
{
    QMap<QString, QString>map;

    QFile file("codes.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString f = file.fileName();
        QString e = file.errorString();
        QString text = QString("Could not open %1 for reading: %2").arg(f, f.size() ).arg(e, e.size());
        emit errorMessage(text);
        return map;
    }


    QTextStream file_text(&file);
    QString jsonString = file_text.readAll();

    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

    if(jsonDoc.isNull())
        return map;

    if(jsonDoc.isArray())
        return map;

    if(jsonDoc.isObject()){
        QJsonObject jsonObject = jsonDoc.object();

        if(!jsonObject.isEmpty()){

            foreach (QString code, jsonObject.keys()) {

                QString country = jsonObject.value(code).toString();
                map.insert(code, country);
            }
        }
    }

    return map;
}

QMap<QString, QString> FGame::generateFlagMap()
{
    QMap<QString, QString>map;

    if(countrieCodesMap.isEmpty())
        countrieCodesMap = generateCountrieCodesMap();

    QMapIterator<QString, QString> it(countrieCodesMap);
    while (it.hasNext()) {
        it.next();

        QString key = it.key();

        if(key.size() <= 2){

            QString filepath = QDir::currentPath() + "/" + key +".svg"; // File of flag

            QFile file(filepath);
            if(file.exists())
                map.insert(key, filepath);
            }
    }

    return map;
}

QMap<QString, QString> FGame::generateCapitalMap()
{
    QMap<QString, QString>map;

    if(countrieCodesMap.isEmpty())
            countrieCodesMap = generateCountrieCodesMap();


    QMapIterator<QString, QString> it(countrieCodesMap);
    while (it.hasNext()) {
            it.next();

            QString alpha2 = it.key();
            QVariant capital = readData(alpha2, "capital");
            //qDebug() << "Code: " << alpha2 << ":" << capital;
            if(capital.isValid()){



            }
    }



    return map;
}

void FGame::generateQuestion()
{
    if(counter >= maxQuestion){
        emit gameOver( pointMap.value(gamePoints));
        return;
    }

    counter++;
    setquestionCounter( counter );



    QString type = "question";
    if(gameflags){

        QString key = getRandomCountrieCode(flagMap.count()-1, type);
        QString path = flagMap.value(key);
        setFlagPath(path);

        QString countrie = countrieCodesMap.value(key);
        setSolution(countrie);
    }

    generateAnswers();
}

void FGame::generateAnswers()
{
    QString type = "answer";
    answerList.clear();

    QMap<int, QString> posList;
    setAnswerA("");
    setAnswerB("");
    setAnswerC("");
    setAnswerD("");

    if(gameflags){

        QString solutionKey = countrieCodesMap.key(solution());
        answerList << solutionKey;

        for(int i = 0; i < 3; i++){
            QString key = getRandomCountrieCode(flagMap.size()-1, type);
            QString country = countrieCodesMap.value(key);


            int pos = getRandomNumber(4);
            while (posList.contains(pos)) {
                pos = getRandomNumber(4);
            }
            posList.insert(pos, country);

            if(pos == 0)
                setAnswerA(country);

            if(pos == 1)
                setAnswerB(country);

            if(pos == 2)
                setAnswerC(country);

            if(pos == 3)
                setAnswerD(country);

        }

        if(posList.value(0).isEmpty())
            setAnswerA(solution());
        if(posList.value(1).isEmpty())
            setAnswerB(solution());
        if(posList.value(2).isEmpty())
            setAnswerC(solution());
        if(posList.value(3).isEmpty())
            setAnswerD(solution());

    }
}

QString FGame::getRandomCountrieCode(int max, QString &type)
{
    QString key;

    int nr = getRandomNumber(max);
    key = countrieCodesMap.keys().at(nr);

    if(type == "question"){
        while (questionList.contains(key)) {
            nr = getRandomNumber(max);
            key = countrieCodesMap.keys().at(nr);
        }

        questionList << key;
    }

    if(type == "answer"){
        while (answerList.contains(key)) {
            nr = getRandomNumber(max);
            key = countrieCodesMap.keys().at(nr);
        }

        answerList << key;
    }


    return key;
}

int FGame::getRandomNumber(int max)
{
    quint32 v = QRandomGenerator::system()->bounded(max);
    return v;
}

QVariant FGame::readData(const QString &alpha2, const QString &key)
{
    QVariant result;

    QFile file("data.json");

    if (!file.open(QIODevice::ReadOnly)) {
        QString f = file.fileName();
        QString e = file.errorString();
        QString text = QString("Could not open %1 for reading: %2").arg(f, f.size() ).arg(e, e.size());
        emit errorMessage(text);
        return result;
    }

    QTextStream file_text(&file);
    QString jsonString = file_text.readAll();

    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

    if(jsonDoc.isNull())
        return result;

    if(jsonDoc.isArray()){

        QJsonArray jsonArray = jsonDoc.array();

        for(int i = 0; i < jsonArray.size(); i++){

            QJsonObject obj = jsonArray.at(i).toObject();

            if(!obj.isEmpty()){

                QString code =  obj.value("iso").toObject().value("alpha_2").toString();

                if(code == alpha2){

                    result = obj.value(key).toVariant();
                    break;
                }

            }
        }
    }

    return result;
}
