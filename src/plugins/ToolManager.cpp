#include "ToolManager.h"
#include "PathConvert.h"
#include "RecentProjects.h"
#include <QMetaObject>

// ToolManager.cpp
ToolManager* ToolManager::instance() 
{
    static ToolManager mgr;
    return &mgr;
}
ToolManager::ToolManager() 
{
    m_tools.append(new UnixPathConvertTool(this));
    m_tools.append(new WinPathConvertTool(this));
    m_tools.append(new RecentProjectsVSC(this));
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
            toolInfo["icon"] = tool->icon();
            toolInfo["name"] = tool->name();
            toolInfo["description"] = tool->description();
            toolInfo["hasCustomView"] = tool->hasCustomView();
            // toolInfo["customViewComponent"] = tool->customViewComponent();
            toolList.append(toolInfo);
        }
    }
    return toolList;
}

QVariantList ToolManager::allTools()
{
    QVariantList toolList;
    for(ITool* tool : m_tools) {
        QVariantMap toolInfo;
        toolInfo["icon"] = tool->icon();
        toolInfo["name"] = tool->name();
        toolInfo["description"] = tool->description();
        toolInfo["hasCustomView"] = tool->hasCustomView();
        toolInfo["source"] = tool->customViewUrl();
        toolList.append(toolInfo);
    }
    return toolList;
}


QVariantList ToolManager::recentProjectsVSC()
{
    for(ITool* tool : m_tools) {
        if (tool->name() == m_selectedToolName) {
            QVariantList recentProjects = static_cast<RecentProjectsVSC*>(tool)->recentProjects();
            return recentProjects;
        }
    }
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

QString ToolManager::runTool(const QVariantMap& argMap)

{
    for (ITool* tool : m_tools) {
        if (tool->name() == m_selectedToolName) {
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

