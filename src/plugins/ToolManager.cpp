#include "ToolManager.h"
#include "PathConvert.h"

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
}