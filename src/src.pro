QPERSISTENCE_PATH = ../lib/QPersistence
include($$QPERSISTENCE_PATH/QPersistence.pri)

SPARKLE_PATH = ../lib/Sparkle
include($$SPARKLE_PATH/Sparkle.pri)

### General config ###

TARGET          = psnewmodel
VERSION         = 0.0.0
TEMPLATE        = app
ICON            = icon.icns
QT              += sql widgets
CONFIG          += c++11
QMAKE_CXXFLAGS  += $$QPERSISTENCE_COMMON_QMAKE_CXXFLAGS


### Info.plist ###

TARGET_PLIST = $${OUT_PWD}/$${TARGET}.app/Contents/Info.plist
QMAKE_INFO_PLIST    = $$PWD/misc/Info.plist
PRE_TARGETDEPS      += $$TARGET_PLIST


### Deployment ###

DEPLOY_FILENAME = $${TARGET}-$${VERSION}.zip
DEPLOY_FILE = $${OUT_PWD}/../$${TARGET}-$${VERSION}.zip
SPARKLE_APPCAST = $$PWD/misc/updater/sparkle-appcast.xml
DEPLOY_TARGET.target    = deploy

DEPLOY_TARGET.commands  = macdeployqt $${OUT_PWD}/$${TARGET}.app
DEPLOY_TARGET.commands  += ;/usr/libexec/PlistBuddy -c \"Set :CFBundleVersion $${VERSION}\" $$TARGET_PLIST
DEPLOY_TARGET.commands  += ;rm -R $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/accessible
DEPLOY_TARGET.commands  += ;rm -R $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/printsupport
DEPLOY_TARGET.commands  += ;rm $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/imageformats/libqmng.dylib
DEPLOY_TARGET.commands  += ;rm $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/imageformats/libqtga.dylib
DEPLOY_TARGET.commands  += ;rm $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/imageformats/libqtiff.dylib
DEPLOY_TARGET.commands  += ;rm $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/imageformats/libqwbmp.dylib
DEPLOY_TARGET.commands  += ;rm $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/sqldrivers/libqsqlmysql.dylib
DEPLOY_TARGET.commands  += ;rm $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib
DEPLOY_TARGET.commands  += ;rm $${OUT_PWD}/$${TARGET}.app/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib
DEPLOY_TARGET.commands  += ;rm $$DEPLOY_FILE
DEPLOY_TARGET.commands  += ;cd $${OUT_PWD} && /usr/bin/zip -q -y $$DEPLOY_FILE -r $${TARGET}.app && cd -
DEPLOY_TARGET.commands  += ;rm $$SPARKLE_APPCAST
DEPLOY_TARGET.commands  += ;cp $$PWD/misc/updater/sparkle-appcast.template.xml $$SPARKLE_APPCAST
DEPLOY_TARGET.commands  += ;awk \'\{ gsub(/\\[DSASIGNATURE\\]/, \
                            \"\'\"`/usr/bin/ruby \"$$PWD/misc/updater/sign_update.rb\" $$DEPLOY_FILE ~/.sparkle/dsa_priv.pem`\"\'\"); print \}\' \
                            $$SPARKLE_APPCAST > $${SPARKLE_APPCAST}.tmp && mv $${SPARKLE_APPCAST}.tmp $$SPARKLE_APPCAST
DEPLOY_TARGET.commands  += ;sed -i \'\' \'s/\\[DATE\\]/\'\"`date`\"\'/g\' "$$SPARKLE_APPCAST"
DEPLOY_TARGET.commands  += ;sed -i \'\' \'s/\\[LENGTH\\]/\'\"`stat -f "%z" "$$DEPLOY_FILE"`\"\'/g\' "$$SPARKLE_APPCAST"
DEPLOY_TARGET.commands  += ;sed -i \'\' \'s/\\[VERSION\\]/$$VERSION/g\' "$$SPARKLE_APPCAST"
DEPLOY_TARGET.commands  += ;sed -i \'\' \'s/\\[FILENAME\\]/$$DEPLOY_FILENAME/g\' "$$SPARKLE_APPCAST"

QMAKE_EXTRA_TARGETS += DEPLOY_TARGET



### Qp ###

INCLUDEPATH     += $$QPERSISTENCE_INCLUDEPATH
LIBS            += $$QPERSISTENCE_LIBS
POST_TARGETDEPS += $$QPERSISTENCE_POST_TARGETDEPS


### Sparkle ###

INCLUDEPATH     += $$SPARKLE_INCLUDEPATH
LIBS            += $$SPARKLE_LIBS
QMAKE_LFLAGS    += $$SPARKLE_LFLAGS
QMAKE_BUNDLE_DATA += SPARKLE_FRAMEWORK

SPARKLE_SIGNATURE.files = $$PWD/misc/updater/dsa_pub.pem
SPARKLE_SIGNATURE.path  = Contents/Resources
QMAKE_BUNDLE_DATA       += SPARKLE_SIGNATURE


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
    ui/model/drinkslistmodel.cpp \
    ui/model/gamelistmodel.cpp \
    ui/model/playerslistmodel.cpp \
    ui/mainwindow.cpp \
    ui/dialogs/playerinformationdialog.cpp \
    ui/widgets/imagewell.cpp \
    misc/tools.cpp \
    ui/game/gamewindow.cpp \
    ui/model/gameoverviewmodel.cpp \
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
    ui/dialogs/drinkinformationdialog.cpp \
    ui/widgets/menubar.cpp \
    ui/widgets/playerslistwidget.cpp \
    ui/widgets/placescombobox.cpp \
    ui/widgets/playerslistdelegate.cpp \
    ui/widgets/popupwidget.cpp \
    ui/model/gameinformationmodel.cpp \
    ui/widgets/objectlistwidget.cpp \
    ui/widgets/drinkslistwidget.cpp \
    data/old_offlineGameInformation.cpp \
    data/old_dokoofflinegamebuddys.cpp \
    ui/startwindow.cpp \
    ui/game/playerstatswidget.cpp \
    ui/league/addleaguedialog.cpp \
    ui/league/leaguewindow.cpp \
    ui/model/leagueclassementmodel.cpp \
    ui/league/leagueplayerheaderview.cpp \
    ui/league/leagueplacementdelegate.cpp \
    ui/model/leaguegameplacementmodel.cpp \
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
    misc/updater/updater.cpp \
    ui/game/dialogcontroller.cpp \
    data/gamecomparestatistics.cpp \
    ui/game/drinkstatswidget.cpp \
    ui/game/gamecomparestatswidget.cpp \
    ui/widgets/startwidget.cpp


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
    ui/model/drinkslistmodel.h \
    ui/model/gamelistmodel.h \
    ui/model/playerslistmodel.h \
    ui/mainwindow.h \
    ui/dialogs/playerinformationdialog.h \
    ui/widgets/imagewell.h \
    misc/tools.h \
    ui/game/gamewindow.h \
    ui/model/gameoverviewmodel.h \
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
    ui/dialogs/drinkinformationdialog.h \
    ui/widgets/menubar.h \
    ui/widgets/playerslistwidget.h \
    ui/widgets/placescombobox.h \
    ui/widgets/playerslistdelegate.h \
    ui/widgets/popupwidget.h \
    ui/model/gameinformationmodel.h \
    ui/widgets/objectlistwidget.h \
    ui/widgets/drinkslistwidget.h \
    data/old_offlineGameInformation.h \
    data/old_dokoofflinegamebuddys.h \
    ui/startwindow.h \
    ui/game/playerstatswidget.h \
    ui/league/addleaguedialog.h \
    ui/league/leaguewindow.h \
    ui/model/leagueclassementmodel.h \
    ui/league/leagueplayerheaderview.h \
    ui/league/leagueplacementdelegate.h \
    ui/model/leaguegameplacementmodel.h \
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
    ui/widgets/clickablelabel.h \
    ui/game/gamelengthwidget.h \
    ui/game/recontrastatswidget.h \
    ui/game/resumewidget.h \
    ui/game/pointsstatswidget.h \
    ui/game/drinkswidget.h \
    misc/cocoainitializer.h \
    misc/updater/sparkleupdater.h \
    misc/updater/updater.h \
    ui/game/dialogcontroller.h \
    data/gamecomparestatistics.h \
    ui/game/drinkstatswidget.h \
    ui/game/gamecomparestatswidget.h \
    ui/widgets/startwidget.h


FORMS += \
    ui/mainwindow.ui \
    ui/dialogs/playerinformationdialog.ui \
    ui/game/gamewindow.ui \
    ui/game/newrounddialog.ui \
    ui/game/schmeissereidialog.ui \
    ui/game/statisticswidget.ui \
    ui/dialogs/drinkinformationdialog.ui \
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
    ui/game/drinkswidget.ui \
    ui/game/drinkstatswidget.ui \
    ui/game/gamecomparestatswidget.ui \
    ui/widgets/startwidget.ui

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
    resource/drinks/krombacher_weizen.png \
    misc/Info.plist

OBJECTIVE_SOURCES += \
    misc/cocoainitializer.mm \
    misc/updater/sparkleupdater.mm
