#ifndef _RECENTPROJECTS_H_
#define _RECENTPROJECTS_H_
#pragma once

#include "ITool.h"

class RecentProjectsVSC : public ITool {
    Q_OBJECT
public:
    explicit RecentProjectsVSC(QObject* parent = nullptr) : ITool(parent) {}
    ~RecentProjectsVSC() override = default;
    QString icon() const override; 
    QString name() const override; 
    QString description() const override;
    QUrl customViewUrl() const override; 
};


#endif // _RECENTPROJECTS_H_