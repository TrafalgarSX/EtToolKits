pragma ComponentBehavior: Bound

import QtQuick
import DelegateUI

DelWindow {
    id: root
    width: 1500
    height: 800
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("EtToolkits")
    followThemeSwitch: false
    captionBar.themeButtonVisible: true
    captionBar.topButtonVisible: true
    captionBar.winIconWidth: 22
    captionBar.winIconHeight: 22
    captionBar.winIconDelegate: Item {
        Image {
            id: winIcon
            source: "qrc:/qt/qml/EtToolkitsQml/rc/icon.png"
            width: 22
            height: 22
            fillMode: Image.PreserveAspectFit
        }

    }
    captionBar.topCallback: (checked) => {
                                DelApi.setWindowStaysOnTopHint(root, checked);
                            }

    Component.onCompleted: {
        setSpecialEffect(DelWindow.Win_MicaAlt);
    }
    onWidthChanged: {
        galleryMenu.compactMode = width < 1100;
    }

    Rectangle {
        id: rootBAckground
        anchors.fill: content
        opacity: 0.2
    }

    Rectangle {
        id: themeCircle
        x: r - width
        y: -height * 0.5
        width: 0
        height: 0
        radius: width * 0.5
        color: "#141414"

        property real r: Math.sqrt(parent.width * parent.width + parent.height * parent.height)

        NumberAnimation {
            running: DelTheme.isDark
            properties: "width,height"
            target: themeCircle
            from: 0
            to: themeCircle.r * 2
            duration: DelTheme.Primary.durationMid
            onStarted: {
                root.setWindowMode(true);
                themeCircle.visible = true;
            }
            onFinished: {
                themeCircle.visible = false;
                themeCircle.width = Qt.binding(() => themeCircle.r * 2);
                themeCircle.height = Qt.binding(() => themeCircle.r * 2);
                if (root.specialEffect === DelWindow.None)
                    root.color = DelTheme.Primary.colorBgBase;
                rootBAckground.color = DelTheme.Primary.colorBgBase;
            }
        }

        NumberAnimation {
            running: !DelTheme.isDark
            properties: "width,height"
            target: themeCircle
            from: themeCircle.r * 2
            to: 0
            duration: DelTheme.Primary.durationMid
            onStarted: {
                root.setWindowMode(false);
                themeCircle.visible = true;
                if (root.specialEffect === DelWindow.None)
                    root.color = DelTheme.Primary.colorBgBase;
                rootBAckground.color = DelTheme.Primary.colorBgBase;
            }
            onFinished: {
                themeCircle.width = 0;
                themeCircle.height = 0;
            }
        }
    }

    Item {
        id: content
        anchors.top: root.captionBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        DelMenu {
            id: galleryMenu
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: aboutButton.top
            showEdge: true
            defaultMenuWidth: 300
            defaultMenuIconSize: 18
            defaultSelectedKey: ["HomePage"]
            onClickMenu: function(deep, key, data) {
                console.debug("onClickMenu", deep, key, JSON.stringify(data));
                if (data && data.source) {
                    containerLoader.visible = true;
                    containerLoader.source = "";
                    containerLoader.source = data.source;
                } else if (data && data.isTheme) {
                    containerLoader.visible = false;
                }
            }
            initModel: [
                {
                    key: "HomePage",
                    label: qsTr("首页"),
                    iconSource: DelIcon.HomeOutlined,
                    source: "qrc:/qt/qml/EtToolkitsQml/ui/HomePage.qml",
                },
                {
                    type: "divider"
                },
                {
                    label: qsTr("检测工具"),
                    iconSource: DelIcon.FundProjectionScreenOutlined,
                    menuChildren: [
                        {
                            key: "RandomTest",
                            label: qsTr("随机数检测"),
                            source: "qrc:/qt/qml/EtToolkitsQml/ui/test_tools/RandomTest.qml",
                        }
                    ]
                },
                {
                    label: qsTr("证书工具"),
                    iconSource: DelIcon.ToolOutlined,
                    menuChildren: [
                        {
                            key: "GenCert",
                            label: qsTr("证书生成"),
                            source: "qrc:/qt/qml/EtToolkitsQml/ui/cert_tools/GenCert.qml",
                        }
                    ]
                }
            ]
        }

        DelDivider {
            width: galleryMenu.width
            height: 1
            anchors.bottom: aboutButton.top
        }

        Loader {
            id: aboutLoader
            visible: false
            sourceComponent: AboutPage { visible: aboutLoader.visible }
        }

        Loader {
            id: settingsLoader
            visible: false
            sourceComponent: SettingsPage { visible: settingsLoader.visible }
        }

        DelIconButton {
            id: aboutButton
            width: galleryMenu.width
            height: 40
            anchors.bottom: setttingsButton.top
            type: DelButton.Type_Text
            radiusBg: 0
            text: galleryMenu.compactMode ? "" : qsTr("关于")
            colorText: DelTheme.Primary.colorTextBase
            iconSize: galleryMenu.defaultMenuIconSize
            iconSource: DelIcon.UserOutlined
            onClicked: {
                if (aboutLoader.visible)
                    aboutLoader.visible = false;
                else {
                    aboutLoader.visible = true;
                    settingsLoader.visible = false;
                }
            }
        }

        DelIconButton {
            id: setttingsButton
            width: galleryMenu.width
            height: 40
            anchors.bottom: parent.bottom
            type: DelButton.Type_Text
            radiusBg: 0
            text: galleryMenu.compactMode ? "" : qsTr("设置")
            colorText: DelTheme.Primary.colorTextBase
            iconSize: galleryMenu.defaultMenuIconSize
            iconSource: DelIcon.SettingOutlined
            onClicked: {
                if (settingsLoader.visible)
                    settingsLoader.visible = false;
                else {
                    settingsLoader.visible = true;
                    aboutLoader.visible = false;
                }
            }
        }

        Item {
            id: container
            anchors.left: galleryMenu.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5
            clip: true

            Loader {
                id: containerLoader
                anchors.fill: parent

                NumberAnimation on opacity {
                    running: containerLoader.status == Loader.Ready
                    from: 0
                    to: 1
                    duration: DelTheme.Primary.durationSlow
                }
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+W"
        onActivated: Qt.quit()
    }

    SystemTray {
        id: toolKitsTray
        windowId: root
    }
}
