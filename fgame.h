#ifndef FGAME_H
#define FGAME_H

#include <QObject>
#include <QQmlEngine>

#include <QMap>
#include <QUrl>

class FGame : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool flagsAvailable READ flagsAvailable WRITE setFlagsAvailable NOTIFY flagsAvailableChanged)
    Q_PROPERTY(bool bordersAvailable READ bordersAvailable WRITE setBordersAvailable NOTIFY bordersAvailableChanged)
    Q_PROPERTY(bool areasAvailable READ areasAvailable WRITE setAreasAvailable NOTIFY areasAvailableChanged)
    Q_PROPERTY(bool capitalsAvailable READ capitalsAvailable WRITE setCapitalsAvailable NOTIFY capitalsAvailableChanged)
    Q_PROPERTY(bool continentAvailable READ continentAvailable WRITE setContinentAvailable NOTIFY continentAvailableChanged)

    Q_PROPERTY(QVariant question READ question WRITE setQuestion NOTIFY questionChanged)
    Q_PROPERTY(QString answerA READ answerA WRITE setAnswerA NOTIFY answerAChanged)
    Q_PROPERTY(QString answerB READ answerB WRITE setAnswerB NOTIFY answerBChanged)
    Q_PROPERTY(QString answerC READ answerC WRITE setAnswerC NOTIFY answerCChanged)
    Q_PROPERTY(QString answerD READ answerD WRITE setAnswerD NOTIFY answerDChanged)
    Q_PROPERTY(QString solution READ solution WRITE setSolution NOTIFY solutionChanged)
    Q_PROPERTY(QString flagPath READ flagPath WRITE setFlagPath NOTIFY flagPathChanged)
    Q_PROPERTY(int questionCounter READ questionCounter WRITE setQuestionCounter NOTIFY questionCounterChanged)
    Q_PROPERTY(bool flags READ flags WRITE setFlags NOTIFY flagsChanged)


public:
    explicit FGame(QObject *parent = nullptr);

    Q_INVOKABLE void startFlagsGame();
    Q_INVOKABLE void startBordersGame();
    Q_INVOKABLE void startCapitalsGame();
    Q_INVOKABLE void startAreasGame();
    Q_INVOKABLE void startContinentGame();
    Q_INVOKABLE void startNextQuestion();
    Q_INVOKABLE void setLanguageCode(const QString &code);
    Q_INVOKABLE void cancelGame();

    Q_INVOKABLE QString getDescriptionText();

    // Jokers
    Q_INVOKABLE void setJoker50();
    Q_INVOKABLE void setJokerPub();
    Q_INVOKABLE void setJokerTel();

    Q_INVOKABLE void setAnswer(const QString &answer);


    bool flagsAvailable() const;
    void setFlagsAvailable(bool newFlagsAvailable);

    bool bordersAvailable() const;
    void setBordersAvailable(bool newBordersAvailable);

    bool areasAvailable() const;
    void setAreasAvailable(bool newAreasAvailable);

    bool capitalsAvailable() const;
    void setCapitalsAvailable(bool newCapitalsAvailable);

    QVariant question() const;
    void setQuestion(const QVariant &newQuestion);

    QString flagPath() const;
    void setFlagPath(const QString &newFlagPath);

    QString solution() const;
    void setSolution(const QString &newSolution);

    QString answerA() const;
    void setAnswerA(const QString &newAnswerA);

    QString answerB() const;
    void setAnswerB(const QString &newAnswerB);

    QString answerC() const;
    void setAnswerC(const QString &newAnswerC);

    QString answerD() const;
    void setAnswerD(const QString &newAnswerD);

    int questionCounter() const;
    void setQuestionCounter(int newquestionCounter);


    bool flags() const;
    void setFlags(bool newFlags);

    bool continentAvailable() const;
    void setContinentAvailable(bool newContinentAvailable);

signals:

    // QML Properties
    void flagsAvailableChanged();
    void bordersAvailableChanged();
    void areasAvailableChanged();
    void capitalsAvailableChanged();
    void continentAvailableChanged();
    void flagsChanged();

    void questionChanged();
    void flagPathChanged();
    void solutionChanged();
    void answerAChanged();
    void answerBChanged();
    void answerCChanged();
    void answerDChanged();
    void questionCounterChanged();


    void selectButton(const QString &button);
    void errorMessage(const QString &errortext);
    void credits(const QString &euro);
    void gameOver(const QString &euro);


private:

    // QML Properties
    bool m_flagsAvailable;
    bool m_bordersAvailable;
    bool m_areasAvailable;
    bool m_capitalsAvailable;
    bool m_continentAvailable;
    bool m_flags;

    // Type of game
    bool gameflags;
    bool gameborders;
    bool gameareas;
    bool gamecapitals;
    bool gamecontinent;

    // Question and Answers
    QVariant m_question;
    QString m_flagPath;
    QString m_solution;
    QString m_answerA;
    QString m_answerB;
    QString m_answerC;
    QString m_answerD;
    int m_questionCounter;
    int maxQuestion;
    int counter;

    // Points
    QMap<int, QString> pointMap;
    int gamePoints;


    int getFlagsCount();

    QMap<QString, QString> countrieCodesMap;
    QMap<QString, QString> generateCountrieCodesMap();

    QMap<QString, QString> flagMap;
    QMap<QString, QString> generateFlagMap();

    QMap<QString, QString> capitalMap;
    QMap<QString, QString> generateCapitalMap();

    QMap<QString, QString> borderMap;
    QMap<QString, QString> generateBorderMap();

    QMap<QString, QString> continentCodeMap;
    QMap<QString, QString> continentMap;
    QMap<QString, QString> generateContinentMap();

    QMap<QString, QString> km2Map;
    QMap<QString, double> areaMap;
    QMap<QString, double> generateAreaMap();

    // Get random question
    void generateQuestion();
    void generateAnswers(const QString &key);
    //QStringList questionList;
    QList<int> qList;
    QList<int> aList;
    QList<QString> questionKeyList;
    QList<QString> answerKeyList;
    QString getAnswer(const QString &key);

    bool isIsland(const QString &key);

    //QStringList answerList;
    void setupVariables();

    //QString getRandomCountrieCode(int max, QString &type);
    int getRandomNumber(int max);


    // Get data from
    QVariant readData(const QString &alpha2, const QString &key);

    int indexOfKeys(const QString &key, const QMap<QString, QString> &map);
    int indexOfValues(const QString &value, const QMap<QString, QString> &map);
    QString convertToString(double value);
    QString getKeyFrom(const QMap<QString,QString> &map, int nr);
    QString getKeyFrom(const QMap<QString,double> &map, int nr);
    QString getRandomKey();

    QString upper(const QString &source);

    QString languageCode;
    QString translate(const QString &langCode, const QString &alpha2);
    QString translateString(const QString &langCode, const QString &source);


};

#endif // FGAME_H
