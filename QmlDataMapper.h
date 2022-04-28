#ifndef QMLDATAMAPPER_H
#define QMLDATAMAPPER_H

#include <QObject>
#include <QAbstractItemModel>
class QmlDataMapperPrivate;
class QmlDataMapper : public QObject
{
    Q_OBJECT
public:
    explicit QmlDataMapper(QObject *parent = nullptr);
    ~QmlDataMapper();

    void clearMapping();	//清除映射
    void removeMapping(QObject *object);	//移除单个映射
    int currentIndex() const;
    int count() const;
    //映射属性名
    QByteArray mappedPropertyName(QObject *object) const;
     int mappedSection(QObject *object) const;
     QObject * mappedControlAt(const int &section) const;
     QAbstractItemModel * model() const;

     void setModel(QAbstractItemModel *model);
public Q_SLOTS:

     void addMapping(QObject *object, const int &section);

     void addMapping(QObject *object, const int &section, const QByteArray &propertyName);
     void revert();
     void setCurrentIndex(int index);
     void toFirst();
     void toLast();
     void toNext();
     void toPrevious();
     void updateData(int index);

 Q_SIGNALS:
     void currentIndexChanged(int index);
     void countChanged();

 private:
     QmlDataMapperPrivate * const d;
signals:

};

#endif // QMLDATAMAPPER_H
