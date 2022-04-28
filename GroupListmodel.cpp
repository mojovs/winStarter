#include "GroupListmodel.h"
#include <QFileDialog>
#include <stdio.h>
#include <string.h>
#include <QFileIconProvider>
#include <QPixmap>
#pragma execution_character_set("utf-8")

/**
 * @brief 构造函数
 * @param parent
 */
GroupListModel::GroupListModel(QObject *parent) : QAbstractListModel{parent}
{
    //打开数据库
    db.openDB();
    //设置数据库数据
    if (DataBase::checkFirstRun())
    {
        db.reset();
    }
    //添加一个默认的
    //查询出组别
    beginResetModel();
    setAppData();
    endResetModel();
}

/**
 * @brief 析构
 */
GroupListModel::~GroupListModel() {}
//获取到数据 ,显示
QVariant GroupListModel::data(const QModelIndex &index, int role) const
{
    //设置显示内容
    if (!index.isValid())
    {
        return QVariant();
    }
    //当前行大于 总数
    if (index.row() > appGroups.count())
    {
        return QVariant();
    }
    int i = index.row();
    //查询数据表里，得出分组
    if (role == RoleGroup)
    {
        qDebug() << "Model data:" << appGroups.at(i);
        return appGroups.at(i);
    }
    else
    {
        return QVariant();
    }
}

bool GroupListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //开始插入
    beginInsertRows(parent, row, row + count);
    //先导入数据
    for (int i = 0; i < count; i++)
    {
        //导入数据库
        QString groupName("新建分组");
        db.appendGroup(groupName);
        appGroups.append(groupName);
        qDebug() << appGroups;
    }

    endInsertRows();

    return true;
}

bool GroupListModel::appendRow()
{
    //开始插入
    QString groupName("新建分组");

    if (!appGroups.contains(groupName))
    {
        beginResetModel();
        db.appendGroup(groupName);
        appGroups.append(groupName);
        qDebug() << appGroups;
        endResetModel();
    }
    return true;
}

bool GroupListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && RoleGroup == role)
    {
        if (appGroups[index.row()] != value.toString())
        {
            //更新数据库
            db.setGroups(appGroups[index.row()], value.toString());
            appGroups[index.row()] = value.toString();
            emit dataChanged(index, index, {RoleGroup});
            return true;
        }
    }
    return false;
}

int GroupListModel::rowCount(const QModelIndex &parent) const
{
    return appGroups.count();
}

QHash<int, QByteArray> GroupListModel::roleNames() const
{
    QHash<int, QByteArray> name;
    name[RoleGroup] = "CusGroup";
    return name;
}

/**
 * @brief 设置内部数据
 */
void GroupListModel::setAppData()
{
    db.queryGroups(appGroups);
}
