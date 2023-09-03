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
#include <QDoubleValidator>

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

    // Set all varable to 0
    setupVariables();


    setQuestion("This is my question");
    setAnswerA("Antwort A");
    setAnswerB("Antwort B");
    setAnswerC("Antwort C");
    setAnswerD("Antwort D");
    setSolution("Antwort C");
    setFlagPath("");

    maxQuestion = 15;

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
    continentCodeMap.insert("af", "Africa");
    continentCodeMap.insert("sa", "South America");
    continentCodeMap.insert("na", "North America");
    continentCodeMap.insert("eu", "Europe");
    continentCodeMap.insert("as", "Asia");
    continentCodeMap.insert("eu/as", "Europe/Asia");
    continentCodeMap.insert("oc", "Australia/Oceania");

    // Get local for langiage code
    QLocale locale;
    QString lcode = locale.bcp47Name();
    languageCode = lcode;

}

void FGame::startFlagsGame()
{
    setupVariables();

    gameflags = true;
    setFlags(gameflags);

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

}

void FGame::startBordersGame()
{
    setupVariables();

    gameborders = true;
    setFlags(gameflags);

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(borderMap.isEmpty())
        borderMap = generateBorderMap();

}

void FGame::startCapitalsGame()
{
    setupVariables();
    gamecapitals = true;
    setFlags(gameflags);

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(capitalMap.isEmpty())
        capitalMap = generateCapitalMap();
}

void FGame::startAreasGame()
{

    setupVariables();

    gameareas = true;
    setFlags(gameflags);

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(areaMap.isEmpty())
        areaMap = generateAreaMap();
}

void FGame::startContinentGame()
{

    setupVariables();

    gamecontinent = true;
    setFlags(gameflags);

    if(flagMap.isEmpty())
        flagMap = generateFlagMap();

    if(continentMap.isEmpty())
        continentMap = generateContinentMap();
}

void FGame::startNextQuestion()
{
    generateQuestion();
}

void FGame::setLanguageCode(const QString &code)
{
    languageCode = code;
    //qDebug() << "Languagecode: " << code;
}

void FGame::cancelGame()
{
    setupVariables();
    emit gameOver("");
}

QString FGame::getDescriptionText()
{
    QString text;

    QFile textfile("://txt/description.txt");
    if(textfile.open(QIODevice::ReadOnly)){
        QTextStream in(&textfile);
        text = in.readAll();
    }else{
        emit errorMessage( textfile.errorString() );
    }



    return text;
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
                    QDoubleValidator val;
                    val.setLocale(QLocale::C);
                    val.setNotation(QDoubleValidator::StandardNotation);
                    val.setDecimals(2);
                    qreal km2 = vmap.value("km2").toDouble(&ok);
                    QString km = QString::number(km2);
                    val.fixup(km);
                    km2Map.insert(alpha2, km);
                    if(ok){
                        //qDebug() << alpha2 << " : " << km;
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

    QString key = getRandomKey();
    while (questionKeyList.contains(key)) {
            key = getRandomKey();
    }
    questionKeyList << key;


    QString country = countrieCodesMap.value(key);

    if(!flagMap.contains(key)){
        emit errorMessage("Failed to generate question!");
        return;
    }

    QString flagpath = flagMap.value(key);
    setFlagPath(flagpath);

    QLocale local;

    if(gameflags){

        QString alpha2 = countrieCodesMap.key(country);

        // Translate country
        if(local.bcp47Name() != languageCode)
            country = translate(languageCode, alpha2);

        // if no translate available get locale
        if(country.isEmpty())
            country = countrieCodesMap.value(key);

        setSolution(country);
    }

    if(gamecapitals){
        QString city = capitalMap.value(key);
        setSolution(city);
        setQuestion("Capital of " + country + "?");
    }

    if(gameareas){
        double km2 = areaMap.value(key);
        QString km2s = convertToString(km2);
        setSolution(km2s);
        setQuestion("Size of " + country + "?");
    }

    if(gameborders){
        QString borders = borderMap.value(key);
        if(local.bcp47Name() != languageCode && borders != "None"){
            borders = translateString(languageCode, borders);
            country = translateString(languageCode, country);
        }

        setSolution(borders);
        setQuestion("Neighbor(s) of " + country + "?");
    }

    if(gamecontinent){
        QString continentCode = continentMap.value(key);
        QString continentName = continentCodeMap.value(continentCode);
        setSolution(continentName);
        setQuestion("Continent of " + country + "?");
        key = continentCode;
    }


    generateAnswers(key);
}


// ToDo -> try to get not double name of "None" -> seems to work
// ToDo -> try to get not double continent name -> seems ok now
void FGame::generateAnswers(const QString &key)
{
    // Generate answers
    // Clear all answers
    setAnswerA("");
    setAnswerB("");
    setAnswerC("");
    setAnswerD("");
    QList<int>pList; // List of position
    QLocale local;

    QStringList bordersAnsers;

    if(gameborders)
        bordersAnsers << borderMap.value(key);

    answerKeyList.clear();

    // When gamecontinent key = continent code
    answerKeyList.append(key); // Solution of question


    for (int i = 0; i < 3; ++i) {

        QString a;                  // Answer
        QString k = getRandomKey(); // Key of countrycode in alpha2

        if(gamecontinent){

            QString cc = continentMap.value(k);
            k = cc;
        }

        if(gameborders){
            while (bordersAnsers.contains( borderMap.value(k) )) {
                    k = getRandomKey();
            }
        }

        // Get random country code
        if(!gameborders){
            while (answerKeyList.contains(k)) {
                k = getRandomKey();

                if(gamecontinent){

                    QString cc = continentMap.value(k);
                    k = cc;
                }
            }
        }

        if(gameborders)
            bordersAnsers << borderMap.value(k);

        answerKeyList << k;

        // Position the possible answers
        int pos = getRandomNumber(4);
        while (pList.contains(pos)) {
            pos = getRandomNumber(4);
        }
        pList << pos;

        if(gameflags){

            // Translate country
            if(local.bcp47Name() != languageCode)
                a = translate(languageCode, k);

            // if no translate available get locale name of country
            if(a.isEmpty())
                a = countrieCodesMap.value(k);
        }

        if(gamecapitals)
            a = capitalMap.value(k);

        // ToDo -> try to get not double name of "None"
        if(gameborders){
            a = borderMap.value(k);

            if(local.bcp47Name() != languageCode && a != "None")
                a = translateString(languageCode, a);

        }




        // ToDo -> get double continent name
        if(gamecontinent){
            // Get continent name
            QString cn = continentCodeMap.value(k);
            a = cn;
        }

        if(gameareas){
            double km = areaMap.value(k);
            a = convertToString(km);
        }


        if(pos == 0)
            setAnswerA(a);
        if(pos == 1)
            setAnswerB(a);
        if(pos == 2)
            setAnswerC(a);
        if(pos == 3)
            setAnswerD(a);

        // Position the solution into an empty place
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

//void FGame::generateQuestion()
//{
//    if(counter >= maxQuestion){

//        emit gameOver( pointMap.value(gamePoints));
//        return;
//    }

//    counter++;
//    setQuestionCounter( counter );

//    // Set max value for random nr
//    int max = 0;
//    if(gameflags)
//        max = flagMap.count()-1;

//    if(gamecapitals)
//        max = capitalMap.count()-1;

//    if(gameborders)
//        max = borderMap.count()-1;

//    if(gameareas)
//        max = areaMap.count()-1;

//    if(gamecontinent)
//        max = continentMap.count()-1;


//    int nr = getRandomNumber(max);
//    while (qList.contains(nr)) {
//        nr = getRandomNumber(max);
//    }
//    qList << nr; // Questionlist number

//    // * Test
//    QString key = getRandomKey();
//    while (questionKeyList.contains(key)) {
//        key = getRandomKey();
//    }
//    questionKeyList << key;
//    //qDebug() << "Key: " << key;
//    // ******

//    if(gameflags)
//        key = getKeyFrom(countrieCodesMap, nr); //  key = countrieCodesMap.keys().at(nr);

//    if(gamecapitals)
//        key = getKeyFrom(capitalMap, nr); //key = capitalMap.keys().at(nr);

//    if(gameborders)
//        key = getKeyFrom(borderMap, nr); // key = borderMap.keys().at(nr);

//    if(gameareas)
//        key = getKeyFrom(areaMap, nr); //key = areaMap.keys().at(nr);

//    if(gamecontinent)
//        key = getKeyFrom(continentMap, nr); //key = continentMap.keys().at(nr);



//    QString country = countrieCodesMap.value(key);

//    if(!flagMap.contains(key)){
//        emit errorMessage("Failed to generate question!");
//        return;
//    }


//    QString flagpath = flagMap.value(key);
//    setFlagPath(flagpath);


//    // Clear answer list
//    aList.clear();
//    answerKeyList.clear();

//    if(gameflags){

//        // Translate country
//        QString alpha2 = countrieCodesMap.key(country);
//        country = translate(languageCode, alpha2);

//        if(country.isEmpty()) // if no translate available get locale
//            country = countrieCodesMap.value(key);

//        setSolution(country);
//        aList << nr;
//    }

//    if(gamecapitals){
//        QString city = capitalMap.value(key);
//        setSolution(city);
//        setQuestion("Capital of " + country + "?");
//        int nr = indexOfValues(city, capitalMap);
//        aList << nr;
//    }

//    if(gameareas){
//        double km2 = areaMap.value(key);
//        QString km2s = convertToString(km2);

//        QString country = countrieCodesMap.value(key);
//        setSolution(km2s);
//        setQuestion("Size of " + country + "?");
//        QList<QString> kList = areaMap.keys();
//        int nr = kList.indexOf(key);
//        aList << nr;
//    }

//    if(gameborders){
//        QString borders = borderMap.value(key);
//        QString country = countrieCodesMap.value(key);
//        setSolution(borders);
//        setQuestion("Neighbor(s) of " + country + "?");
//        int nr = indexOfValues(borders, borderMap);
//        aList << nr;

//        // try to translate -> it's working
////        QLocale local;
////        if(local.bcp47Name() != languageCode && borders != "None"){

////            qDebug() << "Code:" << languageCode;
////            QStringList borderList = borders.split(", ");
////            QString tranBorders;
////            //qDebug() << borderList;
////            foreach (QString c, borderList) {

////                    QString key = countrieCodesMap.key(c);
////                    QString tc = translate(languageCode, key);
////                    //qDebug() << key << ":" << tc;
////                    tranBorders.append(tc);
////                    tranBorders.append(", ");

////            }

////            tranBorders.removeLast();
////            tranBorders.removeLast();
////            setSolution(tranBorders);
////        }
//    }

//    if(gamecontinent){
//        QString continentCode = continentMap.value(key);
//        QString continentName = continentCodeMap.value(continentCode);
//        setSolution(continentName);
//        setQuestion("Continent of " + country + "?");
//        int nr = indexOfValues(continentName, continentCodeMap);
//        aList << nr;
//        max = continentCodeMap.count()-1;
////        qDebug() << "Code: " << continentCode;
////        qDebug() << "Name: " << continentName;
//    }


//    // Generate answers
//    // Clear all answers
//    setAnswerA("");
//    setAnswerB("");
//    setAnswerC("");
//    setAnswerD("");
//    QList<int>pList; // List of position


//    for (int i = 0; i < 3; i++) {

//        QString k; // Key
//        QString a; // Answer

//        int nr = getRandomNumber(max);
//        while (aList.contains(nr)  ) {
//             nr = getRandomNumber(max);
//        }
//        aList << nr;

//        // Position the possible answers
//        int pos = getRandomNumber(4);
//        while (pList.contains(pos)) {
//             pos = getRandomNumber(4);
//        }
//        pList << pos;


//        if(gameflags){

//             // Test
//             int c = 0;
//             for (auto i = countrieCodesMap.begin(), end = countrieCodesMap.end(); i != end; ++i) {

//                    if(c == nr){
//                        k = i.key();
//                        break;
//                    }
//                    c++;
//             }



//             //k = countrieCodesMap.keys().at(nr);
//             a = countrieCodesMap.value(k);

//             // Translate country
//             QString lc = languageCode;
//             QString alpha2 = countrieCodesMap.key(a);
//             a = translate(languageCode, alpha2);

//        }
//        if(gamecapitals){
//             k = capitalMap.keys().at(nr);
//             a = capitalMap.value(k);
//        }
//        if(gameborders){
//             k = borderMap.keys().at(nr);
//             a = borderMap.value(k);
//             qDebug() << "A:" << a;

//        }
//        if(gamecontinent){
//             k = continentCodeMap.keys().at(nr);
//             a = continentCodeMap.value(k);
//        }
//        if(gameareas){
//             k = areaMap.keys().at(nr);
//             double km = areaMap.value(k);
//             a = convertToString(km);
//        }

//        if(pos == 0)
//             setAnswerA(a);
//        if(pos == 1)
//             setAnswerB(a);
//        if(pos == 2)
//             setAnswerC(a);
//        if(pos == 3)
//             setAnswerD(a);

//        if(m_answerA.isEmpty())
//             setAnswerA(solution());
//        if(m_answerB.isEmpty())
//             setAnswerB(solution());
//        if(m_answerC.isEmpty())
//             setAnswerC(solution());
//        if(m_answerD.isEmpty())
//             setAnswerD(solution());
//    }

//}

QString FGame::getAnswer(const QString &key)
{
    QString a;

    if(gameflags)
        a = countrieCodesMap.value(key);

    if(gamecapitals)
        a = capitalMap.value(key);

    if(gameborders)
        a = borderMap.value(key);

    if(gamecontinent){
        a = continentCodeMap.value(key);

    }

    if(gameareas){
        double val = areaMap.value(key);
        a = convertToString(val);
    }

    return a;
}

bool FGame::isIsland(const QString &key)
{
    return readData(key, "neighbors").toStringList().isEmpty();
}

void FGame::setupVariables()
{
    // Type of game
    gameflags = false;
    gameborders = false;
    gameareas = false;
    gamecapitals = false;
    gamecontinent = false;
    setFlags(false);

    counter = 0;
    setQuestionCounter(counter);

    gamePoints = 0;

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

int FGame::indexOfValues(const QString &value, const QMap<QString, QString> &map)
{
    int index = 0;

    QMapIterator<QString, QString> it(map);
    while (it.hasNext()) {
        it.next();
        if(value == it.value())
            break;
        index++;
    }

    return index;
}

QString FGame::convertToString(double value)
{
    QLocale local;
    QString vs;

    if(value <= 9999)
        vs = local.toString( value, 'g', 6 ) + " km2";

    if(value <= 1000000 && value > 9999)
        vs = local.toString( value, 'g', 6 ) + ",00" + " km2";

    if(value > 1000000 && value < 10000000)
        vs = local.toString( value, 'g', 8 ) + " km2";

    if(value >= 10000000 )
        vs = local.toString( value, 'g', 10 ) + " km2";

    return vs;
}

QString FGame::getKeyFrom(const QMap<QString, QString> &map, int nr)
{
    QString k;
    int c = 0;
    for (auto i = map.cbegin(), end = map.cend(); i != end; ++i){

        if(c == nr){
            k = i.key();
            break;
        }

        c++;
    }

    return k;
}

QString FGame::getKeyFrom(const QMap<QString, double> &map, int nr)
{
    QString k;
    int c = 0;
    for (auto i = map.cbegin(), end = map.cend(); i != end; ++i){

        if(c == nr){
            k = i.key();
            break;
        }

        c++;
    }

    return k;
}

// Returns the country code alpha2
QString FGame::getRandomKey()
{
    int max;
    QString key;

    // Set max value for random nr
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


    if(gameflags)
        key = getKeyFrom(countrieCodesMap, getRandomNumber(max));

    if(gamecapitals)
        key = getKeyFrom(capitalMap, getRandomNumber(max));

    if(gameborders)
        key = getKeyFrom(borderMap, getRandomNumber(max));

    if(gameareas)
        key = getKeyFrom(areaMap, getRandomNumber(max));

    if(gamecontinent)
        key = getKeyFrom(continentMap, getRandomNumber(max));

    return key;
}

int FGame::indexOfKeys(const QString &key, const QMap<QString, QString> &map)
{
    int index = 0;

    QMapIterator<QString, QString> it(map);
    while (it.hasNext()) {
        it.next();
        if(key == it.key())
            break;
        index++;
    }

    return index;
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

QString FGame::translate(const QString &langCode, const QString &alpha2)
{
    QString s;
    QFile file("world.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QString f = file.fileName();
        QString e = file.errorString();
        QString text = QString("Could not open %1 for reading: %2").arg(f, f.size() ).arg(e, e.size());
        emit errorMessage(text);
        return s;
    }

    QTextStream file_text(&file);
    QString jsonString = file_text.readAll();

    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

    if(jsonDoc.isNull())
        return s;

    if(jsonDoc.isArray()){

        foreach (QJsonValue val, jsonDoc.array()) {

            if(alpha2 == val.toObject().value("alpha2").toString()){
                s = val.toObject().value(langCode).toString();
                break;
            }


        }

    }

    return s;
}

QString FGame::translateString(const QString &langCode, const QString &source)
{
    QStringList sourceList = source.split(", ");
    QString s;
    //qDebug() << sourceList;
    foreach (QString c, sourceList) {

        QString key = countrieCodesMap.key(c);
        QString tc = translate(langCode, key);

        if(tc.isEmpty())
            tc = c; //qDebug() << "Empty:" << key << ":" << c;

        if(key.isEmpty())
            tc = c;

        s.append(tc);

        if(c != sourceList.last())
            s.append(", ");

    }

    return s;
}
