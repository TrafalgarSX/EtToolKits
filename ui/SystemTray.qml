import QtQuick
import DelegateUI
import Qt.labs.platform

Rectangle {
    id: root
    width: 80
    height: 200
    color: "transparent"

    property var windowId: null

    SystemTrayIcon {
        id: trayIcon
        visible: true
        icon.source: "qrc:/qt/qml/EtToolkitsQml/rc/icon.png"
        menu: Menu {
            MenuItem {
                text: qsTr("Show")
                onTriggered: {
                    windowId.show()
                    windowId.raise()
                    windowId.requestActivate()
                }
            }

            MenuItem {
                text: qsTr("Quit")
                onTriggered: {
                    Qt.exit(0)
                }
            }
        }
        onActivated: {
            windowId.show()
            windowId.raise()
            windowId.requestActivate()
        }
    }
}