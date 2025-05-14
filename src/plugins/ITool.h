#ifndef ITOOL_H
#define ITOOL_H
#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <QQmlEngine>

// ITool.h
class ITool : public QObject {
    Q_OBJECT
public:
    explicit ITool(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ITool() = default;
    virtual QUrl icon() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual bool hasCustomView() const { return false; }
    virtual QUrl customViewUrl() const { return QUrl(); }
    virtual void process(QVariantMap argMap) { (void)argMap; };
    Q_INVOKABLE virtual QObject* customViewComponent(QQmlEngine*) { return nullptr; }
signals:
    void toolRunFailed(const QString& name, const QString& error);
    void toolRunSuccess(const QString& name, const QString& result);
};

#endif // ITOOL_H