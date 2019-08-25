include($$mac_compiler)
MOC_DIR=$$top_builddir/tmp/common/
OBJECTS_DIR=$$top_builddir/tmp/common/

DESTDIR=$$top_builddir/libs/

QT += core network

TEMPLATE = lib

TARGET = common

CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$top_srcdir/src \
               $$top_srcdir/3rdparty/pugixml/src

SOURCES += downloadmanager.cpp \
           autoupdate.cpp

HEADERS += downloadmanager.h \
           autoupdate.h
