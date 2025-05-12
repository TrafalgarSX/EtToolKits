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
    QString name() const override { return "Windows 路径转换"; }
    QString desc() const override { return R"(Windows 路径转换，转换为 \\，或者 /)"; }
    Q_INVOKABLE bool process(const QString& in, int style) {
        // 实现路径转换逻辑, 并将结果复制到剪贴板
        QString result = in;
        // 检测路径合法性
        if (result.isEmpty()) {
            return false;
        }
        // 检测路径是否包含非法字符
        if (result.contains(QRegularExpression("[<>:\"/\\|?*]"))) {
            return false;
        }
        /*
         style == 1 将路径中的 \ 替换为 \\ 
         style == 2 将路径中的 \ 替换为  /
        */
        if (style == 1) {
            result.replace("\\", "\\\\");
        } else if (style == 2) {
            result.replace("\\", "/");
        }

        // 将结果复制到剪贴板
        QClipboard* clipboard = QGuiApplication::clipboard();
        clipboard->setText(result);
        return true;
    }
};

#endif // PATHCONVERT_H