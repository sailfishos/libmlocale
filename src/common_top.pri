# this file contains the parts that are common to all libs

MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = $$M_BUILD_TREE/lib
TEMPLATE = lib

include(../mkspecs/common.pri)

VERSION = $${ML_VERSION}
DEFINES += M_VERSION=\\\"$${M_VERSION}\\\"

DEFINES += M_MAJOR_VERSION=$${ML_MAJOR_VERSION}
DEFINES += M_MINOR_VERSION=$${ML_MINOR_VERSION}
DEFINES += M_PATCH_VERSION=$${ML_PATCH_VERSION}

# enable debug log if needed
LOGDEBUG=$$(LOGDEBUG)
contains(LOGDEBUG, yes){
    DEFINES += LOGDEBUG
}

# make debug.cpp build
DEFINES += NO_COLOR

INCLUDEPATH += $${M_SOURCE_TREE}/src/include
INCLUDEPATH += $${OUT_PWD}/.moc

QMAKE_STRIP = echo

CONFIG += qt warn_on depend_includepath qmake_cache target_qt dll create_prl
!win32:CONFIG += link_pkgconfig
macx:CONFIG -= lib_bundle

!win32-msvc*:QMAKE_CXXFLAGS += -Wno-long-long -pedantic
!win32:QMAKE_CXXFLAGS += -Wno-variadic-macros -fvisibility=hidden

!win32-msvc*:LIBS += -lm -lX11
macx:LIBS -= -lX11
win32:LIBS -= -lX11

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators,
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS


## Build configuration



## Features

contains(M_BUILD_FEATURES, testable) {
  DEFINES += TESTABLE
  HEADERS += ../corelib/core/testabilityinterface.h
}


## Dependencies

contains(DEFINES, HAVE_ICU) {
    LIBS += -licui18n -licuuc -licudata
}

QT += \
    svg \
    network \
    core \
    gui \
    xml

!simulator: QT += opengl

QMAKE_EXTRA_TARGETS += check
check.depends = $$DESTDIR/lib$${TARGET}.so.$$VERSION
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$DESTDIR/lib$${TARGET}.so.$$VERSION
check-xml.commands = $$system(true)

target.path = $$ML_INSTALL_LIBS
