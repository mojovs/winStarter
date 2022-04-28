#ifndef APPMODEL_H
#define APPMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "DataBase.h"
#include "ImageProvider.h"

/**
 * @brief 界面右侧网格界面，显示安装的软件图标和名称，点击可以运行
 */
class AppModel : public QAbstractListModel
{
    Q_OBJECT
    enum RoleType
    {
        RoleGroup    = Qt::UserRole + 1,
        RoleName     = Qt::UserRole + 2,
        RoleLocation = Qt::UserRole + 3,
        RoleIcon     = Qt::UserRole + 4,

    };

  public:
    explicit AppModel(QObject *parent = nullptr);
    ~AppModel();

    /*--模型构造部分--*/
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;              //界面数据
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;         //插入行
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override; //设置数据
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;                          //行数
    QHash<int, QByteArray> roleNames() const;

    void setAppData();                              //设置应用程序数据
    Q_INVOKABLE void move(int index, int newIndex); //将当前的数据移动
    /*元对象s/ger*/
    const QString &group() const;
    void setGroup(const QString &newGroup);

  signals:
    void groupChanged();
  public slots:
    void onGroupChanged();

  private:
    DataBase db;          //数据库类
    QSqlQuery qry;        //查询类
    QString m_group;      //组
    int m_count;          //有多少个应用
    QStringList appNames; // app名称列表
    QStringList appPaths; // app的路径
    QStringList appIcons; // app图片url
    Q_PROPERTY(QString group READ group WRITE setGroup NOTIFY groupChanged)
};

#endif // APPMODEL_H
