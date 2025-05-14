import QtQuick
import QtQuick.Controls
import EtToolKitsPlugin

Item {
    id: recentProjectsVSC
    visible: true
    implicitHeight: projectList.implicitHeight + 10
    property var projects: ToolManager.recentProjectsVSC()

    ListView {
        // --- 新增：可见的最大条目数和条目高度 ---
        property int maxVisibleItems: 7
        property int itemHeight: 60

        id: projectList
        width: parent.width
        implicitHeight: visible ? Math.min(projects.length, maxVisibleItems) * itemHeight : 0
        clip: true

        model: projects
        delegate: ItemDelegate {
            required property var modelData

            id: wrapper
            width: ListView.view.width - 20
            background: Rectangle {
                color: ListView.isCurrentItem ? "#e0e7ef" : "#f5f6fa"
                radius: 8
            }

            contentItem: Row {
                spacing: 12
                Image {
                    source: modelData.icon
                    width: 40 
                    height: 40
                }
                Column {
                    spacing: 2
                    Text {
                        text: modelData.name
                        font.pixelSize: 18
                        color: "#23272e"
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        text: modelData.path
                        font.pixelSize: 13
                        color: "#888"
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignLeft
                        width: wrapper.ListView.view.width - 60
                    }
                }
            }
            onClicked: {
                const arg = new Object()
                arg["openProject"] = modelData.path
                console.log("Clicked on: " + modelData.name)
                console.log("Project path: " + modelData.path)
                ToolManager.runTool(arg)
                recentProjectsVSC.visible = false
            }
        }
        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Up) {
                currentIndex = Math.max(0, currentIndex - 1)
                event.accepted = true
            } else if (event.key === Qt.Key_Down) {
                currentIndex = Math.min(count - 1, currentIndex + 1)
                event.accepted = true
            } else if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                currentItem?.clicked()
                event.accepted = true
            }
        }
    }
}