#include <DelegateUI/delapp.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
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
    qmlRegisterSingletonInstance("EtToolKits", 1, 0, "ToolManager", ToolManager::instance());


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
    QObject::connect(callPluginKey, &QHotkey::activated, [&engine]() {
        // 动态创建或显示插件窗口
        QObject* pluginWin = nullptr;
        // 遍历已加载对象，避免重复创建
        for (QObject* obj : engine.rootObjects()) {
            if (obj->objectName() == "pluginMainWindow") {
                pluginWin = obj;
                break;
            }
        }
        if (!pluginWin) {
            // 第一次创建
            engine.load(QUrl(QStringLiteral("qrc:/qt/qml/EtToolkitsQml/ui/plugins/PluginMainWindow.qml")));
            for (QObject* obj : engine.rootObjects()) {
                if (obj->objectName() == "pluginMainWindow") {
                    pluginWin = obj;
                    break;
                }
            }
        }
        if (pluginWin) {
            QMetaObject::invokeMethod(pluginWin, "showWindow");
        }
    });

    return app.exec();
}
