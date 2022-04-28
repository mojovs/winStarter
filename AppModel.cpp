#include "AppModel.h"
#include <QDebug>

#pragma execution_character_set("utf-8")
/**
 * @brief 构造
 * @param parent
 */
AppModel::AppModel(QObject *parent) : QAbstractListModel{parent}
{
    // imgPro = new ImageProvider();
    //如果db没有打开
    if (!db.isDBOpened())
    {
        db.openDB();
    }
    //设置数据
    m_group = "默认";
    setAppData();
    //函数绑定
    connect(this, SIGNAL(groupChanged()), this, SLOT(onGroupChanged())); //组改变时
}

/**
 * @brief 析造
 */
AppModel::~AppModel()
{
    // delete imgPro;
    db.closeDB();
}

/**
 * @brief 显示界面数据
 * @param index
 * @param role
 * @return
 */
QVariant AppModel::data(const QModelIndex &index, int role) const
{
    //设置显示内容
    if (!index.isValid())
    {
        return QVariant();
    }
    //当前行大于 总数
    if (index.row() > m_count)
    {
        return QVariant();
    }
    int i = index.row();
    //查询数据表里，得出分组
    if (role == RoleName)
    {
        return appNames.at(i);
    }
    else if (role == RoleLocation)
    {
        return appPaths.at(i);
    }
    else if (role == RoleIcon)
    {
        return appIcons.at(i);
    }
    else
    {
        return QVariant();
    }
}

bool AppModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return true;
}

bool AppModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}

int AppModel::rowCount(const QModelIndex &parent) const
{
    return m_count;
}

QHash<int, QByteArray> AppModel::roleNames() const
{
    QHash<int, QByteArray> name;
    name[RoleName]     = "appName";
    name[RoleLocation] = "appPath";
    name[RoleIcon]     = "appIcon";
    return name;
}

/**
 * @brief 设置应用程序数据
 */
void AppModel::setAppData()
{
    //先获取数据库行数
    beginResetModel();
    m_count = db.queryCountInGroup(m_group);
    db.queryNamesInGroup(m_group, appNames);
    db.queryPathsInGroup(m_group, appPaths);
    db.queryIconsInGroup(m_group, appIcons);
    endResetModel();
}

void AppModel::move(int index, int newIndex)
{
    beginResetModel();
    //开始调换数据
    db.queryMove(index, newIndex); //执行
    appNames.swap(index, newIndex);
    appPaths.swap(index, newIndex);
    appIcons.swap(index, newIndex);
    endResetModel();
}

const QString &AppModel::group() const
{
    return m_group;
}

void AppModel::setGroup(const QString &newGroup)
{
    if (m_group == newGroup) return;
    m_group = newGroup;
    emit groupChanged();
}

/**
 * @brief 槽
 */
void AppModel::onGroupChanged()
{
    //重新设置下数据
    setAppData();
}
