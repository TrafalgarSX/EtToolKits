import QtQuick
import QtQuick.Controls

Window {
    id: pluginWindow
    width: 500; height: 600
    visible: false
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    function showWindow() {
        pluginWindow.visible = true
        searchField.forceActiveFocus()
    }

    DelRectangle {
        anchors.fill: parent
        color: "#222"
        Column {
            anchors.fill: parent; spacing: 8; padding: 16
            DelInput {
                id: searchField
                placeholderText: "搜索插件…"
                onTextChanged: pluginListView.model = ToolManager.filteredTools(searchField.text)
            }
            ListView {
                id: pluginListView
                height: 200
                model: ToolManager.filteredTools(searchField.text)
                delegate: ItemDelegate {
                    text: modelData.name
                    onClicked: {
                        ToolManager.selectTool(modelData.name)
                        paramInput.text = ""
                        pluginViewLoader.sourceComponent = null
                        if (modelData.hasCustomView)
                            pluginViewLoader.sourceComponent = modelData.customViewComponent
                    }
                }
            }
            DelInput {
                id: paramInput
                placeholderText: "输入参数，回车执行"
                onAccepted: {
                    let result = ToolManager.runTool(ToolManager.selectedToolName, paramInput.text)
                    Qt.application.clipboard.setText(result)
                    pluginWindow.visible = false
                }
            }
            Loader {
                id: pluginViewLoader
                anchors.top: paramInput.bottom
                width: parent.width
            }
        }
    }
}