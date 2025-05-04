import QtQuick
import QtQuick.Controls.Basic
import Qt.labs.qmlmodels
import Qt.labs.platform as Platform
import DelegateUI

Flickable {
    // contentHeight: mainRow.height
    id: main
    anchors.fill: parent
    clip: true
    ScrollBar.vertical: DelScrollBar { }

    property date startTimeDate

    Component {
        id: textDelegate

        Text {
            id: displayText
            leftPadding: 8
            rightPadding: 8
            font {
                family: DelTheme.Primary.fontPrimaryFamily
                pixelSize: DelTheme.Primary.fontPrimarySize
            }
            text: cellData
            color: DelTheme.Primary.colorTextBase
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            TextMetrics {
                id: displayWidth
                font: displayText.font
                text: displayText.text
            }

            TextMetrics {
                id: startWidth
                font: displayText.font
                text: {
                    let index = displayText.text.indexOf(filterInput);
                    if (index !== -1)
                        return displayText.text.substring(0, index);
                    else
                        return '';
                }
            }

            TextMetrics {
                id: filterWidth
                font: displayText.font
                text: filterInput
            }

            Rectangle {
                color: "red"
                opacity: 0.1
                x: startWidth.advanceWidth + (displayText.width - displayWidth.advanceWidth) * 0.5
                width: filterWidth.advanceWidth
                height: parent.height
            }
        }
    }

    Row {
        id: mainRow
        anchors.fill: parent
        anchors.margins: 10

        // Left Section
        Column {
            id: leftColumn
            width: parent.width * 0.6

            DelTableView {
                id: tableView
                width: parent.width
                height: 650
                // alternatingRow: true
                columns: [
                    {
                        title: '检测项',
                        dataIndex: 'randTestAlgorithm',
                        delegate: textDelegate,
                        width: tableView.width * 0.4 - 20,
                        selectionType: 'checkbox',

                    },
                    {
                        title: '>=显著水平样本数',
                        dataIndex: 'significantLevelSampleCount',
                        delegate: textDelegate,
                        width: tableView.width * 0.2 - 20,
                    },
                    {
                        title: '<显著水平样本数',
                        dataIndex: 'nonSignificantLevelSampleCount',
                        delegate: textDelegate,
                        width: tableView.width * 0.2,
                    },
                    {
                        title: '样本分布均匀性',
                        dataIndex: 'uniformityPTValue',
                        delegate: textDelegate,
                        width: tableView.width * 0.2,
                    },
                ]
                initModel: randomTest.randomTestListData

                Component.onCompleted: {
                    let checkKeyArray = [];
                    for(let i = 0; i < randomTest.randomTestListData.length; i++) { 
                        checkKeyArray.push(i)
                    }
                    console.log("checkKeyArray: " + checkKeyArray)
                    tableView.checkForRows(checkKeyArray)
                }
            }

            DelDivider {
                width: parent.width * 0.6
                height: 30
                titleAlign: DelDivider.Align_Center
            }

            Row {
                spacing: 10
                DelButton {
                    text: qsTr("反选")
                    width: tableView.width * 0.5 - 20
                    type: DelButton.Type_Filled
                    onClicked: {
                        let checkedKeys = tableView.getCheckedKeys()
                        console.log("checkedKeys: " + checkedKeys)
                        let newCheckKeyArray = [];
                        for(let i = 0; i < randomTest.randomTestListData.length; i++) { 
                            if(checkedKeys.indexOf(String(i)) === -1) {
                                newCheckKeyArray.push(i)
                            }
                        }
                        console.log("newCheckKeyArray: " + newCheckKeyArray)
                        tableView.clearAllCheckedKeys()
                        tableView.checkForRows(newCheckKeyArray)
                    }
                }
                DelTag {
                    text: "显著水平："
                    height: 30
                    width: tableView.width * 0.2 - 20
                }
                DelInput {
                    id: significantLevelInput
                    width: tableView.width * 0.3 - 20
                    height: 30
                    text: "0.01"

                    onTextChanged: {
                        if (text.length > 0) {
                            let value = parseFloat(text);
                            if (isNaN(value) || value < 0 || value > 1) {
                                text = "0.01";
                            } else {
                                randomTest.significantLevel(value);
                            }
                        } else {
                            text = "0.01";
                        }
                    }
                }
            }
        }

        DelDivider {
            width: 30
            height: parent.height
            orientation: Qt.Vertical
        }

        // Right Section
        Column {
            id: rightColumn
            spacing: 10
            width: parent.width * 0.4

            DelProgress {
                id: progressBar
                width: parent.width - 30
                height: 30
                percent: randomTest.progress * 100
                status: {
                    if (randomTest.progress == 1) {
                        return DelProgress.Status_Success
                    } else if (randomTest.progress == 0) {
                        return DelProgress.Status_Error
                    } else {
                        return DelProgress.Status_Active
                    }
                }
            }

            DelTag {
                text: "随机数样本文件大小建议为 1000*1000 bit(规范推荐，共测试 1000 个样本)"
                presetColor: "volcano"
                width: parent.width
                height: 30
                anchors.left: progressBar.left
            }

            Row {
                spacing: 10

                DelTag {
                    text: "单样本大小："
                    height: 30
                    width: progressBar.width * 0.2 - 20
                }

                DelText {
                    id: sampleSize
                    text: "规范推荐 1000*1000 bit"
                    verticalAlignment: Text.AlignVCenter
                    width: progressBar.width * 0.5 - 20
                    height: 30
                }
            }

            Row {
                spacing: 10
                DelTag {
                    text: "样本数量："
                    height: 30
                    width: progressBar.width * 0.2 - 20
                }

                DelText {
                    id: sampleCount
                    text: "规范推荐 1000 个样本"
                    verticalAlignment: Text.AlignVCenter
                    width: progressBar.width * 0.5 - 20
                    height: 30
                }
            }

            Row {
                spacing: 10
                DelTag {
                    text: "成功通过检测项，需要>=显著水平的样本数量："
                    height: 30
                    width: progressBar.width * 0.6 - 20
                }

                DelText {
                    id: needPassSampleCount
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    width: progressBar.width * 0.4 - 20
                    height: 30
                }
            }

            DelDivider {
                width: parent.width
                height: 30
                title: qsTr("随机数样本文件检测")
                titleAlign: DelDivider.Align_Center
            }

            Row {
                spacing: 5
                DelTag {
                    text: "样本路径："
                    height: 30
                    width: progressBar.width * 0.2 - 20
                }

                DelInput {
                    id: sampleFileUrl
                    text: ""
                    width: progressBar.width * 0.5
                    height: 30
                }

                DelButton {
                    text: qsTr("选择样本文件")
                    height: 30
                    width: progressBar.width * 0.3
                    type: DelButton.Type_Filled
                    onClicked: {
                        fileDialog.open()
                    }
                }
            }

            Row {
                spacing: 5
                DelTag {
                    text: "报告路径："
                    height: 30
                    width: progressBar.width * 0.2 - 20
                }

                DelInput {
                    id: reportFileUrl
                    text: ""
                    placeholderText: "TODO impliment"
                    width: progressBar.width * 0.5
                    height: 30
                }

                DelButton {
                    text: qsTr("开始检测")
                    height: 30
                    width: progressBar.width * 0.3
                    type: DelButton.Type_Filled
                    onClicked: {
                        console.log("checkedKeys: " + tableView.checkedKeys)
                        if(sampleFileUrl.text == "") {
                            message.warning("请先选择样本文件！")
                        } else if(tableView.checkedKeys.length == 0) {
                            message.warning("请至少选择一个检测项！")
                        }else {
                            randomTest.runRandomTest(tableView.checkedKeys)
                            main.startTimeDate = new Date(); // 获取当前时间
                            startTime.text = main.startTimeDate.toLocaleString(); // 格式化为本地时间字符串
                        }

                    }
                }
            }

            Row {
                spacing: 5
                DelTag {
                    text: "开始测试时间："
                    height: 30
                    width: progressBar.width * 0.2
                }

                DelText {
                    id: startTime
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    width: progressBar.width * 0.6
                    height: 30
                }
            }

            Row {
                spacing: 5
                DelTag {
                    text: "测试完成时间："
                    height: 30
                    width: progressBar.width * 0.2
                }

                DelText {
                    id: endTime
                    text: ""
                    verticalAlignment: Text.AlignVCenter
                    width: progressBar.width * 0.6
                    height: 30
                }
            }

            Row {
                spacing: 5
                DelTag {
                    id: testTimeTag
                    text: "耗时："
                    height: 30
                    width: progressBar.width
                    presetColor: "green"
                }
            }

            DelDivider {
                width: parent.width
                height: 30
                title: qsTr("软件随机数生成器测试数据生成")
                titleAlign: DelDivider.Align_Center
            }

            Row {
                spacing: 5
                DelTag {
                    text: "随机数测试数据生成路径："
                    height: 30
                    width: progressBar.width * 0.3
                }

                DelInput {
                    id: testDataDirUrl
                    text: ""
                    width: progressBar.width * 0.9 - 30
                    height: 30
                }
            }

            Row {
                spacing: 5
                DelButton {
                    text: qsTr("选择测试数据文件保存路径")
                    height: 30
                    width: progressBar.width * 0.5
                    type: DelButton.Type_Filled
                    onClicked: {
                        randFileGenFileDialog.open()
                    }
                }

                DelButton {
                    text: qsTr("开始生成测试数据")
                    height: 30
                    width: progressBar.width * 0.5
                    type: DelButton.Type_Filled
                    onClicked: {
                        // TODO call drng generate function
                    }
                }
            }
        }
    }

    Platform.FileDialog {
        id: fileDialog
        title: "选择随机数样本文件"
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        fileMode: Platform.FileDialog.ExistingFiles
        onAccepted: {
            let sampleFileUrlText = fileDialog.currentFile; // 转成本地路径

            const localPath = new URL(sampleFileUrlText).pathname.substr(Qt.platform.os == "windows" ? 1 : 0);
            randomTest.sampleUrl = sampleFileUrlText;
            sampleFileUrl.text = localPath;

            // 获取上一目录下的 report.csv 文件路径
            let reportdir = localPath.substring(0, localPath.lastIndexOf('/'));
            reportdir = reportdir.substring(0, reportdir.lastIndexOf('/'));
            let reportFilePath = reportdir + "/report.csv";
            reportFileUrl.text = reportFilePath;

            // 获取样本文件相关信息
            let sampleInfo = randomTest.checkSampleInfo(localPath);
            if(sampleInfo.length != 0) {
                sampleCount.text = sampleInfo[0].sampleCount;
                sampleSize.text = sampleInfo[0].sampleSize;
                needPassSampleCount.text = sampleInfo[0].needPassSampleCount;
            }

        }
    }

    Platform.FolderDialog {
        id: randFileGenFileDialog
        title: "选择随机数样本文件生成路径"
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        onAccepted: {
            let testDataDirUrlText = randFileGenFileDialog.folder; // 转成本地路径
            const localDir = new URL(testDataDirUrlText).pathname.substr(Qt.platform.os == "windows" ? 1 : 0);
            testDataDirUrl.text = localDir;
        }
    }

    DelMessage {
        id: message 
        z: 999
        parent: root.captionBar
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
    }

    DelPopup {
        id: testRetPopup
        x: (parent.width - width) * 0.5
        y: (parent.height - height) * 0.5
        width: 400
        height: 300
        parent: Overlay.overlay
        closePolicy: DelPopup.NoAutoClose
        movable: true
        resizable: true
        minimumX: 0
        maximumX: parent.width - width
        minimumY: 0
        maximumY: parent.height - height
        minimumWidth: 400
        minimumHeight: 300
        contentItem: Item {
            DelCaptionButton {
                id: popupCloseButton
                anchors.right: parent.right
                radiusBg: testRetPopup.radiusBg * 0.5
                colorText: colorIcon
                iconSource: DelIcon.CloseOutlined
                onClicked: testRetPopup.close();
            }

            Flickable {
                id: popupBg
                anchors {
                    top: popupCloseButton.bottom
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                }
                anchors.leftMargin: 25
                contentHeight: popupText.height
                contentWidth: popupText.width
                clip: true
                DelText {
                    id: popupText
                    width: parent.width
                    text: "检测失败，请检查样本文件是否符合规范！"
                    color: DelTheme.Primary.colorTextBase
                    wrapMode: TextArea.WrapAtWordBoundaryOrAnywhere // 自动换行

                }
                ScrollBar.vertical: DelScrollBar { }
                ScrollBar.horizontal: DelScrollBar { }
            }
        }
    }

    Connections {
        target: randomTest
        function onRandomTestListDataChanged(randomTestListData) {
            for(let i = 0; i < randomTestListData.length; i++) { 
                tableView.setRow(i, randomTestListData[i])
            }
        }
    }

    Connections{
        target: randomTest
        function onRandomTestSuccess(testRetInfo) {
            main.setTestTime()
            popupText.text = testRetInfo;
            // TODO 可以自定义更好看的成功提示框
            testRetPopup.open();
        }
    }

    Connections{
        target: randomTest
        function onRandomTestFailed(testRetInfo) {
            progressBar.status = DelProgress.Status_Exception
            main.setTestTime()
            popupText.text = testRetInfo;
            testRetPopup.open();
        }
    }

    function setTestTime() {
        let endTimeDate = new Date(); // 获取当前时间
        endTime.text = endTimeDate.toLocaleString(); // 格式化为本地时间字符串
        let startTimeValue = main.startTimeDate.getTime(); // 获取开始时间的时间戳
        let endTimeValue = endTimeDate.getTime(); // 获取结束时间的时间戳
        let timeDiff = endTimeValue - startTimeValue; // 计算时间差
        console.log("startTimeValue: " + startTimeValue)
        console.log("endTimeValue: " + endTimeValue)
        console.log("timeDiff: " + timeDiff)
        // 转换为时、分、秒
        let hours = Math.floor(timeDiff / 3600000); // 1小时 = 3600000毫秒
        let minutes = Math.floor((timeDiff % 3600000) / 60000); // 1分钟 = 60000毫秒
        let seconds = Math.floor((timeDiff % 60000) / 1000); // 1秒 = 1000毫秒

        // 使用模板字符串格式化
        testTimeTag.text = `耗时：${hours}小时 ${minutes}分钟 ${seconds}秒`;
    }
}


