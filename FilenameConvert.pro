QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# use for cmd
#win32 {
#    CONFIG += console
#    CONFIG -= app_bundle
#}

INCLUDEPATH += opencc/include

Debug:LIBPATH += opencc/lib/debug
Release:LIBPATH += opencc/lib/release

LIBS += -lopencc

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    worker.cpp

HEADERS += \
    mainwindow.h \
    worker.h \
    setting.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
