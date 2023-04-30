QT       += core gui webenginewidgets quick \
    widgets
QT += quickwidgets webengine

TEMPLATE = lib
DEFINES += VIDEOCHAT_LIBRARY
TARGET = video
CONFIG += c++20

QTQUICK_COMPILER_SKIPPED_RESOURCES += html.qrc # prevent Qt Quick from generating c++ code for js

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    mainvideochatwindow.cpp \
    utils.cpp


HEADERS += \
    mainvideochatwindow.h \
    utils.h \
     video-chat_global.h


RESOURCES += \
    html.qrc \
    images.qrc \
    qml.qrc

FORMS += \
    mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Bridge-Utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Bridge-Utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../Bridge-Utils/ -lutils

INCLUDEPATH += $$PWD/../Bridge-Utils
DEPENDPATH += $$PWD/../Bridge-Utils
