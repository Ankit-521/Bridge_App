######################################################################
# Automatically generated by qmake (2.01a) Mon Sep 26 15:52:47 2011
######################################################################

QT       += gui core network quick xml sql webengine quickwidgets networkauth

TARGET = bridge
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION,4): QT+= widgets



HEADERS += talker.h \
    chatmanager.h \
    logindialog.h \
    networkutils.h \
    infodialog.h \
    xmlengine.h \
    gameengine.h

SOURCES += main.cpp \
    chatmanager.cpp \
    logindialog.cpp \
    talker.cpp \
    networkutils.cpp \
    infodialog.cpp \
    xmlengine.cpp \
    gameengine.cpp

RESOURCES += \
   html.qrc \
   qml_resources.qrc
FORMS += \
    infodialog.ui \
    logindialog.ui

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml

DISTFILES += \
    android-resources/AndroidManifest.xml


QTQUICK_COMPILER_SKIPPED_RESOURCES += html.qrc # prevent Qt Quick from generating c++ code for js




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bridge-login/release/ -lauth
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bridge-login/debug/ -lauth
else:unix: LIBS += -L$$OUT_PWD/../bridge-login/ -lauth

INCLUDEPATH += $$PWD/../bridge-login
DEPENDPATH += $$PWD/../bridge-login

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Bridge-Utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Bridge-Utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../Bridge-Utils/ -lutils

INCLUDEPATH += $$PWD/../Bridge-Utils
DEPENDPATH += $$PWD/../Bridge-Utils

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../client-roommanager/release/ -lroommanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../client-roommanager/debug/ -lroommanager
else:unix: LIBS += -L$$OUT_PWD/../client-roommanager/ -lroommanager

INCLUDEPATH += $$PWD/../client-roommanager
DEPENDPATH += $$PWD/../client-roommanager
