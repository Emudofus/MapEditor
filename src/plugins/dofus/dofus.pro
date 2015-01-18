include(../plugin.pri)

DEFINES += DOFUS_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4) {
    OTHER_FILES =
}

HEADERS += \
    dofus_global.h \
    dofusplugin.h

SOURCES += \
    dofusplugin.cpp


