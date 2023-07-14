#ifndef FINITGAME_H
#define FINITGAME_H

#include <QObject>
#include <QQmlEngine>

#include <QMap>
#include <QUrl>
#include <QQueue>
#include <QFile>

#include <QNetworkAccessManager>
#include <QNetworkReply>


class FInitGame : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)
    Q_PROPERTY(bool sslSupport READ sslSupport WRITE setSslSupport NOTIFY sslSupportChanged)
    Q_PROPERTY(bool setup READ setup WRITE setSetup NOTIFY setupChanged)
    Q_PROPERTY(double kilobytes READ kilobytes WRITE setKilobytes NOTIFY kilobytesChanged)
    Q_PROPERTY(int flagsCount READ flagsCount WRITE setFlagsCount NOTIFY flagsCountChanged)

    Q_PROPERTY(bool fileDownloaded READ fileDownloaded WRITE setFileDownloaded NOTIFY fileDownloadedChanged)
    Q_PROPERTY(QString currentFile READ currentFile WRITE setCurrentFile NOTIFY currentFileChanged)

public:
    explicit FInitGame(QObject *parent = nullptr);
    Q_INVOKABLE void checkGameData();
    Q_INVOKABLE void cancelDownloading();

    //Q_INVOKABLE void startDownload(const QString &filename);
    Q_INVOKABLE void startDownloadGameDatas();
    //Q_INVOKABLE void startDownloadAllFlags();


    bool online() const;
    void setOnline(bool newOnline);

    bool sslSupport() const;
    void setSslSupport(bool newSslSupport);

    QString currentFile() const;
    void setCurrentFile(const QString &newCurrentFile);

    bool fileDownloaded() const;
    void setFileDownloaded(bool newFileDownloaded);

    bool setup() const;
    void setSetup(bool newSetup);

    double kilobytes() const;
    void setKilobytes(double newKilobytes);

    int flagsCount() const;
    void setFlagsCount(int newFlagsCount);

signals:

    void appendDownloadedFile(const QString &filename);
    void errorMessage(const QString &messagetext);

    // QML Properties
    void onlineChanged();
    void sslSupportChanged();
    void setupChanged();
    void kilobytesChanged();
    void flagsCountChanged();

    void currentFileChanged();
    void fileDownloadedChanged();

private slots:

    // Signal from QNetworkReply
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyToRead();

private:

    // QML Properties
    bool m_online;
    bool m_sslSupport;
    bool m_setup;
    double m_kilobytes;
    int m_flagsCount;


    bool m_fileDownloaded;
    QString m_currentFile;

    void checkNetworkState();
    int getFlagsCount();

    // Check files for game
    QMap<QString, QUrl> fileMap;
    QQueue<QUrl> downloadQueue;

    // For downloading game data files
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QFile stream;



    void download(const QUrl &url);
    QString getDownloadBasename(const QUrl &url);
    bool saveDownloadFile(const QString &filen, QIODevice *data);
    bool generateFlagsForDownloading();


};

#endif // FINITGAME_H
