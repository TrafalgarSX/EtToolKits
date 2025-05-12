```qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: dlg
    visible: false
    property alias toolSearchVisible: visible
    modal: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    contentItem: Rectangle {
        width: 400; height: 500; color: "#333"
        Column { anchors.fill: parent; spacing: 8; padding: 16
            TextField {
                id: search
                placeholderText: "搜索工具…"
            }
            ListView {
                model: ToolManager.tools.filter(tool => tool.name.indexOf(search.text) !== -1)
                delegate: ItemDelegate {
                    text: modelData.name
                    onClicked: {
                        let out = ToolManager.runTool(modelData.name, search.text)
                        Qt.application.clipboard.setText(out)
                        dlg.visible = false
                    }
                }
            }
        }
    }
}
```


```cpp
class ITool : public QObject {
    Q_OBJECT
  public:
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString desc READ desc CONSTANT)
    Q_INVOKABLE virtual QString process(const QString& in) = 0;
};

class ToolManager : public QObject {
   Q_OBJECT
   Q_PROPERTY(QQmlListProperty<ITool> tools READ tools CONSTANT)
   // init 时注册所有工具实例：RandomTestTool, GenCertTool...
   // Q_INVOKABLE QString runTool(const QString& name, const QString& input);
};
```

https://blog.csdn.net/ffffffeiyu/article/details/137114858

https://blog.csdn.net/bj5546/article/details/129100512

http://cppdebug.com/archives/471

https://www.cnblogs.com/wang1299/p/18567046