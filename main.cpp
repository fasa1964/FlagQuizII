#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("FlagQuiz II");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("qtexample@flagquizII");
    app.setOrganizationDomain("qtexample@flagquizII.com");

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FlagQuizII", "Main");

    return app.exec();
}
