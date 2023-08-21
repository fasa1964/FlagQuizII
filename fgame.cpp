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
        setAreasAvailable(true);
        setCapitalsAvailable(true);
        setContinentAvailable(true);
    }else{
        setBordersAvailable(false);
        setAreasAvailable(false);
        setCapitalsAvailable(false);
        setContinentAvailable(false);
    }

    // Type of game
    gameflags = false;
    gameborders = false;
    gameareas = false;
    gamecapitals = false;
    gamecontinent = false;
    setFlags(false);


    setQuestion("This is my question");
    setAnswerA("Antwort A");
    setAnswerB("Antwort B");
    setAnswerC("Antwort C");
    setAnswerD("Antwort D");
    setSolution("Antwort C");
    setFlagPath("");

    counter = 0;
    setQuestionCounter(counter);
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

    // Define continent map
    continentCodeMap.insert("af", "Afrika");
    continentCodeMap.insert("sa", "Süd Amerika");
    continentCodeMap.insert("na", "Nord Amerika");
    continentCodeMap.insert("eu", "Europa");
    continentCodeMap.insert("as", "Asien");
    continentCodeMap.insert("eu/as", "Europa/Asien");
    continentCodeMap.insert("oc", "Australien");

}

void FGame::startFlagsGame()
{
    gameflags = true;
    gameborders = false;
    gameareas = false;
    gamecapitals = false;
    gamecontinent = false;
    gamePoints = 0;
    setFlags(gameflags);

    questionList.clear();

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    //generateQuestion();
}

void FGame::startBordersGame()
{
    gameflags = false;
    gameborders = true;
    gameareas = false;
    gamecapitals = false;
    gamecontinent = false;
    setFlags(gameflags);

    questionList.clear();

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(borderMap.isEmpty())
        borderMap = generateBorderMap();

}

void FGame::startCapitalsGame()
{
    gameflags = false;
    gameborders = false;
    gameareas = false;
    gamecapitals = true;
    gamecontinent = false;
    setFlags(gameflags);


    questionList.clear();

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(capitalMap.isEmpty())
        capitalMap = generateCapitalMap();
}

void FGame::startAreasGame()
{
    gameflags = false;
    gameborders = false;
    gameareas = true;
    gamecapitals = false;
    gamecontinent = false;
    setFlags(gameflags);


    questionList.clear();

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(areaMap.isEmpty())
        areaMap = generateAreaMap();
}

void FGame::startContinentGame()
{
    gameflags = false;
    gameborders = false;
    gameareas = false;
    gamecapitals = false;
    gamecontinent = true;
    setFlags(gameflags);


    questionList.clear();

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(continentMap.isEmpty())
        continentMap = generateContinentMap();
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
    setQuestionCounter(counter);

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

void FGame::setQuestionCounter(int newquestionCounter)
{
    if (m_questionCounter == newquestionCounter)
        return;
    m_questionCounter = newquestionCounter;
    emit questionCounterChanged();
}

bool FGame::flags() const
{
    return m_flags;
}

void FGame::setFlags(bool newFlags)
{
    if (m_flags == newFlags)
        return;
    m_flags = newFlags;
    emit flagsChanged();
}

bool FGame::continentAvailable() const
{
    return m_continentAvailable;
}

void FGame::setContinentAvailable(bool newContinentAvailable)
{
    if (m_continentAvailable == newContinentAvailable)
        return;
    m_continentAvailable = newContinentAvailable;
    emit continentAvailableChanged();
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
        }else{
            countrieCodesMap.remove(key);
        }
    }

//    qDebug() << "Countrie map:" << countrieCodesMap.count();
//    qDebug() << "Flag map:" << map.count();
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

            if(capital.isValid()){
                if(countrieCodesMap.contains(alpha2)){
                    QString t = capital.toString();
                    QString u = upper(t);
                    //qDebug() << u;
                    map.insert(alpha2, u);
                }
            }
    }

    return map;
}

QMap<QString, QString> FGame::generateBorderMap()
{
    QMap<QString, QString>map;

    if(countrieCodesMap.isEmpty())
            countrieCodesMap = generateCountrieCodesMap();


    QMapIterator<QString, QString> it(countrieCodesMap);
    while (it.hasNext()) {
            it.next();

            QString alpha2 = it.key();
            QVariant borders = readData(alpha2, "neighbors");

            if(borders.isValid()){
                if(countrieCodesMap.contains(alpha2)){

                    QString country = countrieCodesMap.value(alpha2);
                    QStringList bl = borders.toStringList();
                    QString bs;
                    foreach (QString c, bl) {
                        bs.append(countrieCodesMap.value(c) );
                        if(bl.last() != c)
                            bs.append(", ");
                    }

                    if(bl.isEmpty())
                        bs = "None";

                    //qDebug() << country << ":" << bs;
                    map.insert(alpha2, bs);
                }
            }
    }

    return map;
}

QMap<QString, QString> FGame::generateContinentMap()
{
    QMap<QString, QString>map;

    if(countrieCodesMap.isEmpty())
        countrieCodesMap = generateCountrieCodesMap();


    QMapIterator<QString, QString> it(countrieCodesMap);
    while (it.hasNext()) {
            it.next();

            QString alpha2 = it.key();
            QVariant cont = readData(alpha2, "continent");

            if(cont.isValid()){
                if(countrieCodesMap.contains(alpha2)){

                    QString key = cont.toString();
                    //QString continent = continentMap.value(key);
                    map.insert(alpha2, key);
                }
            }
    }

    return map;
}

QMap<QString, double> FGame::generateAreaMap()
{
    QMap<QString,double>map;

    if(countrieCodesMap.isEmpty())
            countrieCodesMap = generateCountrieCodesMap();


    QMapIterator<QString, QString> it(countrieCodesMap);
    while (it.hasNext()) {
            it.next();

            QString alpha2 = it.key();
            QVariant area = readData(alpha2, "area");

            QVariantMap vmap = area.toMap();

            if(area.isValid()){
                if(countrieCodesMap.contains(alpha2)){
                    bool ok;
                    double km2 = vmap.value("km2").toDouble(&ok);
                    if(ok){
                        //qDebug() << alpha2 << " : " << vmap.value("km2");
                        map.insert(alpha2, km2);
                    }
                }
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
    setQuestionCounter( counter );

    // Set max value for random nr
    int max = 0;
    if(gameflags)
        max = flagMap.count()-1;

    if(gamecapitals)
        max = capitalMap.count()-1;

    if(gameborders)
        max = borderMap.count()-1;

    if(gameareas)
        max = areaMap.count()-1;

    if(gamecontinent)
        max = continentMap.count()-1;


    int nr = getRandomNumber(max);
    while (qList.contains(nr)) {
        nr = getRandomNumber(max);
    }
    qList << nr; // Questionlist number

    QString key;

    if(gameflags)
        key = countrieCodesMap.keys().at(nr);

    if(gamecapitals)
        key = capitalMap.keys().at(nr);

    if(gameborders)
        key = borderMap.keys().at(nr);

    if(gameareas)
        key = areaMap.keys().at(nr);

    if(gamecontinent)
        key = continentMap.keys().at(nr);


    QString country = countrieCodesMap.value(key);

    if(!flagMap.contains(key)){
        emit errorMessage("Failed to generate question!");
        return;
    }



    qDebug() << "Key: "  << key;
    qDebug() << "Country: "  << country;


    QString flagpath = flagMap.value(key);
    setFlagPath(flagpath);


    // Clear answer list
    aList.clear();

    if(gameflags){
        setSolution(country);
        aList << nr;
    }

    if(gamecapitals){
        QString city = capitalMap.value(key);
        setSolution(city);
        setQuestion("Capital of " + country + "?");
        int nr = capitalMap.values().indexOf(city);
        aList << nr;
    }

    if(gameareas){
        double km2 = areaMap.value(key);
        QString km2s = QString("%1 km2").arg(km2);
        QString country = countrieCodesMap.value(key);
        setSolution(km2s);
        setQuestion("Size of " + country + "?");
        int nr = areaMap.keys().indexOf(key);
        aList << nr;
    }

    if(gameborders){
        QString borders = borderMap.value(key);
        QString country = countrieCodesMap.value(key);
        setSolution(borders);
        setQuestion("Neighbor(s) of " + country + "?");
        int nr = borderMap.values().indexOf(borders);
        aList << nr;
    }

    if(gamecontinent){
        QString continentCode = continentMap.value(key);
        QString continentName = continentCodeMap.value(continentCode);
        //QString country = countrieCodesMap.value(key);
        setSolution(continentName);
        setQuestion("Continent of " + country + "?");
        int nr = continentCodeMap.values().indexOf(continentName);
        aList << nr;
        max = continentCodeMap.count()-1;
//        qDebug() << "Code: " << continentCode;
//        qDebug() << "Name: " << continentName;
    }


    // Generate answers
    // Clear all answers
    setAnswerA("");
    setAnswerB("");
    setAnswerC("");
    setAnswerD("");
    QList<int>pList; // List of position


    for (int i = 0; i < 3; i++) {

        int nr = getRandomNumber(max);
        while (aList.contains(nr)) {
             nr = getRandomNumber(max);
        }
        aList << nr;

        int pos = getRandomNumber(4);
        while (pList.contains(pos)) {
             pos = getRandomNumber(4);
        }
        pList << pos;

        QString k;
        QString a;
        if(gameflags){
             k = countrieCodesMap.keys().at(nr);
             a = countrieCodesMap.value(k);
        }
        if(gamecapitals){
             k = capitalMap.keys().at(nr);
             a = capitalMap.value(k);
        }
        if(gameborders){
             k = borderMap.keys().at(nr);
             a = borderMap.value(k);
        }
        if(gamecontinent){
             k = continentCodeMap.keys().at(nr);
             a = continentCodeMap.value(k);
        }
        if(gameareas){
             k = areaMap.keys().at(nr);
             double km = areaMap.value(k);
             a = QString("%1 km2").arg(km);
        }

        if(pos == 0)
             setAnswerA(a);
        if(pos == 1)
             setAnswerB(a);
        if(pos == 2)
             setAnswerC(a);
        if(pos == 3)
             setAnswerD(a);

        if(m_answerA.isEmpty())
             setAnswerA(solution());
        if(m_answerB.isEmpty())
             setAnswerB(solution());
        if(m_answerC.isEmpty())
             setAnswerC(solution());
        if(m_answerD.isEmpty())
             setAnswerD(solution());
    }

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

    if(gamecapitals){

        QString solutionKey = countrieCodesMap.key(solution());
        answerList << solutionKey;

        for(int i = 0; i < 3; i++){

            QString key = getRandomCountrieCode(capitalMap.size()-1, type);
            QString capital = capitalMap.value(key);

            int pos = getRandomNumber(4);
            while (posList.contains(pos)) {
                pos = getRandomNumber(4);
            }
            posList.insert(pos, capital);

            if(pos == 0)
                setAnswerA(capital);

            if(pos == 1)
                setAnswerB(capital);

            if(pos == 2)
                setAnswerC(capital);

            if(pos == 3)
                setAnswerD(capital);

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

// Rewrite this function
QString FGame::upper(const QString &source)
{
    if(source.isEmpty())
        return "";

    QString s;

    for(int i = 0; i < source.size(); i++){

        QString u;
        bool space;
        bool minus;


        u = source.at(i);

        if(space){
            u = u.toUpper();
            space = false;
        }

        if(minus){
            u = u.toUpper();
            minus = false;
        }


        if(i == 0)
            u = u.toUpper();


        if(u == " "){
            space = true;
        }

        if(u == "-"){
            minus = true;
        }

        s.append(u);

    }

    return s;
}
