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
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(bool hasCustomView READ hasCustomView CONSTANT)
public:
    explicit ITool(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ITool() = default;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual bool hasCustomView() const { return false; }
    virtual void process(QVariantMap argMap) = 0;
    Q_INVOKABLE virtual QObject* customViewComponent(QQmlEngine*) { return nullptr; }
signals:
    void toolRunFailed(const QString& name, const QString& error);
    void toolRunSuccess(const QString& name, const QString& result);
};

#endif // ITOOL_H