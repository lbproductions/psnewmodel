TARGET          = QImageMetaData
TEMPLATE        = lib
CONFIG         += static
QT             += gui
DESTDIR         = $$PWD/../lib

HEADERS += \
    QImageMetaData.h \
    QImageMetaDataRational.h \
    Test.hpp

SOURCES += \
    QImageMetaData.cpp \
    Test.cpp \
    TestMetaData.cpp