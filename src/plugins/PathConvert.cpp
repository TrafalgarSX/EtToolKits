#include "PathConvert.h"

QString UnixPathConvertTool::name() const { return "windows path convert(Unix style)"; }

QString UnixPathConvertTool::description() const { return R"(Windows 路径转换，转换Unix 风格： /)"; }

void UnixPathConvertTool::process(QVariantMap argMap)
{
    // 实现路径转换逻辑, 并将结果复制到剪贴板
    QString result = argMap["path"].toString();
    // 检测路径合法性
    if (result.isEmpty()) {
        emit toolRunFailed(name(), "路径不能为空");
    }
    // 检测路径是否包含非法字符
    if (result.contains(QRegularExpression(R"([<>"|?*])"))) {
        emit toolRunFailed(name(), "路径包含非法字符");
    }
    /*
        style == 1 将路径中的 \ 替换为 \\
        style == 2 将路径中的 \ 替换为  /
    */
    result.replace("\\", "/");

    // 将结果复制到剪贴板
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    emit toolRunSuccess(name(), result);
}

QString WinPathConvertTool::name() const { return "windows path convert(Win style)"; }

QString WinPathConvertTool::description() const { return R"(Windows 路径转换，转换Win 风格： \\)"; }

void WinPathConvertTool::process(QVariantMap argMap)
{
    // 实现路径转换逻辑, 并将结果复制到剪贴板
    QString result = argMap["path"].toString();
    // 检测路径合法性
    if (result.isEmpty()) {
        emit toolRunFailed(name(), "路径不能为空");
    }
    // 检测路径是否包含非法字符
    if (result.contains(QRegularExpression(R"([<>"|?*])"))) {
        emit toolRunFailed(name(), "路径包含非法字符");
    }
    result.replace(R"(\)", R"(\\)");

    // 将结果复制到剪贴板
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    emit toolRunSuccess(name(), result);
}