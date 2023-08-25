#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <finitgame.h>
#include <fgame.h>
#include <fgamesettings.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("FlagQuiz II");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("qtexample@flagquizII");
    app.setOrganizationDomain("qtexample@flagquizII.com");

    qmlRegisterType<FInitGame>("FInitGame", 1, 0, "Init");
    qmlRegisterType<FGame>("FGame", 1, 0, "Game");
    qmlRegisterType<FGameSettings>("FGameSettings", 1, 0, "GameSettings");


    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FlagQuizII", "Main");

    return app.exec();
}
