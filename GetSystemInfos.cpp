#include "GetSystemInfos.h"
#include <QDir>

#include <QDebug>
#pragma execution_character_set("utf-8")

const QVector<QString> &GetSystemInfos::getLinkFilesPath() const
{
    return linkFilesPath;
}

void GetSystemInfos::setLinkFilesPath(const QVector<QString> &newLinkFilesPath)
{
    linkFilesPath = newLinkFilesPath;
}

/**
 * @brief 该类的构造函数
 * @param parent
 */

GetSystemInfos::GetSystemInfos(QObject *parent) : QObject{parent} {}
/**
 * @brief 析构
 */
GetSystemInfos::~GetSystemInfos()
{
    if (linkFilesPath.count() > 0)
    {
        linkFilesPath.clear();
    }
}

/**
 * @brief 从注册表导出系统所有安装的app相关信息
 * @param info [out]
 * HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Persisted
 */
void GetSystemInfos::getInstalledApps(QVector<AppInfo> &infoVector)
{
    QString regName = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
    QSettings settings(regName, QSettings::NativeFormat); //打开注册表

    QStringList regGroups = settings.childGroups();
    //循环读取各个程序组内 key value
    foreach (QString regItem, regGroups)
    {
        AppInfo appInfo;
        settings.beginGroup(regItem);
        //开始获取文件名
        appInfo.appName = settings.value("DisplayName").toString();
        appInfo.appPath = settings.value("InstallLocation").toString();
        //获取文件icon
        appInfo.appIcon = settings.value("InstallLocation").toString();
        //结束读取组
        settings.endGroup();
        //导出
        infoVector.append(appInfo);
    }
}

/**
 * @brief 通过路径C:\ProgramData\Microsoft\Windows\Start Menu\Programs获取信息
 * @param infoVector[out]
 */
void GetSystemInfos::getAppsInfo(QVector<AppInfo> &infoVector)
{
    //清空数据
    if (!linkFilesPath.isEmpty())
    {
        linkFilesPath.clear();
    }
    //打开路径C:\ProgramData\Microsoft\Windows\Start Menu\Programs
    QString rootPath = "C:/ProgramData/Microsoft/Windows/Start Menu/Programs";
    //获取根路径信息
    QFileInfo rootInfo(rootPath);
    if (!rootInfo.isDir())
    {
        qDebug() << rootPath << " is not a Dir" << __FILE__ << __LINE__;
        return;
    }
    //获取该路径下所有link文件
    getLinkFile(rootPath, linkFilesPath);
    //添加文件信息列表到
    foreach (QString path, linkFilesPath)
    {
        if (path.isEmpty())
        {
            qDebug() << "empty path occured:" << __FILE__ << __LINE__;
            continue;
        }
        //构造信息结构体
        AppInfo appInfo;
        QFileInfo fileInfo(path);
        appInfo.appName = fileInfo.baseName();
        appInfo.appPath = path;
        appInfo.appIcon = path;
        //加入到信息数组里
        infoVector.append(appInfo);
    }
}

/**
 * @brief 递归获取目录下所有的link文件
 * @param path 路径
 * @return 当前所有文件
 */
void GetSystemInfos::getLinkFile(QString path, QVector<QString> &files)
{
    if (path.isEmpty())
    {
        qDebug() << "path is null";
        return;
    }
    QDir aDir(path); //新建路径
    //循环处理
    //获取文件夹
    foreach (QFileInfo fileInfo, aDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files))
    {
        //如果是路径的？话
        if (fileInfo.isDir())
        {
            // qDebug() << "Dirs:" << fileInfo.filePath();
            getLinkFile(fileInfo.absoluteFilePath(), files);
        }
        //如果是文件的话
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            // qDebug() << "files:" << fileInfo.filePath();
            files.append(fileInfo.absoluteFilePath());
        }
    }
}
