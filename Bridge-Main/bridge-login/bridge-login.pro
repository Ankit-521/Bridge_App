QT  += core gui quickwidgets quick networkauth
QT += widgets

CONFIG += c++11

include(../assets.pri)

TEMPLATE = lib
TARGET = auth
DEFINES += AUTHMANAGER_LIBRARY



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
    authenticator.cpp \
    authmanager.cpp

HEADERS += \
    Bridge-login_global.h \
    authenticator.h \
    authmanager.h

FORMS += \
    authmanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RESOURCES += \
    qml.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Bridge-Utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Bridge-Utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../Bridge-Utils/ -lutils

INCLUDEPATH += $$PWD/../Bridge-Utils
DEPENDPATH += $$PWD/../Bridge-Utils
