#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include <DelegateUI/delapp.h>
#include "random_test/RandomTest.h"

int main(int argc, char* argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickWindow::setDefaultAlphaBuffer(true);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("MenPenS");
    app.setApplicationName("DelegateUI");
    app.setApplicationDisplayName("DelegateUI Gallery");
    app.setApplicationVersion(DelApp::libVersion());

    QQmlApplicationEngine engine;

    DelApp::initialize(&engine);

    RandomTest randomTest;
    engine.rootContext()->setContextProperty("randomTest", &randomTest);

    const QUrl url(QStringLiteral("qrc:/qt/qml/EtToolkitsQml/ui/Main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

