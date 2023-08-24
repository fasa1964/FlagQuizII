#include "finitgame.h"

#include <QLocale>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QSslSocket>
#include <QEventLoop>
#include <QNetworkRequest>

FInitGame::FInitGame(QObject *parent)
    : QObject{parent}
{

    setOnline(false);
    setSslSupport(false);
    setSetup(false);
    setKilobytes(0.0);
    setCurrentFile("None");
    setFlagsCount(getFlagsCount());


    QString locale =  QLocale::system().bcp47Name();

    // Countries codes in alpha2
    fileMap.insert("codes.json", QUrl("https://flagcdn.com/"+ locale +"/codes.json"));

    // Countries with different languages in alpha2 -> download single file from github
    fileMap.insert("world.json", QUrl("https://raw.github.com/stefangabos/world_countries/master/data/countries/_combined/world.json"));

    // Countries with capitals and other datas like sqare meter
    fileMap.insert("data.json", QUrl("https://raw.github.com/bhatvikrant/world-countries-capitals/master/data/data.json"));

}

// Calling from qml
void FInitGame::checkGameData()
{

    // Check for file 'codes.json' 'world.json' and 'data.json'
    foreach (QString s, fileMap.keys()) {

        QFile file(s);
        if(!file.exists()){
            downloadQueue.enqueue(fileMap.value(s));
        }
    }


    QFile file("codes.json");
    if(file.exists() && getFlagsCount() == 0){
        if(!generateFlagsForDownloading()){
            emit errorMessage(tr("Sorry, failed to create URL's for flags!"));
            return;
        }
    }

    if(file.exists() &&  getFlagsCount() < getAvailableFlags()){
        emit errorMessage(tr("Some flags are missing!"));
        if(!generateFlagsForDownloading()){
            emit errorMessage(tr("Sorry, failed to create URL's for flags!"));
            return;
        }else{
            if(!downloadQueue.isEmpty()){
                setSetup(true);
            }
        }
    }

    if(!downloadQueue.isEmpty()){
        setSetup(true);
    }

}

void FInitGame::cancelDownloading()
{
    if(!downloadQueue.isEmpty()){
        downloadQueue.clear();
        emit errorMessage(tr("Cancel downloading game data's!"));
    }
}

void FInitGame::startDownloadGameDatas()
{
    checkNetworkState();

    if(!online()){
        emit errorMessage(tr("You are offline. Make sure you are online and try again!"));
        return;
    }

    if(!sslSupport()){
        emit errorMessage(tr("Your device does not support SSL (Secure Socket Layer). I'm Breaking up to download game datas!"));
        return;
    }


    if(!downloadQueue.isEmpty()){
        setSetup(true);
        QUrl url = downloadQueue.dequeue();
        download(url);
    }

}

//!--------------------------------------

// QML properties
bool FInitGame::online() const
{
    return m_online;
}

void FInitGame::setOnline(bool newOnline)
{
    if (m_online == newOnline)
        return;
    m_online = newOnline;
    emit onlineChanged();
}

bool FInitGame::sslSupport() const
{
    return m_sslSupport;
}

void FInitGame::setSslSupport(bool newSslSupport)
{
    if (m_sslSupport == newSslSupport)
        return;
    m_sslSupport = newSslSupport;
    emit sslSupportChanged();
}

QString FInitGame::currentFile() const
{
    return m_currentFile;
}

void FInitGame::setCurrentFile(const QString &newCurrentFile)
{
    if (m_currentFile == newCurrentFile)
        return;
    m_currentFile = newCurrentFile;
    emit currentFileChanged();
}
//!-------------------------------

// Signal from QNetworkReply
void FInitGame::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    quint64 kb = bytesReceived / 1000.0;
    setKilobytes(kb);
}

void FInitGame::downloadFinished()
{
    stream.close();

    if(reply->error() != QNetworkReply::NoError){
        emit errorMessage(reply->errorString());
        return;
    }

    // Check if file saved
    QString fname = stream.fileName();
    QFile file(fname);
    if(file.exists())
        emit appendDownloadedFile(fname);


    if(!downloadQueue.isEmpty()){

        QUrl url = downloadQueue.dequeue();
        download(url);
        setFlagsCount( getFlagsCount() );

    }


    if(fname == "codes.json"){

        if(!generateFlagsForDownloading()){
            emit errorMessage(tr("Sorry, failed to create URL's for flags!"));
            return;
        }
    }

}

void FInitGame::downloadReadyToRead()
{
    stream.write( reply->readAll() );
}
//!-------------------------------------------------



int FInitGame::flagsCount() const
{
    return m_flagsCount;
}

void FInitGame::setFlagsCount(int newFlagsCount)
{
    if (m_flagsCount == newFlagsCount)
        return;
    m_flagsCount = newFlagsCount;
    emit flagsCountChanged();
}

double FInitGame::kilobytes() const
{
    return m_kilobytes;
}

void FInitGame::setKilobytes(double newKilobytes)
{
    if (qFuzzyCompare(m_kilobytes, newKilobytes))
        return;
    m_kilobytes = newKilobytes;
    emit kilobytesChanged();
}

bool FInitGame::setup() const
{
    return m_setup;
}

void FInitGame::setSetup(bool newSetup)
{
    if (m_setup == newSetup)
        return;
    m_setup = newSetup;
    emit setupChanged();
}

bool FInitGame::fileDownloaded() const
{
    return m_fileDownloaded;
}

void FInitGame::setFileDownloaded(bool newFileDownloaded)
{
    if (m_fileDownloaded == newFileDownloaded)
        return;
    m_fileDownloaded = newFileDownloaded;
    emit fileDownloadedChanged();
}
//!---------------------------

void FInitGame::checkNetworkState()
{
    // Check for ssl support
    setSslSupport( QSslSocket::supportsSsl() );

    // Check if online
    bool status = false;
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply* reply = nam.get(req);
    QEventLoop loop;

    QTimer timeoutTimer;

    connect(&timeoutTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    timeoutTimer.setSingleShot(true);
    timeoutTimer.start(3000);

    loop.exec();

    if (reply->bytesAvailable())
        status = true;


    setOnline(status);

}

int FInitGame::getFlagsCount()
{
    QDir dir;
    QStringList list = dir.entryList( QStringList() << "*.svg", QDir::Files);
    return list.count();
}

int FInitGame::getAvailableFlags()
{
    int count = 0;

    QFile file("codes.json");

    if (!file.open(QIODevice::ReadOnly)) {
        QString f = file.fileName();
        QString e = file.errorString();
        QString text = QString("Could not open %1 for reading: %2").arg(f, f.size() ).arg(e, e.size());
        emit errorMessage(text);
        return count;
    }

    QTextStream file_text(&file);
    QString jsonString = file_text.readAll();

    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

    if(jsonDoc.isNull())
        return count;

    if(jsonDoc.isArray())
        return count;

    if(jsonDoc.isObject()){
        QJsonObject jsonObject = jsonDoc.object();
        if(!jsonObject.isEmpty())
            count = jsonObject.keys().size();
    }


    return count;
}

void FInitGame::download(const QUrl &url)
{
    // Get filename wich is downloading
    QString filename = getDownloadBasename(url);
    stream.setFileName(filename);
    setCurrentFile(filename);

    if(!stream.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit errorMessage(stream.errorString());
        return;
    }

    QNetworkRequest request(url);
    reply = manager.get( request );

    connect(reply, &QNetworkReply::downloadProgress, this, &FInitGame::downloadProgress );
    connect(reply, &QNetworkReply::finished, this, &FInitGame::downloadFinished );
    connect(reply, &QNetworkReply::readyRead, this, &FInitGame::downloadReadyToRead );

    if(downloadQueue.isEmpty()){
        setSetup(false);
        emit errorMessage(tr("Setup has been finished!"));
    }else {
        setSetup(true);
    }

}

QString FInitGame::getDownloadBasename(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "Unknown";

    return basename;
}

bool FInitGame::saveDownloadFile(const QString &filen, QIODevice *data)
{
    QFile file(filen);

    if (!file.open(QIODevice::WriteOnly)) {
        QString f = filen;
        QString e = file.errorString();
        QString text = QString("Could not open %1 for writing: %2").arg(f, f.size() ).arg(e, e.size());
        emit errorMessage(text);
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

bool FInitGame::generateFlagsForDownloading()
{
    QFile file("codes.json");

    if (!file.open(QIODevice::ReadOnly)) {
        QString f = file.fileName();
        QString e = file.errorString();
        QString text = QString("Could not open %1 for reading: %2").arg(f, f.size() ).arg(e, e.size());
        emit errorMessage(text);
        return false;
    }

    QTextStream file_text(&file);
    QString jsonString = file_text.readAll();

    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

    if(jsonDoc.isNull())
        return false;

    if(jsonDoc.isArray())
        return false;

    if(jsonDoc.isObject())
    {

        QJsonObject jsonObject = jsonDoc.object();

        if(!jsonObject.isEmpty())
        {

            foreach (QString key, jsonObject.keys()) {

                QString filename = QString("%1.svg").arg(key);
                QFile file(filename);

                if(!file.exists()){
                    QString urlString = QString("https://flagcdn.com/%1.svg").arg(key);
                    QUrl url = QUrl(urlString);
                    downloadQueue.enqueue(url);
                }
            }


        }else{
            return false;
        }

    }else{
        return false;
    }

    return true;
}
