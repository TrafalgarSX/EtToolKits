#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H
#pragma once

// ToolManager.h
#include <QQmlListProperty>
#include <QVector>
#include "ITool.h"

class ToolManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<ITool> tools READ tools CONSTANT)
    Q_PROPERTY(QString selectedToolName READ selectedToolName NOTIFY selectedToolChanged)
public:
    static ToolManager* instance();
    QQmlListProperty<ITool> tools();
    Q_INVOKABLE QVariantList filteredTools(const QString& keyword);
    Q_INVOKABLE QVariantList allTools();
    Q_INVOKABLE QVariantList recentProjectsVSC();
    Q_INVOKABLE void selectTool(const QString& name);
    Q_INVOKABLE QString runTool(const QVariantMap& argMap);
    QString selectedToolName() const;
signals:
    void selectedToolChanged();
    void toolRunFailed(const QString& name, const QString& error);
    void toolRunSuccess(const QString& name, const QString& result);
private:
    ToolManager();
private:
    QVector<ITool*> m_tools;
    QString m_selectedToolName;
    // ...注册插件、查找插件等实现...
};

#endif // TOOLMANAGER_H