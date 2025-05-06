#include <DelegateUI/delapp.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include "cert_util/gen_cert.h"
#include "random_test/RandomTest.h"


const QString applicationVersion = "0.0.1";

int main(int argc, char* argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickWindow::setDefaultAlphaBuffer(true);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("Trafalgar");
    app.setApplicationName("EtToolKits");
    app.setApplicationDisplayName("EtToolKits");
    app.setApplicationVersion(applicationVersion);

    QQmlApplicationEngine engine;
    DelApp::initialize(&engine);

    RandomTest randomTest;
    engine.rootContext()->setContextProperty("randomTest", &randomTest);
    GenCertHelper genCertHelper;
    engine.rootContext()->setContextProperty("genCertHelper", &genCertHelper);

    const QUrl url(QStringLiteral("qrc:/qt/qml/EtToolkitsQml/ui/Main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
