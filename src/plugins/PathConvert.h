#ifndef PATHCONVERT_H
#define PATHCONVERT_H
#pragma once

#include "ITool.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QRegularExpression>

class UnixPathConvertTool : public ITool {
    Q_OBJECT
public:
    explicit UnixPathConvertTool(QObject* parent = nullptr) : ITool(parent) {}
    ~UnixPathConvertTool() override = default;
    QString icon() const override; 
    QString name() const override; 
    QString description() const override;
    Q_INVOKABLE void process(QVariantMap argMap) override;
};

class WinPathConvertTool : public ITool {
    Q_OBJECT
public:
    explicit WinPathConvertTool(QObject* parent = nullptr) : ITool(parent) {}
    ~WinPathConvertTool() override = default;
    QString icon() const override; 
    QString name() const override; 
    QString description() const override;
    Q_INVOKABLE void process(QVariantMap argMap) override;
};

#endif // PATHCONVERT_H