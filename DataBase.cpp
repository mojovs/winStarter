#include "DataBase.h"
#include <QDebug>
#include <QFile>
#include <QFileIconProvider>
#include <QSqlError>
#include <QBuffer>
#include <QDir>
#pragma execution_character_set("utf-8")
//构造
QSqlDatabase DataBase::db;
QSqlQuery *DataBase::query;
DataBase::DataBase(QObject *parent) : QObject{parent}
{
    //创建系统信息类
    systemInfos = new GetSystemInfos();
    //打开数据库
    //数据导入数据库
}

//析构
DataBase::~DataBase()
{
    delete systemInfos;
    delete query;
    if (db.isOpen())
    {
        qDebug() << "Close DB";
        db.close();
    }
    appInfoList.clear();
}
//打开数据库
bool DataBase::openDB()
{
    QString fileName = "data.db";

    if (fileName.isEmpty())
    {
        qDebug() << "FileName is null";
        return false;
    }

    //设置数据库类型
    db = QSqlDatabase::addDatabase("QSQLITE");
    //添加数据库
    db.setDatabaseName(fileName);
    //打开数据库
    if (!db.open())
    {
        qDebug() << "DB  ERROR:";
        return false;
    }
    if (db.isOpenError())
    {
        qDebug() << "OPEN DB ERROR";
        return false;
    }
    qDebug() << "Open DB";
    //设置query
    query = new QSqlQuery(db);
    //打开表
    openTable();
    return true;
}

/**
 * @brief 关闭数据库
 * @return
 */
void DataBase::closeDB()
{
    if (!db.isOpen())
    {
        db.close();
    }
}

/**
 * @brief 判断数据库打开了没
 * @return
 */
bool DataBase::isDBOpened()
{
    if (db.isOpen())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief 打开一长数据表
 * @return
 */
bool DataBase::openTable()
{
    //    qryModel = new QSqlQueryModel(this);
    //    //查询
    //    qryModel->setQuery("select * from icon");
    //
    //    qryModel->setHeaderData(0,Qt::Horizontal,"id");
    //    qryModel->setHeaderData(1,Qt::Horizontal,"group");
    //    qryModel->setHeaderData(2,Qt::Horizontal,"exePath");
    //    qryModel->setHeaderData(3,Qt::Horizontal,"icon");
    //
    //    if(qryModel->lastError().isValid())
    //    {
    //        qDebug()<<"qryModel Last Error"<<qryModel->lastError();
    //        return false;
    //    }

    return true;
}

/**
 * @brief 判断当前程序是否第一次启动
 * @return
 */
bool DataBase::checkFirstRun()
{
    QString curPath = QDir::currentPath(); //获取当前路径
    curPath += "/FIRST";
    QFileInfo info(curPath);
    //如果下面有first文件，说名不是第一次启动
    if (info.isFile())
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief 获取系统内所有安装的app信息
 * @return
 */
bool DataBase::getInstalledAppsInfo()
{
    //获取app信息
    systemInfos->getAppsInfo(appInfoList);
    return true;
}

/**
 * @brief DataBase::queryAppCount
 * @return app数量
 */
int DataBase::queryAppCount()
{
    query->exec("select count(*) from icon");
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    query->next();
    qDebug() << "******count is " << query->value(0).toInt();
    return query->value(0).toInt();
}
/**
 * @brief 根据group名查找总数
 * @param name
 * @return
 */
int DataBase::queryCountInGroup(QString name)
{
    query->prepare("select count(*) from icon where CusGroup=:cusGroup");
    query->bindValue(":cusGroup", name);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    query->next();
    qDebug() << "******count is " << query->value(0).toInt();
    return query->value(0).toInt();
}

/**
 * @brief 根据id查询组
 * @return
 */
void DataBase::queryGroup(int i, QString &group)
{
    query->prepare("select CusGroup from icon where Id=:id order by Id ");
    query->bindValue(":id", i);
    query->exec();

    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }

    //获取当前记录
    group = query->value(0).toString();
}

/**
 * @brief DataBase::setGroup
 * @param i
 * @param name
 */
void DataBase::setGroup(int i, QString name)
{
    query->prepare("update icon set CusGroup=:cusGroup where Id=:id ");

    query->bindValue(":cusGroup", name);
    query->bindValue(":id", i);
    query->exec();

    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
}

/**
 * @brief DataBase::addGroup
 * @param name
 */
void DataBase::appendGroup(QString name)
{
    query->prepare("insert into icon (CusGroup) values(:cusGroup) ");

    query->bindValue(":cusGroup", name);
    query->exec();

    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
}

/**
 * @brief 删除该组
 * @param name
 */
void DataBase::deleteGroup(QString name)
{
    query->prepare("delete from icon where CusGroup=?cusGroup");
    query->bindValue(":cusGroup", name);
    query->exec();

    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
}

/**
 * @brief DataBase::setGroups
 * @param oldName
 * @param newName
 */
void DataBase::setGroups(QString oldName, QString newName)
{
    query->prepare("update icon set CusGroup=:newName where CusGroup=:oldName ");

    query->bindValue(":oldName", oldName);
    query->bindValue(":newName", newName);
    query->exec();

    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
}

void DataBase::queryGroups(QStringList &group)
{
    query->exec("select distinct CusGroup from icon ");
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    while (query->next())
    {
        group.append(query->value(0).toString());
    }
}

/**
 * @brief 根据id查找app名称
 */
void DataBase::queryAppName(int i, QString &name)
{
    query->clear();
    query->prepare("select Name from icon where Id=:id order by Id");
    query->bindValue(":id", i);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    query->next();
    //获取当前记录
    name = query->value(0).toString();
}

void DataBase::queryNamesInGroup(QString name, QStringList &list)
{
    query->prepare("select Name from icon where CusGroup=:cusGroup order by Id");
    query->bindValue(":cusGroup", name);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    list.clear();
    while (query->next())
    {
        list.append(query->value(0).toString());
    }
}

/**
 * @brief 查询app路径
 */
void DataBase::queryAppPath(int i, QString &path)
{
    query->clear();
    query->prepare("select ExePath from icon where Id=:id order by Id");
    query->bindValue(":id", i);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    query->next();
    //获取当前记录
    path = query->value(0).toString();
}

void DataBase::queryPathsInGroup(QString name, QStringList &list)
{
    query->prepare("select ExePath from icon where CusGroup=:cusGroup order by Id");
    query->bindValue(":cusGroup", name);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    list.clear();
    while (query->next())
    {
        list.append(query->value(0).toString());
    }
}

void DataBase::queryAppIcon(const QString name, QByteArray &data)
{
    query->prepare("select Icon from icon where Name=:name  order by Id");
    query->bindValue(":name", name);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }

    //获取当前记录
    data = query->value(0).toByteArray();
}

/**
 * @brief 查询app图标数据
 * @param i id
 * @param data[out]
 */
void DataBase::queryAppIcon(const int i, QByteArray &data)
{
    query->prepare("select Icon from icon where Id=:id order by Id");
    query->bindValue(":id", i);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }

    //获取当前记录
    query->next();

    data = query->value(0).toByteArray();
}

void DataBase::queryIconsInGroup(QString name, QStringList &list)
{
    query->prepare("select Icon from icon where CusGroup=:cusGroup order by Id");
    query->bindValue(":cusGroup", name);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
    list.clear();
    while (query->next())
    {
        //构造url
        QString img("data:image/png;base64,");
        img.append(query->value(0).toByteArray().toBase64().data());
        list.append(img);
    }
}

/**
 * @brief DataBase::queryMove
 * @param id
 * @param newId
 */
void DataBase::queryMove(int id, int newId)
{
    query->prepare("update icon set Id=(case when Id=:id then :newid else :id end) where Id in (:id,:newid)");
    query->bindValue(":id", id);
    query->bindValue(":newid", newId);
    query->exec();
    if (query->lastError().isValid())
    {
        qDebug() << "query error" << query->lastError().text() << __FILE__ << " " << __LINE__;
    }
}

/**
 * @brief 对数据库的数据进行最基本的设置
 * 1. 导入应用数据到数据库
 */
void DataBase::reset()
{
    //清空数据库icon表所有内容
    qDebug() << "reset data";
    query->exec("delete from icon");
    if (query->lastError().isValid())
    {
        qDebug() << "query erro:" << query->lastError().text();
    }
    getInstalledAppsInfo(); //获取所有的app
    //循环导入数据库里
    for (int i = 0; i < appInfoList.count(); i++)
    {
        const AppInfo *info = &appInfoList.at(i);
        QByteArray picData;
        getIcon(info->appPath, picData); //获取icon

        query->prepare(tr("insert into icon values(:id,:group,:name,:path,:icon)"));
        query->bindValue(":id", i);
        query->bindValue(":group", tr("默认"));
        query->bindValue(":name", info->appName);
        query->bindValue(":path", info->appPath);
        query->bindValue(":icon", picData);
        query->exec();
        if (query->lastError().isValid())
        {
            qDebug() << "query erro:" << query->lastError().text() << __FILE__ << __LINE__;
        }
        picData.clear(); //清空
    }
}

/**
 * @brief 根据路径获取图片
 * @param path icon路径
 */
void DataBase::getIcon(QString path, QByteArray &picData)
{
    // link文件
    QFileInfo lnkinfo(path);
    if (!lnkinfo.isFile())
    {
        qDebug() << "This is not a file:" << path;
        return;
    }
    //打开icon提供器
    QFileIconProvider iconProvider;
    QIcon icon;
    // link目标文件
    QString targFile = lnkinfo.symLinkTarget();
    QFileInfo info(targFile);
    if (!info.isFile())
    {
        qDebug() << "This is not a file:" << targFile;
        //这是只是一个快捷方式，那么
        icon = iconProvider.icon(lnkinfo);
    }

    icon           = iconProvider.icon(info);
    QPixmap pixmap = icon.pixmap(32, 32);
    //写入到字节流里
    QBuffer buff(&picData); //为数据创建io设备
    buff.open(QIODevice::WriteOnly);
    //以png格式保存
    pixmap.save(&buff, "PNG");
}
