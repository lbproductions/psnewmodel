QPERSISTENCE_PATH = ../lib/QPersistence
include($$QPERSISTENCE_PATH/QPersistence.pri)

### General config ###

TARGET          = psnewmodel
VERSION         = 0.0.0
TEMPLATE        = app
QT              += sql widgets
CONFIG          += c++11
QMAKE_CXXFLAGS  += $$QPERSISTENCE_COMMON_QMAKE_CXXFLAGS

### Qp ###

INCLUDEPATH     += $$QPERSISTENCE_INCLUDEPATH
LIBS            += $$QPERSISTENCE_LIBS


### Files ###

SOURCES += main.cpp \
    data/player.cpp \
    data/place.cpp \
    data/drink.cpp \
    data/game.cpp \
    data/livedrink.cpp \
    data/round.cpp \
    data/schmeisserei.cpp \
    mainwindow.cpp \
    model/playerslistmodel.cpp

HEADERS  += \
    data/player.h \
    data/place.h \
    data/drink.h \
    data/game.h \
    data/livedrink.h \
    data/round.h \
    data/schmeisserei.h \
    mainwindow.h \
    model/playerslistmodel.h

FORMS += \
    mainwindow.ui
