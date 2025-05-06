import QtQuick
import QtQuick.Controls.Universal  as QC
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.platform as Platform
import QtQuick.Effects
import DelegateUI
import "utils.js" as Utils

Flickable {
    id: main
    anchors.fill: parent
    clip: true
    ScrollBar.vertical: DelScrollBar { }
    property string currentFolderTarget: ""
    property string tagColor: "blue"
    property string signPubKeyUrl: ""
    property string encPubKeyUrl: ""

    ColumnLayout {
        id: topLayout
        anchors.fill: parent
        clip: true // 剪裁超出边界的内容


        GroupBox {
            id: signCertbox
            title: qsTr("签名证书")
            Layout.fillWidth: true   // 水平方向充满
            Layout.fillHeight: true  // 垂直方向充满

            background: Rectangle {
                color: DelTheme.Primary.colorBgBase
                border.color: "lightgrey" // 边框颜色
                border.width: 1 // 边框宽度
                radius: 5 // 圆角
            }

            GridLayout {
                id: gridLayout
                anchors.fill: parent
                anchors.rightMargin: 12
                layer.enabled: false
                rows: 3
                columns: 2

                DelTag {
                    id: signPubKeyLabel
                    text: qsTr("签名公钥:")
                    presetColor: main.tagColor
                    font.bold: true
                    font.weight: Font.Bold
                    Layout.preferredHeight: parent.height / 8
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    Layout.row: 0
                    Layout.column: 0
                }

                DelInput {
                    id: signPubKeyHex
                    verticalAlignment: Text.AlignVCenter
                    placeholderText: qsTr("请输入16进制字符串(64字节，128个字符)")
                    font.pixelSize: DelTheme.Primary.fontPrimarySize - 2
                    onTextChanged: {
                        if (!Utils.isHexString(signPubKeyHex.text)) {
                            signPubKeyHex.colorText = "red"
                        } else {
                            signPubKeyHex.colorText = DelTheme.DelInput.colorText
                        }
                    }
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 8
                    Layout.row: 0
                    Layout.column: 1
                }

                DelTag {
                    id: signPubKeyPathLabel
                    text: qsTr("签名证书保存路径:")
                    presetColor: main.tagColor
                    font.bold: true
                    font.weight: Font.Bold
                    Layout.preferredHeight: parent.height / 8
                    Layout.row: 1
                    Layout.column: 0
                }

                DelInput {
                    id: signPubKeyPath
                    enabled: false
                    font.pixelSize: DelTheme.Primary.fontPrimarySize - 2
                    verticalAlignment: Text.AlignVCenter
                    placeholderText: qsTr("请选择存储目录")
                    // 计算的高度与最小高度取最大值，确保不会小于最小高度要求
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 8
                    Layout.row: 1
                    Layout.column: 1
                }

                Rectangle {
                    id: signBtnRect
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.row: 2
                    Layout.preferredHeight: parent.height / 8
                    color: DelTheme.Primary.colorBgBase
                    DelButton {
                        id: signSelectDir
                        text: qsTr("选择保存路径")
                        anchors.verticalCenter: parent.verticalCenter // 仅垂直居中
                        height: parent.height
                        width: parent.width / 6
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width / 4
                        type: DelButton.Type_Filled
                        onClicked: {
                            main.currentFolderTarget = "sign"
                            fileDialog.open()
                        }
                    }

                    DelButton {
                        id: genSignCert
                        text: qsTr("生成签名证书")
                        anchors.verticalCenter: parent.verticalCenter // 仅垂直居中
                        height: parent.height
                        width: parent.width / 6
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width / 4
                        type: DelButton.Type_Filled
                        onClicked: {
                            if(signPubKeyHex.text === "") {
                                messageHint.warning(qsTr("签名公钥不能为空！"))
                                return
                            }
                            if(signPubKeyPath.text === "") {
                                messageHint.warning(qsTr("签名公钥保存路径不能为空！"))
                                return
                            }

                            var success = genCertHelper.genCert(signPubKeyHex.text, main.signPubKeyUrl)
                            if(success) {
                                messageHint.success(qsTr("签名证书生成成功！"))
                            }else{
                                messageHint.error(qsTr("签名证书生成失败！"))
                            }
                        }
                    }
                }
            }
        }
        GroupBox {
            id: encCertbox
            title: qsTr("加密证书")
            Layout.fillWidth: true   // 水平方向充满
            Layout.fillHeight: true  // 垂直方向充满
            background: Rectangle {
                color: DelTheme.Primary.colorBgBase
                border.color: "lightgrey" // 边框颜色
                border.width: 1 // 边框宽度
                radius: 5 // 圆角
            }
            GridLayout {
                id: encGridLayout
                anchors.fill: parent
                anchors.rightMargin: 12
                layer.enabled: false
                rows: 3
                columns: 2

                DelTag {
                    id: encPubKeyLabel
                    text: qsTr("加密公钥:")
                    presetColor: main.tagColor
                    font.weight: Font.Bold
                    font.bold: true
                    Layout.preferredHeight: parent.height / 8
                    Layout.row: 0
                    Layout.column: 0
                }

                DelInput {
                    id: encPubKeyHex
                    placeholderText: qsTr("请输入16进制字符串（64字节，128个字符）")
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: DelTheme.Primary.fontPrimarySize - 2
                    onTextChanged: {
                        if (!Utils.isHexString(encPubKeyHex.text)) {
                            encPubKeyHex.colorText = "red"
                        } else {
                            encPubKeyHex.colorText = DelTheme.DelInput.colorText
                        }
                    }
                    Layout.fillWidth: true
                    // 计算的高度与最小高度取最大值，确保不会小于最小高度要求
                    Layout.preferredHeight: parent.height / 8
                    Layout.row: 0
                    Layout.column: 1
                }

                DelTag {
                    id: encPubKeyPathLabel
                    text: qsTr("加密证书保存路径:")
                    presetColor: main.tagColor
                    font.bold: true
                    font.weight: Font.Bold
                    Layout.preferredHeight: parent.height / 8
                    Layout.row: 1
                    Layout.column: 0
                }

                DelInput {
                    id: encPubKeyPath
                    enabled: false
                    font.pixelSize: DelTheme.Primary.fontPrimarySize - 2
                    verticalAlignment: Text.AlignVCenter
                    placeholderText: qsTr("请选择存储目录")
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 8
                    Layout.row: 1
                    Layout.column: 1
                }

                Rectangle {
                    id: encBtnRect
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.row: 2
                    Layout.preferredHeight: parent.height / 8
                    color: DelTheme.Primary.colorBgBase
                    DelButton {
                        id: encSelectDir
                        text: qsTr("选择保存路径")
                        anchors.verticalCenter: parent.verticalCenter // 仅垂直居中
                        width: parent.width / 6
                        height: parent.height
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width / 4
                        type: DelButton.Type_Filled
                        onClicked: {
                            main.currentFolderTarget = "enc"
                            fileDialog.open()
                        }
                    }

                    DelButton {
                        id: genEncCert
                        text: qsTr("生成加密证书")
                        anchors.verticalCenter: parent.verticalCenter // 仅垂直居中
                        height: parent.height
                        width: parent.width / 6
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width / 4
                        type: DelButton.Type_Filled
                        onClicked: {
                            if(encPubKeyHex.text === "") {
                                messageHint.warning(qsTr("加密公钥不能为空！"))
                                return
                            }
                            if(encPubKeyPath.text === "") {
                                messageHint.warning(qsTr("加密公钥保存路径不能为空！"))
                                return
                            }
                            var success = genCertHelper.genCert(encPubKeyHex.text, main.encPubKeyUrl)
                            if(success) {
                                messageHint.success(qsTr("加密证书生成成功！"))
                            }else{
                                messageHint.error(qsTr("加密证书生成失败！"))
                            }
                        }
                    }
                }
            }
        }
        Platform.FileDialog {
            id: fileDialog
            title: qsTr("选择保存路径")
            folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
            currentFile: {
                if (main.currentFolderTarget === "sign") {
                    return "sign_cert.crt"
                } else {
                    return "enc_cert.crt"
                }
            }
            fileMode: FileDialog.SaveFile
            nameFilters: ["cert files (*.pem *.crt)"]
            onAccepted: {
                const localFilePath = new URL(fileDialog.currentFile).pathname.substr(Qt.platform.os == "windows" ? 1 : 0);
                if (main.currentFolderTarget === "sign") {
                    main.signPubKeyUrl = fileDialog.currentFile
                    signPubKeyPath.text = localFilePath
                } else {
                    main.encPubKeyUrl = fileDialog.currentFile
                    encPubKeyPath.text = localFilePath
                }
            }
        }
    }
    DelMessage {
        id: messageHint
        z: 999
        parent: root.captionBar
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        closeButtonVisible: true
    }

} 