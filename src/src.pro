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
    ui/game/graphwidget.cpp \
    ui/widgets/scrollarea.cpp \
    ui/game/graphaxis.cpp \
    ui/game/newrounddialog.cpp \
    ui/widgets/playerscombobox.cpp \
    ui/game/schmeissereidialog.cpp \
    ui/game/statisticswidget.cpp \
    ui/widgets/shadowframe.cpp \
    ui/drinkinformationdialog.cpp \
    ui/widgets/menubar.cpp \
    ui/widgets/playerslistwidget.cpp \
    ui/widgets/placescombobox.cpp \
    ui/widgets/playerslistdelegate.cpp \
    ui/widgets/popupwidget.cpp \
    model/gameinformationmodel.cpp \
    ui/widgets/objectlistwidget.cpp \
    ui/widgets/drinkslistwidget.cpp \
    ui/game/adddrinkswidget.cpp \
    data/old_offlineGameInformation.cpp \
    data/old_dokoofflinegamebuddys.cpp \
    ui/startwindow.cpp


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
    ui/game/graphwidget.h \
    ui/widgets/scrollarea.h \
    ui/game/graphaxis.h \
    ui/game/newrounddialog.h \
    ui/widgets/playerscombobox.h \
    ui/game/schmeissereidialog.h \
    ui/game/statisticswidget.h \
    ui/widgets/shadowframe.h \
    ui/drinkinformationdialog.h \
    ui/widgets/menubar.h \
    ui/widgets/playerslistwidget.h \
    ui/widgets/placescombobox.h \
    ui/widgets/playerslistdelegate.h \
    ui/widgets/popupwidget.h \
    model/gameinformationmodel.h \
    ui/widgets/objectlistwidget.h \
    ui/widgets/drinkslistwidget.h \
    ui/game/adddrinkswidget.h \
    data/old_offlineGameInformation.h \
    data/old_dokoofflinegamebuddys.h \
    ui/startwindow.h


FORMS += \
    ui/mainwindow.ui \
    ui/playerinformationdialog.ui \
    ui/game/gamewindow.ui \
    ui/game/newrounddialog.ui \
    ui/game/schmeissereidialog.ui \
    ui/game/statisticswidget.ui \
    ui/drinkinformationdialog.ui

RESOURCES += \
    resource/sidebar/sidebar.qrc \
    resource/statusbar/statusbar.qrc \
    resource/general/general.qrc \
    resource/stylesheets/stylesheets.qrc \
    resource/pushbutton/pushbutton.qrc

OTHER_FILES += \
    resource/stylesheets/imagewell.qss \
    resource/stylesheets/pushbutton-dark.qss \
    resource/stylesheets/bubble.qss
