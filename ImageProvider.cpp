#include "ImageProvider.h"
ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(this->m_img);
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return this->m_img;
}
