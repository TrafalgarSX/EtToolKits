#include "PathConvert.h"

QString PathConvertTool::name() const { return "Windows 路径转换(path convert)"; }

QString PathConvertTool::description() const { return R"(Windows 路径转换，转换为 \\，或者 /)"; }

void PathConvertTool::process(QVariantMap argMap)
{
    // 实现路径转换逻辑, 并将结果复制到剪贴板
    if (argMap.size() != 2) {
        emit toolRunFailed(name(), "参数错误");
    }

    QString result = argMap["path"].toString();
    int style = argMap["style"].toInt();
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
    if (style == 1) {
        result.replace("\\", "\\\\");
    } else if (style == 2) {
        result.replace("\\", "/");
    }

    // 将结果复制到剪贴板
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(result);
    emit toolRunSuccess(name(), result);
}