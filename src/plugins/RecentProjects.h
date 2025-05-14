#ifndef _RECENTPROJECTS_H_
#define _RECENTPROJECTS_H_
#pragma once

#include "ITool.h"

class RecentProjectsVSC : public ITool {
    Q_OBJECT
public:
    explicit RecentProjectsVSC(QObject* parent = nullptr);
    ~RecentProjectsVSC() override = default;
    QUrl icon() const override; 
    QString name() const override; 
    bool hasCustomView() const override;
    QString description() const override;
    QUrl customViewUrl() const override; 
    void process(QVariantMap argMap) override;
    QVariantList recentProjects(); // 新增：QML 可调用
    void openProject(const QString& path); // 新增：QML 可调用
private:
    void loadVscInfo();
    QVariantList parseRecentProjects();
private:
    QString m_vscInfoFile;
    QString m_vscodeExe;
    QString m_stateDb;
};


#endif // _RECENTPROJECTS_H_