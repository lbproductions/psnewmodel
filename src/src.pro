QPERSISTENCE_PATH = ../lib/QPersistence
include($$QPERSISTENCE_PATH/QPersistence.pri)

### General config ###

TARGET          = psnewmodel
VERSION         = 0.0.0
TEMPLATE        = app
ICON            = icon.icns
QT              += sql widgets
CONFIG          += c++11
QMAKE_CXXFLAGS  += $$QPERSISTENCE_COMMON_QMAKE_CXXFLAGS


### Qp ###

INCLUDEPATH     += $$QPERSISTENCE_INCLUDEPATH
LIBS            += $$QPERSISTENCE_LIBS
POST_TARGETDEPS += $$QPERSISTENCE_POST_TARGETDEPS


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
    data/old_offlineGameInformation.cpp \
    data/old_dokoofflinegamebuddys.cpp \
    ui/startwindow.cpp \
    ui/game/playerstatswidget.cpp \
    ui/league/addleaguedialog.cpp \
    ui/league/leaguewindow.cpp \
    model/leagueclassementmodel.cpp \
    ui/league/leagueplayerheaderview.cpp \
    ui/league/leagueplacementdelegate.cpp \
    model/leaguegameplacementmodel.cpp \
    ui/league/leaguehorizontalheaderview.cpp \
    data/matchday.cpp \
    ui/widgets/graphwidget.cpp \
    ui/game/gamegraphwidget.cpp \
    ui/league/leaguegraphwidget.cpp \
    ui/widgets/solooverviewwidget.cpp \
    ui/widgets/recontrapiewidget.cpp \
    ui/widgets/totalpointswidget.cpp \
    ui/widgets/gamestogetherwidget.cpp \
    ui/widgets/gameswidget.cpp \
    ui/game/commentwidget.cpp \
    ui/game/stopgamewidget.cpp \
    ui/game/gamestatswidget.cpp \
    ui/widgets/biggestdistancewidget.cpp \
    ui/game/settingswidget.cpp \
    misc/settings.cpp \
    ui/widgets/clickablelabel.cpp \
    ui/game/gamelengthwidget.cpp \
    ui/game/recontrastatswidget.cpp \
    ui/game/resumewidget.cpp \
    ui/game/pointsstatswidget.cpp \
    ui/game/drinkswidget.cpp \
    ui/game/dialogcontroller.cpp


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
    data/old_offlineGameInformation.h \
    data/old_dokoofflinegamebuddys.h \
    ui/startwindow.h \
    ui/game/playerstatswidget.h \
    ui/league/addleaguedialog.h \
    ui/league/leaguewindow.h \
    model/leagueclassementmodel.h \
    ui/league/leagueplayerheaderview.h \
    ui/league/leagueplacementdelegate.h \
    model/leaguegameplacementmodel.h \
    ui/league/leaguehorizontalheaderview.h \
    data/matchday.h \
    ui/widgets/graphwidget.h \
    ui/game/gamegraphwidget.h \
    ui/league/leaguegraphwidget.h \
    ui/widgets/solooverviewwidget.h \
    ui/widgets/recontrapiewidget.h \
    ui/widgets/totalpointswidget.h \
    ui/widgets/gamestogetherwidget.h \
    ui/widgets/gameswidget.h \
    ui/game/commentwidget.h \
    ui/game/stopgamewidget.h \
    ui/game/gamestatswidget.h \
    ui/widgets/biggestdistancewidget.h \
    ui/game/settingswidget.h \
    misc/settings.h \
    local_dbconfig.h \
    ui/widgets/clickablelabel.h \
    ui/game/gamelengthwidget.h \
    ui/game/recontrastatswidget.h \
    ui/game/resumewidget.h \
    ui/game/pointsstatswidget.h \
    ui/game/drinkswidget.h \
    ui/game/dialogcontroller.h


FORMS += \
    ui/mainwindow.ui \
    ui/playerinformationdialog.ui \
    ui/game/gamewindow.ui \
    ui/game/newrounddialog.ui \
    ui/game/schmeissereidialog.ui \
    ui/game/statisticswidget.ui \
    ui/drinkinformationdialog.ui \
    ui/league/addleaguedialog.ui \
    ui/league/leaguewindow.ui \
    ui/widgets/solooverviewwidget.ui \
    ui/widgets/totalpointswidget.ui \
    ui/game/commentwidget.ui \
    ui/game/stopgamewidget.ui \
    ui/game/gamestatswidget.ui \
    ui/widgets/biggestdistancewidget.ui \
    ui/game/settingswidget.ui \
    ui/game/recontrastatswidget.ui \
    ui/game/resumewidget.ui \
    ui/game/drinkswidget.ui

RESOURCES += \
    resource/sidebar/sidebar.qrc \
    resource/statusbar/statusbar.qrc \
    resource/general/general.qrc \
    resource/stylesheets/stylesheets.qrc \
    resource/pushbutton/pushbutton.qrc \
    resource/gamewindow/gamewindow.qrc \
    resource/drinks/drinks.qrc \
    resource/leaguewindow/leaguewindow.qrc

OTHER_FILES += \
    resource/stylesheets/imagewell.qss \
    resource/stylesheets/pushbutton-dark.qss \
    resource/stylesheets/bubble.qss \
    resource/gamewindow/bubenSolo.png \
    resource/drinks/wickingerblut.png \
    resource/drinks/beer_default.png \
    resource/drinks/astra_rotlicht.png \
    resource/drinks/veltins_033.png \
    resource/drinks/wickueler_033.png \
    resource/drinks/veltins_05.png \
    resource/drinks/gose.png \
    resource/drinks/brinkhoffs_033.png \
    resource/drinks/warsteiner_cola_033.png \
    resource/drinks/krombacher033.png \
    resource/drinks/v+cola.png \
    resource/drinks/v+apple.png \
    resource/drinks/v+energy.png \
    resource/drinks/v+lemon.png \
    resource/drinks/bitburger_05.png \
    resource/drinks/erdinger.png \
    resource/drinks/whiskeycola.png \
    resource/drinks/gambrinus10.png \
    resource/drinks/oettingerradler05.png \
    resource/drinks/paulaner.png \
    resource/drinks/brinkhoffs_05.png \
    resource/drinks/gambrinus11.png \
    resource/drinks/potts.png \
    resource/drinks/mixerycola05.png \
    resource/drinks/hoevels.png \
    resource/drinks/krombacher_weizen.png
