#ifndef MYEVENTFILTER_H
#define MYEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>

#include <Windows.h>
#include <QWindow>
class MyEventFilter :public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT


private:

    WId wId;	//窗口id
    QPoint pt;	//鼠标坐标

    bool setGlobalHook(bool b);		//开始安装/卸载钩子


    Q_PROPERTY(WId wId READ getWId WRITE setWId NOTIFY wIdChanged)
    Q_PROPERTY(QPoint pt READ getPt WRITE setPt NOTIFY ptChanged)


public:
    MyEventFilter(QObject *parent=0);		//基本构造
    MyEventFilter(WId wid);		//利用wid构造

    ~MyEventFilter();		//析构
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);	//本地事件接受

    //设置wid的部分
    WId getWId() const;
    void setWId(WId newWId);

    //设置坐标位置
    QPoint getPt() const;
    void setPt(QPoint newPt);

    //持续检测鼠标位置
    Q_INVOKABLE void checkMsIsEdge();

signals:
    void mousekeyboardAction(WPARAM wParam, LPARAM lParam);
    void wIdChanged();
    void ptChanged(QPoint point);
public slots:
    //槽，接受hook消息
    void on_recvHookMsg(WPARAM wParam,LPARAM lParam);

};

#endif // MYEVENTFILTER_H
