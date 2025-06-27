QT       += core gui
QT+=network sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    baseworker.cpp \
    fileworker.cpp \
    linksql.cpp \
    main.cpp \
    mainwindow.cpp \
    msgworker.cpp \
    mytcpserver.cpp

HEADERS += \
    FileTransferContext.h \
    baseworker.h \
    fileworker.h \
    linksql.h \
    mainwindow.h \
    msgworker.h \
    mytcpserver.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
