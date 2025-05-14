pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import DelegateUI
import QtQuick.Effects
import EtToolKitsPlugin

ApplicationWindow {
    id: pluginWindow
    width: 800
    height: 600
    visible: false
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint
    color: "transparent" // 背景透明

    function hidePluginWindow() {
        pluginWindow.visible = false
    }
    function showPluginWindow() {
        pluginWindow.visible = true
        pluginWindow.requestActivate()
    }

    DelRectangle {
        id: mainRect
        width: pluginWindow.width
        implicitHeight: contentColumn.implicitHeight
        radius: 8
        anchors.margins: 8
        property bool isPluginInput: false // 是否是插件输入状态

        Column {
            id: contentColumn
            anchors.fill: parent
            spacing: 8
            padding: 16
            DelInput {
                id: searchField
                width: parent.width - 20
                implicitHeight: 50
                placeholderText: "搜索插件…"
                DragHandler {
                    id: handler
                    onActiveChanged: if (active) pluginWindow.startSystemMove()
                }

                onTextChanged: {
                    if (text.length > 0) {
                        pluginList.visible = true
                        pluginModel.clear()
                        let toolList = ToolManager.allTools()
                        for (var i = 0; i < toolList.length; i++) {
                            var tool = toolList[i]
                            if (tool.name.toLowerCase().indexOf(text.toLowerCase()) !== -1) {
                                pluginModel.append({
                                    name: tool.name,
                                    description: tool.description,
                                    icon: tool.icon,
                                    hasCustomView: tool.hasCustomView,
                                    // customViewComponent: tool.customViewComponent
                                    source: tool.source
                                })
                            }
                        }
                    } else {
                        pluginList.visible = false
                    }
                }

                onAccepted: {
                    if(mainRect.isPluginInput) {
                        const arg = new Object()
                        arg["path"] = searchField.text
                        ToolManager.runTool(arg)
                        searchField.text = ""
                    }
                }

                Keys.onPressed: function(event) {
                    if (event.key === Qt.Key_Down || event.key === Qt.Key_Tab) {
                        pluginList.forceActiveFocus()
                        event.accepted = true
                    }
                }
            }
            DelDivider {
                width: pluginWindow.width
                height: 1
            }
            // 插件列表
            ListView {
                // --- 新增：可见的最大条目数和条目高度 ---
                property int maxVisibleItems: 7
                property int itemHeight: 60

                id: pluginList
                currentIndex: 0
                clip: true
                visible: true
                focus: true

                width: pluginWindow.width
                // 大小自动：少于 maxVisibleItems 时刚好包裹全部条目，多了就滚动
                implicitHeight: Math.min(count, maxVisibleItems) * itemHeight

                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                ScrollBar.vertical: DelScrollBar {}

                model: ListModel { id: pluginModel }
                delegate: ItemDelegate {
                    required property var modelData

                    id: wrapper
                    width: ListView.view.width - 20
                    height: pluginList.itemHeight

                    contentItem: Row {
                        spacing: 12
                        DelAvatar {
                            width: 40; height: 40
                            imageSource: modelData.icon
                        }
                        Column {
                            spacing: 2
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
                                 width: wrapper.ListView.view.width - 60
                             }
                         }
                     }
                     onClicked: {
                         console.log("Clicked on: " + modelData.name)
                         ToolManager.selectTool(modelData.name)
                         searchField.text = ""
                         pluginList.visible = false
                         pluginViewLoader.sourceComponent = null
                         if (modelData.hasCustomView) {
                            //  pluginViewLoader.sourceComponent = modelData.customViewComponent
                             pluginViewLoader.source = modelData.source
                         } else {
                             searchField.forceActiveFocus()
                             mainRect.isPluginInput = true
                         }
                     }
                }
                Keys.onPressed: function(event) {
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
            Loader {
                id: pluginViewLoader
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
        shadowColor: "lightgray" // 阴影颜色
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