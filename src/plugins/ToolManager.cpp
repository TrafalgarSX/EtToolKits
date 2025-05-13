#include "ToolManager.h"
#include "PathConvert.h"
#include <QMetaObject>

// ToolManager.cpp
ToolManager* ToolManager::instance() 
{
    static ToolManager mgr;
    return &mgr;
}
ToolManager::ToolManager() 
{
    m_tools.append(new PathConvertTool(this));
    // 继续注册其他插件

    for(ITool* tool : m_tools) {
        // tools 信号触发
        connect(tool, &ITool::toolRunFailed, this, [this](const QString& name, const QString& error) {
            emit toolRunFailed(name, error);
        });
        connect(tool, &ITool::toolRunSuccess, this, [this](const QString& name, const QString& result) {
            emit toolRunSuccess(name, result);
        });
    }
}

QQmlListProperty<ITool> ToolManager::tools()
{
    QQmlListProperty<ITool> list(this, &m_tools);
    return list;
}

QVariantList ToolManager::filteredTools(const QString& keyword)
{
    QVariantList toolList;
    for(ITool* tool : m_tools) {
        if (tool->name().contains(keyword, Qt::CaseInsensitive)) {
            QVariantMap toolInfo;
            toolInfo["name"] = tool->name();
            toolInfo["description"] = tool->description();
            toolList.append(toolInfo);
        }
    }
    return toolList;
}

void ToolManager::selectTool(const QString& name)
{
    for (ITool* tool : m_tools) {
        if (tool->name() == name) {
            m_selectedToolName = name;
            emit selectedToolChanged();
            return;
        }
    }
    // 如果没有找到工具，可能需要处理错误
}

QString ToolManager::runTool(const QString& name, const QVariantMap& argMap)

{
    for (ITool* tool : m_tools) {
        if (tool->name() == name) {
            // 假设工具有一个 process 方法
            QMetaObject::invokeMethod(tool, [=]() {
                tool->process(argMap);
            }, Qt::QueuedConnection);
            return "Tool executed successfully";
        }
    }
    return "Tool not found";
}

QString ToolManager::selectedToolName() const
{
    return m_selectedToolName;
}

