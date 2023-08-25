#ifndef FGAMESETTINGS_H
#define FGAMESETTINGS_H

#include <QObject>
#include <QQmlEngine>

class FGameSettings : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool playAudio READ playAudio WRITE setPlayAudio NOTIFY playAudioChanged)
    Q_PROPERTY(bool enableLanguage READ enableLanguage WRITE setEnableLanguage NOTIFY enableLanguageChanged)
    Q_PROPERTY(QString languageCode READ languageCode WRITE setLanguageCode NOTIFY languageCodeChanged)
    Q_PROPERTY(QString languageName READ languageName WRITE setLanguageName NOTIFY languageNameChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit FGameSettings(QObject *parent = nullptr);
    Q_INVOKABLE QStringList getAvailableLanguages();
    Q_INVOKABLE void setLanguage(const QString language);

    bool playAudio() const;
    void setPlayAudio(bool newPlayAudio);

    bool enableLanguage() const;
    void setEnableLanguage(bool newEnableLanguage);

    QString languageCode() const;
    void setLanguageCode(const QString &newLanguageCode);

    QString languageName() const;
    void setLanguageName(const QString &newLanguageName);

    int currentIndex() const;
    void setCurrentIndex(int newCurrentIndex);

signals:

    void errorOccurred(const QString &errortext);

    void playAudioChanged();
    void enableLanguageChanged();
    void languageCodeChanged();
    void languageNameChanged();
    void currentIndexChanged();

private:

    bool m_playAudio;
    bool m_enableLanguage;
    QString m_languageCode;
    QString m_languageName;
    int m_currentIndex;

    QMap<QString, QString>languageCodeMap;
    QSet<QString> iso639Codes;

};

#endif // FGAMESETTINGS_H
