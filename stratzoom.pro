RESOURCES += images.qrc

HEADERS += mainwindow.h view.h \
    dss.h
SOURCES += main.cpp \
    dss.cpp
SOURCES += mainwindow.cpp view.cpp

QT += widgets

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

