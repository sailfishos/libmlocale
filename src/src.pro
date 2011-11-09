CONFIG += ordered

DEFINES += M_BUILD_MLOCALE

include(common_top.pri)

TARGET = mlocale

INCLUDEPATH += .

RESOURCES += i18n.qrc

PUBLIC_HEADERS += \
    mbreakiterator.h \
    mlocale.h \
    mlocalebuckets.h \
    mlocaleexport.h \
    mcountry.h \
    mcity.h \
    mlocationdatabase.h \
    mlocaleabstractconfigitem.h \
    mlocaleabstractconfigitemfactory.h \
    mlocalenullconfigitem.h \
    mlocalenullconfigitemfactory.h \

PRIVATE_HEADERS += \
    mcalendar_p.h \
    debug.h \

SOURCES += \
    mbreakiterator.cpp \
    mlocale.cpp \
    mlocalebuckets.cpp \
    mcountry.cpp \
    mcity.cpp \
    mlocationdatabase.cpp \
    \
    mlocaleabstractconfigitem.cpp \
    mlocalenullconfigitem.cpp \
    mlocalenullconfigitemfactory.cpp \
    debug.cpp \

contains(DEFINES, HAVE_ICU) {
    PUBLIC_HEADERS += \
        mcalendar.h \
        mcollator.h \
        mcharsetdetector.h \
        mcharsetmatch.h \
        mstringsearch.h \

    PRIVATE_HEADERS += \
        micubreakiterator.h \
        micuconversions.h

    SOURCES += \
        mcalendar.cpp \
        mcollator.cpp \
        micubreakiterator.cpp \
        micuconversions.cpp \
        mcharsetdetector.cpp \
        mcharsetmatch.cpp \
        mstringsearch.cpp \

} else {
    PRIVATE_HEADERS += \
        mnullbreakiterator.h \

    SOURCES += \
        mnullbreakiterator.cpp
}

contains(DEFINES, HAVE_QMSYSTEM2) {
    CONFIG+=qmsystem2
    QMAKE_CXXFLAGS -= -pedantic
}

HEADERS += \
    $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS

install_headers.path = $$ML_INSTALL_HEADERS
install_headers.files = $$PUBLIC_HEADERS
INSTALLS += target \
    install_headers
###

contains(DEFINES, HAVE_ICU) {
    SUBDIRS += icu-extradata
}

# install MeeGo Touch feature files
install_prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
install_prf.files = \
    $${M_SOURCE_TREE}/mkspecs/features/mlocale.prf \
    $${M_SOURCE_TREE}/mkspecs/features/mlocale_common.prf \
    $${M_SOURCE_TREE}/mkspecs/features/mlocale_coverage.prf \
    $${M_SOURCE_TREE}/mkspecs/features/mlocale_translations.prf \
    $${M_BUILD_TREE}/mkspecs/features/mlocale_defines.prf \

INSTALLS += install_prf

# install pkgconfig file
install_pkgconfig.path = $$ML_INSTALL_LIBS/pkgconfig
install_pkgconfig.files = data/mlocale.pc

INSTALLS += install_pkgconfig

# install forwarding headers
win32 {
    # for windows we can live with the broken pattern matching
    FORWARDING_HEADERS = include/M*
} else {
    # match only the camel case forwarding headers here
    FORWARDING_HEADERS = $$system( find include/M* )
}
forwarding_headers.path = $$ML_INSTALL_HEADERS
forwarding_headers.files = $$FORWARDING_HEADERS
INSTALLS += forwarding_headers
