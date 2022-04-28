#ifndef GROUPLISTMODEL_H
#define GROUPLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QVector>
#include "DataBase.h"
//用来统计分组
class GroupListModel : public QAbstractListModel
{
    Q_OBJECT
    enum RoleType
    {
        RoleGroup    = Qt::UserRole + 1,
        RoleName     = Qt::UserRole + 2,
        RoleLocation = Qt::UserRole + 3,
        RoleIcon     = Qt::UserRole + 4,

    };

  private:
    DataBase db;           //数据库类
    QStringList appGroups; // app分组

  public:
    explicit GroupListModel(QObject *parent = nullptr);
    ~GroupListModel();

    /*--模型构造部分--*/
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE bool appendRow(); //添加一个组

  private:
    void setAppData(); //从数据库里拿出相关数据

  signals:
    // void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
};

#endif // GROUPLISTMODEL_H
