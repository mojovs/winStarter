#include "MyEventFilter.h"
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <QLibrary>
typedef int(*FUN1)(HWND);
typedef int(*FUN2)();
#endif
//构造1
MyEventFilter::MyEventFilter(QObject *parent):QObject(parent)
{

    qDebug()<<"wid is"<<this->wId;
    //函数绑定，获取鼠标位置
    connect(this,SIGNAL(mousekeyboardAction(WPARAM,LPARAM)),this,SLOT(on_recvHookMsg(WPARAM,LPARAM)));
}

//构造2
MyEventFilter::MyEventFilter( WId wid):wId(wid)
{

    qDebug()<<"wid is"<<wid;
    //函数绑定，获取鼠标位置
    connect(this,SIGNAL(mousekeyboardAction(WPARAM,LPARAM)),this,SLOT(on_recvHookMsg(WPARAM,LPARAM)));


}
//析构
MyEventFilter::~MyEventFilter()
{
    qDebug() << (setGlobalHook(false) ? "unset ok" : "unset failed");

}
//
bool MyEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        switch (msg->message) {

        case WM_USER + 100:{
            emit mousekeyboardAction(msg->wParam, msg->lParam);
            //qDebug() << "mouse_keyboard_action";
            break;
        }
        default:
            break;
        }
    }
#endif
    return false;
}
WId MyEventFilter::getWId() const
{
    return wId;
}

void MyEventFilter::setWId(WId newWId)
{
    if (wId == newWId)
        return;
    wId = newWId;

    //设置全局钩子
    qDebug() << (setGlobalHook(true) ? "set ok" : "set failed");
    emit wIdChanged();
}

QPoint MyEventFilter::getPt() const
{
    return pt;
}
//设置上一次鼠标点击的位置
void MyEventFilter::setPt(QPoint newPt)
{
    if (pt == newPt)
        return;
    pt = newPt;
    //发送信号
    emit ptChanged(pt);
}

//检查鼠标在边缘不
void MyEventFilter::checkMsIsEdge()
{

}

//槽
void MyEventFilter::on_recvHookMsg(WPARAM wParam, LPARAM lParam)
{
    //目前只检测鼠标信息
    MOUSEHOOKSTRUCT *msStruct=(MOUSEHOOKSTRUCT*)lParam;
    POINT point=msStruct->pt;
    QString debug = QString::number(point.x)+" "+QString::number(point.y)+"\n";
    //构建pint
    QPoint p(point.x,point.y);
    //鼠标位置传递出去
    this->setPt(p);	//设置p

    //qDebug() << debug;
    if(wParam ==VK_F2)
    {
        qDebug()<<"HookExit";
    }
}
//设置全局的钩子，导入dll
bool MyEventFilter::setGlobalHook(bool b)
{
#ifdef Q_OS_WIN
    static FUN1 SetHook = NULL;
    static FUN2 UnSetHook = NULL;
    if (b) {
        QLibrary lib("QHook.dll");
        if (lib.load()) {
            qDebug() << "load ok!";

            SetHook = (FUN1)lib.resolve("SetHook");
            UnSetHook = (FUN2)lib.resolve("UnSetHook");
            if (SetHook) {
                qDebug() << "load SetHook ok!";
                if(this->wId == NULL)
                {
                    qDebug()<<"No wID return";
                    return false;
                }
                SetHook((HWND)this->getWId());
            }
            if (UnSetHook) {
                qDebug() << "load UnSetHook ok!";
            }
            return true;
        } else {
            qDebug() << "load error!";
            return false;
        }
    } else {
        if (UnSetHook)
            return UnSetHook();
        else
            return false;
    }
#endif
}
