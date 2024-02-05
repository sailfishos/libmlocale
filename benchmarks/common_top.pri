# for defines
include(../mkspecs/common.pri)

MSRCDIR = $${M_SOURCE_TREE}/src
STUBSDIR = ../stubs
INCLUDEPATH += \
    . \
    ../common \
    $$MSRCDIR/include \
    $$MSRCDIR \

DEPENDPATH = $$INCLUDEPATH
QMAKE_LIBDIR += ../../lib
CONFIG += debug
QT += testlib

TEMPLATE = app
# DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += UNIT_TEST
target.path = $$[QT_INSTALL_LIBS]/libmlocale-benchmarks$${QT_MAJOR_VERSION}
INSTALLS += target

LIBS += $$mAddLibrary(mlocale$${QT_MAJOR_VERSION})

support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libmlocale-benchmarks$${QT_MAJOR_VERSION}
INSTALLS += support_files

CONFIG-=app_bundle
