#ifndef CURSORPOINT_H
#define CURSORPOINT_H

#include <QObject>
//这个类是用来监控鼠标位置并导入到
class CursorPoint : public QObject
{
    Q_OBJECT
public:
    explicit CursorPoint(QObject *parent = nullptr);


signals:

};

#endif // CURSORPOINT_H
