include(check.pri)

# for defines
include(../mkspecs/common.pri)

MSRCDIR = $${M_SOURCE_TREE}/src
STUBSDIR = ../stubs
INCLUDEPATH += \
    . \
    $$MSRCDIR \
    $$STUBSDIR \
    $$MSRCDIR/include \

DEPENDPATH = $$INCLUDEPATH
QMAKE_LIBDIR += ../../lib /usr/local/lib
CONFIG += debug
CONFIG -= app_bundle
QT += testlib

TEMPLATE = app
# DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += UNIT_TEST
target.path = $$[QT_INSTALL_LIBS]/libmlocale-tests5
INSTALLS += target

LIBS += $$mAddLibrary(mlocale$${QT_MAJOR_VERSION})

support_files.files =
support_files.path = $$[QT_INSTALL_LIBS]/libmlocale-tests5
INSTALLS += support_files
