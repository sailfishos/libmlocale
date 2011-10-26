# Build configuration

QMAKE_TARGET_COMPANY = Nokia
QMAKE_TARGET_PRODUCT = DirectUI
QMAKE_TARGET_DESCRIPTION = DirectUI Framework
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2010 Nokia


# Features
# Parts to build. Options: libs tests benchmarks demos doc debian
#M_BUILD_PARTS = libs tests benchmarks demos doc debian

# Load configure script results
!win32: {
    include($${M_BUILD_TREE}/mkspecs/mlocaleconfig.pri)
}

#disable werrors on windows and freemantle (M_OS_MAEMO5 comes from meegotouchconfig.pri)
win32 {
    } else {
        contains( DEFINES, M_OS_MAEMO5 ) {
        } else { 
	    !contains( M_BUILD_FEATURES, no-werror ) {
	        QMAKE_CFLAGS *= -Werror
	        QMAKE_CXXFLAGS *= -Werror
            }
        }
    }

mac {
    INCLUDEPATH += include
}

# Load global definitions
include($${M_BUILD_TREE}/mkspecs/features/mlocale_defines.prf)

# Defines for directories, for use in source code.
{
    # TRANSLATION_DIR determines the default translation path
    DEFINES += TRANSLATION_DIR=\\\"\"$$ML_TRANSLATION_DIR\"\\\"

    # extra data for libicu to be used be default
    DEFINES += ML_ICUEXTRADATA_DIR=\\\"\"$$ML_ICUEXTRADATA_DIR\"\\\"
}

unix {
   # drop unneeded dependencies
   !macx {
      QMAKE_LFLAGS += "-Wl,--as-needed"
   }
}

# Compiler configuration for all subprojects in libm

!win32-msvc*:QMAKE_CXXFLAGS *= -g

contains( CONFIG, coverage ) {
    QMAKE_CXXFLAGS *= -ftest-coverage -fprofile-arcs
    LIBS *= -lgcov

    QMAKE_CLEAN *= \
    *.gcda \
    *.gcno \
    *.gcov
}

QMAKE_LIBDIR += $${M_BUILD_TREE}/lib

include(shared.pri)
