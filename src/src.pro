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
    data/drink.cpp \
    data/game.cpp \
    data/league.cpp \
    data/livedrink.cpp \
    data/place.cpp \
    data/player.cpp \
    data/playerstatistics.cpp \
    data/round.cpp \
    data/schmeisserei.cpp \
    model/drinkslistmodel.cpp \
    model/gamelistmodel.cpp \
    model/playerslistmodel.cpp \
    ui/mainwindow.cpp \
    ui/playerinformationdialog.cpp \
    ui/widgets/imagewell.cpp \
    misc/tools.cpp \
    ui/game/gamewindow.cpp \
    model/gameoverviewmodel.cpp \
    ui/game/overviewplayerheaderview.cpp \
    ui/game/overviewhorizontalheaderview.cpp \
    ui/game/overviewdelegate.cpp \
    ui/game/graphwidget.cpp


HEADERS  += \
    data/drink.h \
    data/game.h \
    data/league.h \
    data/livedrink.h \
    data/place.h \
    data/player.h \
    data/playerstatistics.h \
    data/round.h \
    data/schmeisserei.h \
    model/drinkslistmodel.h \
    model/gamelistmodel.h \
    model/playerslistmodel.h \
    ui/mainwindow.h \
    ui/playerinformationdialog.h \
    ui/widgets/imagewell.h \
    misc/tools.h \
    ui/game/gamewindow.h \
    model/gameoverviewmodel.h \
    ui/game/overviewplayerheaderview.h \
    ui/game/overviewhorizontalheaderview.h \
    ui/game/overviewdelegate.h \
    ui/game/graphwidget.h


FORMS += \
    ui/mainwindow.ui \
    ui/playerinformationdialog.ui \
    ui/game/gamewindow.ui

RESOURCES += \
    resource/sidebar/sidebar.qrc \
    resource/statusbar/statusbar.qrc \
    resource/general/general.qrc \
    resource/stylesheets/stylesheets.qrc

OTHER_FILES += \
    resource/stylesheets/imagewell.qss
