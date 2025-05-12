#ifndef ITOOL_H
#define ITOOL_H
#pragma once

#include <QObject>
#include <QString>
#include <QQmlEngine>

// ITool.h
class ITool : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString desc READ desc CONSTANT)
    Q_PROPERTY(bool hasCustomView READ hasCustomView CONSTANT)
public:
    explicit ITool(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ITool() = default;
    virtual QString name() const = 0;
    virtual QString desc() const = 0;
    virtual bool hasCustomView() const { return false; }
    Q_INVOKABLE virtual QObject* customViewComponent(QQmlEngine*) { return nullptr; }
};

#endif // ITOOL_H