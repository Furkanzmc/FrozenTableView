#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "TableModel.h"
#include "ModelSlice.h"

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<TableModel>("Table", 1, 0, "TableModel");
    qmlRegisterType<ModelSlice>("Table", 1, 0, "ModelSlice");
    qmlRegisterType<HeaderModelSlice>("Table", 1, 0, "HeaderModelSlice");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreated,
      &app,
      [url](QObject* obj, const QUrl& objUrl) {
          if (!obj && url == objUrl)
              QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
