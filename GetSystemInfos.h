#ifndef GETSYSTEMINFOS_H
#define GETSYSTEMINFOS_H
#include <QObject>
#include <QSettings>
#include <QVector>
#include <QStringList>

struct AppInfo
{
    QString appName;
    QString appPath;
    QString appIcon;
};

/**
 * @brief 这个类是用来获取系统的相关信息
 */
class GetSystemInfos : public QObject
{
    Q_OBJECT
  private:
    QVector<QString> linkFilesPath; // link文件的绝对路径
  public:
    explicit GetSystemInfos(QObject *parent = nullptr);
    ~GetSystemInfos();

    /**
     * @brief 注册表导出系统所有安装的app相关信息
     * @param info [out]
     */
    [[deprecated("use ")]] static void getInstalledApps(QVector<AppInfo> &infoVector);
    /**
     * @brief 通过路径C:\ProgramData\Microsoft\Windows\Start Menu\Programs获取信息
     * @param infoVector
     */
    void getAppsInfo(QVector<AppInfo> &infoVector);
    /**
     * @brief 递归获取目录下所有的link文件
     * @param path 路径
     * @param files 所有文件路径
     * @return 当前所有文件
     */
    void getLinkFile(QString path, QVector<QString> &files);

    const QVector<QString> &getLinkFilesPath() const;
    void setLinkFilesPath(const QVector<QString> &newLinkFilesPath);

  signals:
};

#endif // GETSYSTEMINFOS_H
