QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    collectionsaddwindow.cpp \
    main.cpp \
    autorizationwindow.cpp \
    movieshowwindow.cpp \
    registrationwindow.cpp \
    globals.cpp \
    moviewindow.cpp

HEADERS += \
    collectionsaddwindow.h \
    movieshowwindow.h \
    registrationwindow.h \
    autorizationwindow.h \
    globals.h \
    moviewindow.h

FORMS += \
    autorizationwindow.ui \
    collectionsaddwindow.ui \
    movieshowwindow.ui \
    registrationwindow.ui \
    moviewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += \target


# Копирование содержимого папки "images" в созданную папку
CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug

images.path = $$DESTDIR/images
images.files = images/*

INSTALLS += \
images
