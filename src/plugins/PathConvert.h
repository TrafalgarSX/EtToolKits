#ifndef PATHCONVERT_H
#define PATHCONVERT_H
#pragma once

#include "ITool.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QRegularExpression>

// RandomTestTool.h
class PathConvertTool : public ITool {
    Q_OBJECT
public:
    explicit PathConvertTool(QObject* parent = nullptr) : ITool(parent) {}
    ~PathConvertTool() override = default;
    QString name() const override; 
    QString description() const override;
    Q_INVOKABLE void process(QVariantMap argMap) override;
};

#endif // PATHCONVERT_H