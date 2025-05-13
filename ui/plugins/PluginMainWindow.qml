import QtQuick
import QtQuick.Controls
import DelegateUI
import EtToolKitsPlugin

Rectangle {
    id: pluginWindow
    width: 800; height: 200
    visible: true

    Column {
        anchors.fill: parent; 
        spacing: 8; 
        padding: 16
        DelInput {
            id: searchField
            width: parent.width - 20
            placeholderText: "搜索插件…"
        }
        DelDivider {
            width: pluginWindow.width
            height: 1
        }
        // Use Flow of Buttons instead of ListView
        Flow {
            width: parent.width - 20
            spacing: 8
            Repeater {
                model: ToolManager.filteredTools(searchField.text)
                delegate: Button {
                    text: modelData.name
                    onClicked: {
                        ToolManager.selectTool(modelData.name)
                        paramInput.text = ""
                        pluginViewLoader.sourceComponent = null
                        if (hasCustomView) {
                            pluginViewLoader.sourceComponent = customViewComponent
                        }
                    }
                }
            }
        }
        DelInput {
            id: paramInput
            width: parent.width - 20
            placeholderText: "输入参数，回车执行"
            onAccepted: {
                const arg = new Object()
                arg["path"] = paramInput.text
                arg["style"] = 2
                ToolManager.runTool(ToolManager.selectedToolName, arg)
            }
        }
        Loader {
            id: pluginViewLoader
            anchors.top: paramInput.bottom
            width: parent.width
        }
    }
}