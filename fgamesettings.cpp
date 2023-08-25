#include "fgamesettings.h"

#include <QLocale>
#include <QFile>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

FGameSettings::FGameSettings(QObject *parent)
    : QObject{parent}
{
    setPlayAudio(true);

    QFile file("world.json");
    if(file.exists())
        setEnableLanguage(true);
    else
        setEnableLanguage(false);


    // Get System languages
    QList<QLocale> allLocales = QLocale::matchingLocales(
        QLocale::AnyLanguage,
        QLocale::AnyScript,
        QLocale::AnyCountry);
    //QSet<QString> iso639LanguageCodes;
    //QSet<QString> iso639Codes;



    for(const QLocale &locale : allLocales) {
        iso639Codes.insert(  locale.name().split('_').first());
        //iso639LanguageCodes.insert( tr("%1 (%2)").arg(QLocale::languageToString(locale.language()), locale.name().split('_').first()) );
        QString c = locale.name().split('_').first();
        QString cl = tr("%1 (%2)").arg(QLocale::languageToString(locale.language()), locale.name().split('_').first());
        languageCodeMap.insert(c, cl );
    }


    QLocale local;
    //setLanguageCode( local.bcp47Name() );

    QString lang = languageCodeMap.value(local.bcp47Name() );
    setLanguageName(lang);

    setCurrentIndex(4);

}

QStringList FGameSettings::getAvailableLanguages()
{
    QStringList list;

    QString filename = "world.json";
    QFile file(filename);
    if(!file.exists())
        return list;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString errorMessage = "Open file:" + filename + " failed.\n" + file.errorString();
        emit errorOccurred(errorMessage);
        return list;
    }

    QTextStream file_text(&file);
    QString jsonString = file_text.readAll();

    file.close();


    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

    if(jsonDoc.isNull())
        return list;

    if(jsonDoc.isArray()){

        QJsonValue val = jsonDoc.array().first();
        if(val.isObject()){

            QJsonObject obj = val.toObject();

            foreach (QString k, obj.keys()) {

                if(iso639Codes.contains(k))
                    list << languageCodeMap.value(k);

            }
        }
    }

    return list;
}

void FGameSettings::setLanguage(const QString language)
{
    setLanguageName(language);

    QString code = languageCodeMap.key(language);
    setLanguageCode(code);

    int index = languageCodeMap.values().indexOf(language);
    setCurrentIndex(index);
}

bool FGameSettings::playAudio() const
{
    return m_playAudio;
}

void FGameSettings::setPlayAudio(bool newPlayAudio)
{
    if (m_playAudio == newPlayAudio)
        return;
    m_playAudio = newPlayAudio;
    emit playAudioChanged();
}

bool FGameSettings::enableLanguage() const
{
    return m_enableLanguage;
}

void FGameSettings::setEnableLanguage(bool newEnableLanguage)
{
    if (m_enableLanguage == newEnableLanguage)
        return;
    m_enableLanguage = newEnableLanguage;
    emit enableLanguageChanged();
}

QString FGameSettings::languageCode() const
{
    return m_languageCode;
}

void FGameSettings::setLanguageCode(const QString &newLanguageCode)
{
    if (m_languageCode == newLanguageCode)
        return;
    m_languageCode = newLanguageCode;
    emit languageCodeChanged();
}

QString FGameSettings::languageName() const
{
    return m_languageName;
}

void FGameSettings::setLanguageName(const QString &newLanguageName)
{
    if (m_languageName == newLanguageName)
        return;
    m_languageName = newLanguageName;
    emit languageNameChanged();
}

int FGameSettings::currentIndex() const
{
    return m_currentIndex;
}

void FGameSettings::setCurrentIndex(int newCurrentIndex)
{
    if (m_currentIndex == newCurrentIndex)
        return;
    m_currentIndex = newCurrentIndex;
    emit currentIndexChanged();
}
