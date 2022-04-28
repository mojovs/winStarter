#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWindow>
#include <QQmlContext>
#include "MyEventFilter.h"
#include "GroupListmodel.h"
#include "AppModel.h"

int main(int argc, char *argv[])
{
    //设置中文
    QTextCodec *txtCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(txtCodec);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    //导入类
    //本地消息过滤器
    // MyEventFilter nativeEventFilter;
    QQmlContext *ctx = engine.rootContext();

    // ctx->setContextProperty("nativeEvent", &nativeEventFilter);
    ctx->setContextProperty("listModel", new GroupListModel());
    ctx->setContextProperty("gridModel", new AppModel());

    //导入到引擎
    engine.load(url);

    //获取winid
    WId wid;
    QWindow *rootWin;
    QObject *m_rootObject = engine.rootObjects().first();
    if (m_rootObject)
    {
        rootWin = qobject_cast<QWindow *>(m_rootObject);
        if (rootWin) wid = rootWin->winId();
    }

    qDebug() << "MAIN wid:" << wid;
    // nativeEventFilter.setWId(wid); //给事件设置wid

    //加载

    // app.installNativeEventFilter(&nativeEventFilter);
    return app.exec();
}
