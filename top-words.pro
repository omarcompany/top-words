TEMPLATE = app

QT += quick concurrent core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        src/declarativewordsmodel.cpp \
        src/main.cpp \
        src/wordsengine.cpp

RESOURCES += qml/qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/declarativewordsmodel.h \
    src/wordsengine.h

INCLUDEPATH += src
