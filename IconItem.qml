import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
Rectangle{
    id:icon
    required property Item dragPrarent;
    property int visualIndex:0;
    property string name;
    property url iconUrl;
    color:"transparent"
    width:100
    height:100

    anchors{
        horizontalCenter: parent.horizontalCenter
        verticalCenter :parent.verticalCenter
    }
    radius:5;
    DragHandler{
        id:dragHandler
    }
    Drag.active: dragHandler.active
    Drag.source: icon
    Drag.hotSpot.x: icon.width/2
    Drag.hotSpot.y: icon.height/2
    //图片文字列
    ColumnLayout{
        id:col
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 8
        Image{
            id: img;
            width: 64
            height: 64

            fillMode: Image.PreserveAspectFit
            smooth: true
            source:iconUrl
            horizontalAlignment: Image.AlignHCenter
            Layout.alignment: Qt.AlignHCenter

        }//Image
        Text{
            id:txtGrid
            text:icon.name
            Layout.fillWidth: true
            maximumLineCount: 2
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignBottom
            font.pixelSize:parent.GridView.isCurrentItem?16:14
            color:parent.GridView.isCurrentItem?"firebrick":"black"
        }//text
    }//colunmLayout

    //状态
    states:State{
        name:"active"
        when:dragHandler.active
        PropertyChanges {
            target: icon
            scale:0.8
            z:10

        }
        ParentChange{
            target:icon
            //激活改变父
            parent:icon.dragPrarent
        }

        AnchorChanges{
            target:icon
            //不锁定锚点
            anchors{
                horizontalCenter: undefined
                verticalCenter: undefined
            }
        }
    }

    //动画
    SequentialAnimation on rotation {
        NumberAnimation { to:  14; duration: 60 }
        NumberAnimation { to: -14; duration: 120 }
        NumberAnimation { to:  0; duration: 60 }
        running:icon.state == "active"
        loops: Animation.Infinite; alwaysRunToEnd: true
    } //swqutialAnimation
    //缩放动画
    transitions: Transition { NumberAnimation { property: "scale"; duration: 200} }

}//rectangle
