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


    foreach (QString s, fileMap.keys()) {

        QFile file(s);
        if(!file.exists()){
            downloadQueue.enqueue(fileMap.value(s));
        }
    }
}


void FInitGame::cancelDownloading()
{
    emit errorMessage(tr("Cancel downloading!"));
}



//void FInitGame::startDownload(const QString &filename)
//{
//    checkNetworkState();

//    if(!online()){
//        emit errorMessage(tr("You are offline."));
//        return;
//    }

//    if(!sslSupport()){
//        emit errorMessage(tr("Your device does not support ssl!"));
//        return;
//    }

//    QUrl url = fileMap.value(filename);
//    download(url);

//}

void FInitGame::startDownloadGameDatas()
{
    checkNetworkState();

    if(!online()){
        emit errorMessage(tr("You are offline. Make sure you are online and try again!"));
        return;
    }

    if(!sslSupport()){
        emit errorMessage(tr("Your device does not support SSL (Secure Socket Layer). Breaking up to download game datas!"));
        return;
    }

    setSetup(true);

    if(!downloadQueue.isEmpty()){
        QUrl url = downloadQueue.dequeue();
        download(url);
    }

}

//void FInitGame::startDownloadAllFlags()
//{
//    qDebug() << "Download all flags!";
//}
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

    }


    if(fname == "codes.json"){

        if(!generateFlagsForDownloading()){
            emit errorMessage(tr("Sorry, failed to create URL's for flags!"));
            return;
        }
    }

//    if(file.exists()){
//        setFileDownloaded(true);
//        emit appendDownloadedFile(currentFile());

//        if(m_currentFile == "codes.json"){

//            if(!generateFlagsForDownloading())
//                emit errorMessage(tr("Failed to create flags url's for downloading!"));
//        }


//    }else
//        setFileDownloaded(false);

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

                //QString value = jsonObject.value(key).toString();
                QString urlString = QString("https://flagcdn.com/%1.svg").arg(key);
                QUrl url = QUrl(urlString);
                downloadQueue.enqueue(url);

//                qDebug() << "Value: " << value;
//                qDebug() << "Key: " << key;

            }


        }else{
            return false;
        }

    }else{
        return false;
    }

    return true;
}
