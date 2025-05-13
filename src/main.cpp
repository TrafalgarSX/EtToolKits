#include <DelegateUI/delapp.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickView>
#include <QHotkey>
#include <QDebug>

#include "cert_util/gen_cert.h"
#include "random_test/RandomTest.h"
#include "plugins/ToolManager.h"


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

    // 注册 ToolManager 单例到 QML
    qmlRegisterSingletonInstance("EtToolKitsPlugin", 1, 0, "ToolManager", ToolManager::instance());


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

	auto callPluginKey = new QHotkey(QKeySequence(Qt::ControlModifier |Qt::AltModifier | Qt::Key_Z), true, &app);
	// auto callPluginKey = new QHotkey(QKeySequence(Qt::AltModifier | Qt::Key_Space), true, &app);
    QQmlApplicationEngine* enginePlugin = nullptr;
    QObject::connect(callPluginKey, &QHotkey::activated, [&enginePlugin, &app]() {
        qDebug() << "Hotkey activated!";
        const QUrl url(QStringLiteral("qrc:/qt/qml/EtToolkitsQml/ui/plugins/PluginMainWindow.qml"));
        if(!enginePlugin) {
            enginePlugin = new QQmlApplicationEngine;
            QObject::connect(
                        enginePlugin, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
                        Qt::QueuedConnection);
            enginePlugin->load(url);
        }
        // call pluginwindow js function
        if(enginePlugin->rootObjects().isEmpty()){
            return;
        }
        QObject *rootObject = enginePlugin->rootObjects().first();
        // 调用 QML 中的函数
        QMetaObject::invokeMethod(rootObject, "showPluginWindow");
    });

    return app.exec();
}
