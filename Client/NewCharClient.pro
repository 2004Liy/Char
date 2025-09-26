QT       += core gui
QT+=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriend.cpp \
    addgroup.cpp \
    addgroupitem.cpp \
    attnitem.cpp \
    chatplaintext.cpp \
    form.cpp \
    frienditem.cpp \
    friendsendfile.cpp \
    home.cpp \
    isfriend.cpp \
    main.cpp \
    mainwindow.cpp \
    mybase.cpp \
    myinformation.cpp \
    mysendfile.cpp \
    prompt.cpp \
    register.cpp \
    senddata.cpp \
    showgroupmember.cpp \
    stickerwidget.cpp

HEADERS += \
    FileTransferContext.h \
    addfriend.h \
    addgroup.h \
    addgroupitem.h \
    attnitem.h \
    chatplaintext.h \
    form.h \
    frienditem.h \
    friendsendfile.h \
    home.h \
    isfriend.h \
    mainwindow.h \
    mybase.h \
    myinformation.h \
    mysendfile.h \
    prompt.h \
    register.h \
    senddata.h \
    showgroupmember.h \
    stickerwidget.h

FORMS += \
    addfriend.ui \
    addgroup.ui \
    addgroupitem.ui \
    attnitem.ui \
    chatplaintext.ui \
    form.ui \
    frienditem.ui \
    friendsendfile.ui \
    home.ui \
    isfriend.ui \
    mainwindow.ui \
    myinformation.ui \
    mysendfile.ui \
    prompt.ui \
    register.ui \
    showgroupmember.ui \
    stickerwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    111.qrc
