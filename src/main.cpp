#include <QQmlApplicationEngine>
#include <QtWidgets/QApplication>

#include "declarativewordsmodel.h"

static QObject *model_factory(QQmlEngine *, QJSEngine *)
{
    return new DeclarativeWordsModel;
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    qmlRegisterSingletonType<DeclarativeWordsModel>("Top.Words", 1, 0, "WordsModel", model_factory);
    engine.load(url);

    return app.exec();


}
