isEmpty(QPERSISTENCE_PATH) {
    error(You have to set QPERSISTENCE_PATH to the path of QPersistence relative to your project)
}

QPERSISTENCE_TARGET          = qpersistence
QPERSISTENCE_VERSION         = 0.0.0
QPERSISTENCE_INCLUDEPATH     = $$PWD/include
!win32: QPERSISTENCE_POST_TARGETDEPS = $$OUT_PWD/$$QPERSISTENCE_PATH/src/lib$${QPERSISTENCE_TARGET}.a


win32:CONFIG(debug, debug|release): QPERSISTENCE_LIBS += -L$$OUT_PWD/$$QPERSISTENCE_PATH/src/debug -l$$QPERSISTENCE_TARGET
else:win32:CONFIG(release, debug|release): QPERSISTENCE_LIBS += -L$$OUT_PWD/$$QPERSISTENCE_PATH/src/release -l$$QPERSISTENCE_TARGET
else:unix: LIBS += -L$$QPERSISTENCE_PATH/src -l$$QPERSISTENCE_TARGET
