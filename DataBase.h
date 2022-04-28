#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "GetSystemInfos.h"
#include <QSqlQuery>
class DataBase : public QObject
{
    Q_OBJECT
  private:
    static QSqlDatabase db; //数据库
    QVector<AppInfo> appInfoList;
    GetSystemInfos *systemInfos;
    static QSqlQuery *query;

  public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();
    //打开数据库
    bool openDB();
    void closeDB();
    bool isDBOpened();
    bool openTable();            //打开表
    static bool checkFirstRun(); //检查是否第一次运行
    /**
     * @brief 对数据库的数据进行最基本的设置
     * 1. 导入应用数据到数据库
     */
    void reset();
    /**
     * @brief 根据路径获取图片
     * @param path icon路径
     */
    void getIcon(QString path, QByteArray &picData);
    //元对象，组别
    const QStringList &getAppGroups() const;
    void setAppGroups(const QStringList &newAppGroups);

    Q_INVOKABLE bool getInstalledAppsInfo();
    /*--数据库控制部分--*/
    int queryAppCount();                 //查询总app数量
    int queryCountInGroup(QString name); //根据group查询

    void queryGroup(int i, QString &group);           //查询组别
    void setGroup(int i, QString name);               //设置该记录的group
    void setGroups(QString oldName, QString newName); //将整个组别改名
    void appendGroup(QString name);                   //添加一个组
    void deleteGroup(QString name);                   //删除该组
    void queryGroups(QStringList &group);             //查询出所有组别

    void queryAppName(int i, QString &name);                 //查询app名称
    void queryNamesInGroup(QString name, QStringList &list); //一次性查询出组内所有

    void queryAppPath(int i, QString &path);                 //查询app路径
    void queryPathsInGroup(QString name, QStringList &list); //一次性查询出组内所有

    void queryAppIcon(const QString name, QByteArray &data); //查询app的图标
    void queryAppIcon(const int i, QByteArray &data);        //查询app的图标数据

    void queryIconsInGroup(QString name, QStringList &list); //一次性查询出组内所有

    void queryMove(int id, int newId); //移动id
  signals:
};

#endif // DATABASE_H
