#include "hook.h"
#include <QDebug>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <QLibrary>

typedef int(*FUN1)(HWND);
typedef int(*FUN2)();
#endif

WId Hook::getWId() const
{
    return wId;
}

void Hook::setWId(WId newWId)
{
    if (wId == newWId)
        return;
    wId = newWId;
    emit wIdChanged();
}

Hook::Hook(QObject *parent)
    : QObject{parent}
{
    //创建的时候设置hook

}

Hook::~Hook()
{

}



bool Hook::setGlobalHook(bool b)
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
