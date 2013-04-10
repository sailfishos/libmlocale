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
equals(QT_MAJOR_VERSION, 4): target.path = $$[QT_INSTALL_LIBS]/libmlocale-benchmarks
equals(QT_MAJOR_VERSION, 5): target.path = $$[QT_INSTALL_LIBS]/libmlocale-benchmarks5
INSTALLS += target

LIBS += $$mAddLibrary(mlocale)

support_files.files =
equals(QT_MAJOR_VERSION, 4): support_files.path = $$[QT_INSTALL_LIBS]/libmlocale-benchmarks
equals(QT_MAJOR_VERSION, 5): support_files.path = $$[QT_INSTALL_LIBS]/libmlocale-benchmarks5
INSTALLS += support_files

CONFIG-=app_bundle
