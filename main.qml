import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1
 import QtQuick.Layouts 1.15
ApplicationWindow{
    id:root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    flags:Qt.WindowCloseButtonHint | Qt.CustomizeWindowHint | Qt.Dialog | Qt.WindowTitleHint|Qt.WindowStaysOnTopHint
    //自动吸附功能
    onXChanged: {
        var windowCenterX=x+width/2;
        var windowCenterY=y+height/2;
        //位于屏幕左半边
        if(windowCenterX< Screen.width/4){
            x=0;
        }else if(windowCenterX> Screen.width/4*3){
            x=Screen.width-root.width
        }else{

        }

    }
   //检测鼠标，如果鼠标不在应用上，那么隐藏

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onExited: {
            console.log('Mouse out',root.window)
            //root.visible=false;
        }
    }


   //弹性动画
    Behavior on x {
        SmoothedAnimation{
            duration: 300
            velocity: 10
        }
    }
    //listView代理
    Component{
        id:listDelegate
        Item{
            id:wrapper
            width: listView.width
            signal curGroupChanged(var groupName);
            height:30
            //鼠标点击选中当前
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    wrapper.ListView.view.currentIndex=index
                    console.log("点击"+index,model.CusGroup)
                    //发送组改变信号
                    curGroupChanged(model.CusGroup);
                }
                onDoubleClicked: {
                    txtGroup.enabled=true;
                }
            }
           RowLayout{

                 anchors.fill: parent
                 TextField{
                    id:txtGroup
                    enabled: false
                    placeholderText: CusGroup
                    Layout.fillWidth: true
                    selectByMouse: true
                    //选中变色及变大

                    font.pixelSize:wrapper.ListView.isCurrentItem?20:16
                    color:wrapper.ListView.isCurrentItem?"firebrick":"black"
                    //设置大小
                    onEditingFinished:{
                        model.CusGroup=txtGroup.text
                        enabled=false
                    }

                 }
                }

           Connections{
               target:wrapper
               function onCurGroupChanged(groupName){
                   console.log("Group changed")
                   gridModel.group=groupName;
                   gridView.update();	//刷新界面
               }
           }

            }
        }
    //一个分割界面
    SplitView{
        orientation: Qt.Horizontal
        anchors.fill:parent
        visible: true

        Item{
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            SplitView.preferredWidth:parent.width/4
            ListView{
                id:listView
                visible: true
                anchors.left:parent.left
                anchors.right:parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                model:listModel
                delegate: listDelegate
                //高量
                highlight: Rectangle{
                    color: "lightsteelblue"
                }
                //右键菜单区域
                MouseArea{
                    anchors.fill: parent
                    acceptedButtons:Qt.RightButton
                    onClicked: {
                        if (mouse.button === Qt.RightButton)
                            groupMenu.open()
                    }
                    onPressAndHold: {
                        if (mouse.source === Qt.MouseEventNotSynthesized)
                            groupMenu.open()
                    }

                    Menu {
                        id: groupMenu
                        MenuItem {
                            text: "新建分组"
                            onTriggered: {
                                listModel.appendRow();
                                listView.update();
                            }

                        }
                        MenuItem { text: "删除分组" }
                    }
                }

                //动画效果
                add: Transition{
                    ParallelAnimation{
                        //变化透明度
                        NumberAnimation{
                            property: "opacity"
                            from:0
                            to:1
                            duration:500

                        }
                        SpringAnimation{
                            property: "y"
                            spring: 3
                            from:0
                            damping: 0.1
                            epsilon: 0.25

                        }

                    }

                }


            }
        }

        //分隔界面右边
        Item{
            SplitView.preferredWidth: parent.width*3/4
            GridView{
                id:gridView
                anchors.left:parent.left
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                model: gridModel
                delegate:DropArea{
                    id:gridDelegate
                    required property string appName;
                    required property url appIcon;
                    //required property color color;

                    property int visualIndex:gridView.currentIndex
                    width:gridView.cellWidth
                    height:gridView.cellWidth
                    onEntered: function(drag){
                    }
                    IconItem{
                        id:icon
                        parent:gridDelegate
                        name:appName
                        iconUrl:appIcon
                        dragPrarent:gridView
                        visualIndex:gridDelegate.visualIndex

                    }

                }
                cellWidth:100
                cellHeight:100
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                focus: true
                move: Transition {
                           NumberAnimation { properties: "x"; duration: 100; easing.type: Easing.OutCubic }
                           NumberAnimation { properties: "y"; duration: 100; easing.type: Easing.OutCubic }
                       }
                       moveDisplaced: Transition {
                           NumberAnimation { properties: "x"; duration: 300; easing.type: Easing.OutCubic}
                           NumberAnimation { properties: "y"; duration: 100;  easing.type: Easing.OutCubic }
                       }
               }
        }
    }





    //任务栏图标
    SystemTrayIcon{
        id:trayIcon
        visible: true
        iconSource: 'res/shark.ico'
        menu: Menu{
            MenuItem{
                text: '设置'
            }
            MenuItem{
                text:'退出'
                onTriggered: {
                    root.close();
                }
            }
        }

        onActivated: {
            root.visible=true;
            root.raise();
            root.requestActivate();
        }


    }

    //信号检测
    Connections{
        target:nativeEvent
        //如果鼠标位置发生变化
        function onPtChanged(point){
            //console.log("位置变化",point.x,point.y,root.visible);
            //如果位于屏幕最左或者最右边
            if(point.x <= 1||point.x>=1919)
            {
                //如果这时窗口处于隐藏状态
                if(root.visible=== false )
                {
                    //显示窗口
                    root.show()
                    root.raise()
                    root.requestActivate();
                }
            }
        }
    }
    Component.onCompleted: {
        var appGroups = Array(listModel.appGroups);
        var i=0;
        console.log(appGroups.length);
        for(i;i<appGroups.length+1;i++)
        {
        //console.log(listModel.appGroups[i]);
        }
    }


}

