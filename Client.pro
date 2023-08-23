QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Entity/datapacket.cpp \
    Entity/feedbacksign.cpp \
    Entity/requestbody.cpp \
    Service/clientservice.cpp \
    Util/md5util.cpp \
    View/DownLoad.cpp \
    View/DownThread.cpp \
    View/Request.cpp \
    View/config.cpp \
    View/main.cpp \
    View/mainpage.cpp \
    View/mainwindow.cpp \
    View/registerpage.cpp

HEADERS += \
    Entity/datapacket.h \
    Entity/feedbacksign.h \
    Entity/requestbody.h \
    Service/clientservice.h \
    Util/md5util.h \
    View/DownFileInfo.h \
    View/DownLoad.h \
    View/DownThread.h \
    View/Request.h \
    View/config.h \
    View/mainpage.h \
    View/mainwindow.h \
    View/registerpage.h

FORMS += \
    View/mainpage.ui \
    View/mainwindow.ui \
    View/registerpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    MainWindow/mainWindow.qss \
    View/mainWindow.qss \
    qss/mainWindow.qss

RESOURCES += \
    Resource/Resource.qrc
