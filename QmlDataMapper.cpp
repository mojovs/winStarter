#include "QmlDataMapper.h"
#include <QPointer>
#include <QVector>
struct Mapping {
    QPointer<QObject> object;
    int section;
    QByteArray propertyName;
};

class QmlDataMapperPrivate{
public:
    QmlDataMapperPrivate():m_model(0),m_currentIndex(-1)
    {
    }

    void updateMapping(Mapping &mapping,QObject *object,const int &setction,const QByteArray &propertyName);
    void update();	//更新模型中的数据到object属性
    QAbstractItemModel *m_model;
    QVector<Mapping> m_mappings;
    int m_currentIndex;
};

/**-------------------私有类部分 --------------------------**/
void QmlDataMapperPrivate::updateMapping(Mapping &mapping, QObject *object, const int &section, const QByteArray &propertyName)
{
    mapping.object = object;
    mapping.section =section ;
    mapping.propertyName = propertyName.isEmpty()?"text":propertyName;
}

void QmlDataMapperPrivate::update()
{
    if(!m_model)
        return;
    if(m_mappings.isEmpty())
        return;
    if(m_currentIndex ==-1)
        return;
    //重新设置所有的映射
    foreach(const Mapping &mapping,m_mappings)
    {
        if(mapping.object)
        {
            //设置属性
            mapping.object->setProperty(mapping.propertyName,m_model->data(m_model->index(m_currentIndex,0),mapping.section));
        }
    }
}

/**------------------ 主类部分 --------------------------**/

/**
 * @brief 构造 函数
 * @param parent
 */
QmlDataMapper::QmlDataMapper(QObject *parent):QObject(parent),d(new QmlDataMapperPrivate())
{

}
/**
 * @brief 析构
 */
QmlDataMapper::~QmlDataMapper()
{
    delete d;
}
/**
 * @brief 清除map
 */
void QmlDataMapper::clearMapping()
{
    //清楚列表
    d->m_mappings.clear();
}
/**
 * @brief 当前index
 * @return
 */
int QmlDataMapper::currentIndex() const
{
    return d->m_currentIndex;
}
/**
 * @brief 当前映射数量
 * @return
 */
int QmlDataMapper::count() const
{
    if(!d->m_model)
    {
        return 0;
    }
    return d->m_model->rowCount();	//当前模型的列数
}

/**
 * @brief 找到已映射的属性名
 * @param object
 * @return 返回该对象的映射属性
 */
QByteArray QmlDataMapper::mappedPropertyName(QObject *object) const
{
    foreach(const Mapping &mapping,d->m_mappings )
    {
        if(mapping.object == object)
            return mapping.propertyName;
    }
    //如果每找到返回空
    return QByteArray();
}

/**
 * @brief 获取已映射对象的role
 * @param object
 * @return
 */
int QmlDataMapper::mappedSection(QObject *object) const
{
    foreach(const Mapping &mapping,d->m_mappings )
    {
        if(mapping.object == object)
            return mapping.section;
    }
    //如果每找到返回空
    return 0;
}

/**
 * @brief 获取映射的object
 * @param section
 * @return
 */
QObject *QmlDataMapper::mappedControlAt(const int &section) const
{

    foreach(const Mapping &mapping,d->m_mappings )
    {
        if(mapping.section==section)
            return mapping.object;
    }
    //如果每找到返回空
    return 0;
}

/**
 * @brief 返回模型
 * @return
 */
QAbstractItemModel *QmlDataMapper::model() const
{
    return d->m_model;
}

/**
 * @brief 为映射设置数据模型
 * @param model
 */
void QmlDataMapper::setModel(QAbstractItemModel *model)
{
    d->m_model=model;
    d->m_currentIndex = 0;
    d->update();	//更新数据，重新钩建数据关系
    emit countChanged();	//发送信号

}

/**
 * @brief 给对象添加映射
 * @param object 对象
 * @param section 对象role
 */
void QmlDataMapper::addMapping(QObject *object, const int &section)
{
    addMapping(object,section,"text");
}

/**
 * @brief 对象添加映射
 * @param object 对象
 * @param section role
 * @param propertyName 属性名
 */
void QmlDataMapper::addMapping(QObject *object, const int &section, const QByteArray &propertyName)
{
    //在映射列表里查找映射
    for(int i =0;i<d->m_mappings.count();i++)
    {

        Mapping &mapping =d->m_mappings[i];
        //更新映射
        //在映射列表里找到对象
        if(mapping.object == object)
        {

        d->updateMapping(mapping,object,section,propertyName);
        //更新映射到对象
        d->update();
        return;
        }
    }

    //添加一个新的映射
    Mapping mapping;
    d->updateMapping(mapping,object,section,propertyName);
    d->m_mappings.append(mapping);
    d->update();
}

/**
 * @brief  恢复
 */
void QmlDataMapper::revert()
{
    d->update();
}

/**
 * @brief 设置前前的index
 * @param index
 */
void QmlDataMapper::setCurrentIndex(int index)
{
    if(!d->m_model)
        return;
    const int rowCount = d->m_model->rowCount();
    if(index<0 ||index>=rowCount)
        return;
    d->m_currentIndex = index;
    d->update();	//更新映射
    emit currentIndexChanged(index);
}

void QmlDataMapper::toFirst()
{
    setCurrentIndex(0);
}

void QmlDataMapper::toLast()
{

    if(!d->m_model)
        return;
    const int rowCount = d->m_model->rowCount();
    setCurrentIndex(rowCount-1);
}

void QmlDataMapper::toNext()
{

    setCurrentIndex(d->m_currentIndex+1);
}

void QmlDataMapper::toPrevious()
{

    setCurrentIndex(d->m_currentIndex-1);
}

/**
 * @brief 更新该index下的数据
 * @param index
 */
void QmlDataMapper::updateData(int index)
{
    d->m_currentIndex = index;
    d->update();
    emit countChanged();
}



