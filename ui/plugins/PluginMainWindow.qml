import QtQuick
import QtQuick.Controls
import DelegateUI
import QtQuick.Effects
import EtToolKitsPlugin

ApplicationWindow {
    id: pluginWindow
    width: 800; 
    height: 200
    visible: false 
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint
    color: "transparent" // 背景透明

    function hidePluginWindow() {
        pluginWindow.visible = false
    }
    function showPluginWindow() {
        pluginWindow.visible = true
    }

    DelRectangle {
        id: mainRect
        anchors.fill: parent
        radius: 8
        anchors.margins: 5
        border.width: 1

        Column {
            anchors.fill: parent; 
            spacing: 8; 
            padding: 16
            DelInput {
                id: searchField
                width: parent.width - 20
                implicitHeight: 70
                placeholderText: "搜索插件…"
                DragHandler {
                    id: handler
                    onActiveChanged: if (active) pluginWindow.startSystemMove()
                }
            }
            DelDivider {
                width: pluginWindow.width
                height: 1
            }
            // 插件列表
            ListView {
                id: pluginList
                Layout.fillWidth: true
                Layout.preferredHeight: 700
                model: ToolManager.filteredTools(searchField.text)
                clip: true
                currentIndex: 0
                visible: true
                ScrollBar.vertical: DelScrollBar { }
                delegate: ItemDelegate {
                    width: parent.width
                    background: Rectangle {
                        color: ListView.isCurrentItem ? "#e0e7ef" : "#f5f6fa" // 高亮/普通灰色
                        radius: 8
                    }
                    contentItem: Row {
                        spacing: 12
                        anchors.verticalCenter: parent.verticalCenter
                        DelAvatar {
                            width: 40; height: 40
                            anchors.verticalCenter: parent.verticalCenter
                            textSource: "U"
                            colorText: "#F56A00"
                            colorBg: "#FDE3CF"
                            // 优先展示图片，否则展示首字母
                            // imageSource: modelData.icon ? modelData.icon : ""
                        }
                        Column {
                            spacing: 2
                            anchors.verticalCenter: parent.verticalCenter
                            DelText {
                                text: modelData.name
                                font.pixelSize: 18
                                color: "#23272e"
                                font.bold: true
                                horizontalAlignment: Text.AlignLeft
                            }
                            DelText {
                                text: modelData.description
                                font.pixelSize: 13
                                color: "#888"
                                horizontalAlignment: Text.AlignLeft
                                elide: Text.ElideRight
                                width: parent.width - 60
                            }
                        }
                    }
                    onClicked: {
                        ToolManager.selectTool(modelData.name)
                        searchField.text = ""
                        pluginViewLoader.sourceComponent = null
                        if (modelData.hasCustomView) {
                            pluginViewLoader.sourceComponent = modelData.customViewComponent
                        } else {
                            searchField.forceActiveFocus()
                        }
                    }

                    function activate() {
                        ToolManager.selectTool(modelData.name)
                        searchField.text = ""
                        pluginViewLoader.sourceComponent = null
                        if (modelData.hasCustomView) {
                            pluginViewLoader.sourceComponent = modelData.customViewComponent
                        } else {
                            searchField.forceActiveFocus()
                        }
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
                        currentItem?.activate()
                        event.accepted = true
                    }
                }
            }
            Loader {
                id: pluginViewLoader
                anchors.top: searchField.bottom
                width: parent.width
            }
        }

    } // mainRect
    
    // 修改 MultiEffect 参数，注意 shadowBlur 值和偏移
    MultiEffect {
        id: effect
        anchors.fill: mainRect
        source: mainRect
        shadowEnabled: true
        shadowColor: "red"
        shadowBlur: 0.5           // 增大模糊半径
        autoPaddingEnabled: true
        shadowHorizontalOffset: 0  // 无偏移
        shadowVerticalOffset: 0   // 无偏移
    }

    Shortcut {
        sequence: "Esc"
        onActivated: pluginWindow.visible = false
    }
    Shortcut {
        sequence: "Ctrl+W"
        onActivated: Qt.quit()
    }

} // pluginWindow