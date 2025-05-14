#include "RecentProjects.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

RecentProjectsVSC::RecentProjectsVSC(QObject* parent) : ITool(parent)
{
    // 假设 m_vscInfoFile 已经被正确设置为配置文件路径
    m_vscInfoFile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/vsc_info.json";
    loadVscInfo();
}

QUrl RecentProjectsVSC::icon() const { return QUrl("qrc:/qt/qml/EtToolkitsQml/rc/vscode.png"); }

QString RecentProjectsVSC::name() const { return "vs code recent projects"; }

bool RecentProjectsVSC::hasCustomView() const { return true; }

QString RecentProjectsVSC::description() const { return R"(visual studio code 最近打开的项目)"; }

QUrl RecentProjectsVSC::customViewUrl() const
{
    return QUrl("qrc:/qt/qml/EtToolkitsQml/ui/plugins/RecentProjectsVSC.qml");
}

void RecentProjectsVSC::loadVscInfo()
{
    QFile file(m_vscInfoFile);
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();
    m_vscodeExe = obj.value("vscodeExe").toString();
    m_stateDb = obj.value("stateDb").toString();
}

QVariantList RecentProjectsVSC::recentProjects() { return parseRecentProjects(); }

QVariantList RecentProjectsVSC::parseRecentProjects()
{
    QVariantList result;
    if (m_stateDb.isEmpty()) return result;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "vscdb_conn");
    db.setDatabaseName(m_stateDb);
    if (!db.open()) {
        qWarning() << "Failed to open vscdb:" << db.lastError();
        return result;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT value FROM ItemTable WHERE key='history.recentlyOpenedPathsList'")) {
        db.close();
        QSqlDatabase::removeDatabase("vscdb_conn");
        return result;
    }

    if (query.next()) {
        QByteArray value = query.value(0).toByteArray();
        QJsonDocument doc = QJsonDocument::fromJson(value);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QJsonArray entries = obj.value("entries").toArray();
            for (const QJsonValue& entryVal : entries) {
                QJsonObject entry = entryVal.toObject();
                QString folderUri = entry.value("folderUri").toString();
                if (folderUri.isEmpty()) continue;
                QUrl url(folderUri);
                QString path = url.toLocalFile();
                if (path.isEmpty()) path = folderUri;  // 兼容 wsl/remote
                QString name = path.section('/', -1, -1);
                if (name.isEmpty()) name = path;
                QVariantMap item;
                item["name"] = name;
                item["path"] = path;
                item["icon"] = icon();
                result.append(item);
            }
        }
    }
    db.close();
    QSqlDatabase::removeDatabase("vscdb_conn");
    return result;
}

void RecentProjectsVSC::openProject(const QString& path)
{
    /*
        神经病， 在 vscode 里运行项目（Debug 和 Release）的时候调用 QProcess 打不开 vscode, 到外面的命令行可以
        浪费我一个多小时， discord 也打不开，但是其他 cmake-gui obsidian everything notepad 都可以
        可能是 vscode 的环境变量问题或者是 electron 的问题, 但是 obsidian 也是 electron 的
        太奇怪，不知道为什么
    */
    if (m_vscodeExe.isEmpty() || path.isEmpty()) return;
    QString cmd = "\"" + m_vscodeExe + "\"";
    bool ok = QProcess::startDetached(cmd, QStringList() << path);
    if (!ok) {
        emit toolRunFailed(name(), "打开项目失败");
        qWarning() << "Failed to open project:" << path;
        return;
    }
}

void RecentProjectsVSC::process(QVariantMap argMap) 
{
    if(argMap.contains("openProject")) {
        QString path = argMap.value("openProject").toString();
        if (path.isEmpty()) {
            emit toolRunFailed(name(), "路径不能为空");
        }
        openProject(path);
        emit toolRunSuccess(name(), "打开项目成功");
    }
    else {
        emit toolRunFailed(name(), "参数错误");
    }
}