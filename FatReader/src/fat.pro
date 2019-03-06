TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fatcontroller.cpp \
    dir.cpp \
    file.cpp \
    tools.cpp \
    vdrivecontroller.cpp \
    drivecontroller.cpp \
    fatcluster.cpp

HEADERS += \
    fatcontroller.h \
    fatstructures.h \
    dir.h \
    file.h \
    tools.h \
    vdrivecontroller.h \
    drivecontroller.h \
    fatcluster.h

