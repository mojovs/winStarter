#ifndef HOOK_H
#define HOOK_H

#include <QObject>
#include <Windows.h>
#include <QWindow>

class Hook : public QObject
{
    Q_OBJECT
private:
   WId wId;

   Q_PROPERTY(WId wId READ getWId WRITE setWId NOTIFY wIdChanged)

public:
    explicit Hook(QObject *parent = nullptr);
    ~Hook();


   bool setGlobalHook(bool b);
   //获取id
   WId getWId() const;
   void setWId(WId newWId);

signals:
   void mousekeyboardAction(WPARAM wParam, LPARAM lParam);
   void wIdChanged();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
};

#endif // HOOK_H
