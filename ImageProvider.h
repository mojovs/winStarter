#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>

class ImageProvider : public QQuickImageProvider
{
  public:
    QImage m_img;
    ImageProvider();
    virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

  private:
};

#endif // IMAGEPROVIDER_H
