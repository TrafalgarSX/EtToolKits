#include "RecentProjects.h"

QString RecentProjectsVSC::icon() const { return ""; }

QString RecentProjectsVSC::name() const { return "vs code recent projects"; }

QString RecentProjectsVSC::description() const { return R"(visual studio code 最近打开的项目)"; }

QUrl RecentProjectsVSC::customViewUrl() const { return QUrl("qrc:/qt/qml/EtToolkitsQml/ui/plugins/RecentProjectsVSC.qml"); }