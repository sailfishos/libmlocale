TEMPLATE    = subdirs

include(../mkspecs/common.pri)

SUBDIRS     = \
 pt_mlocationdatabase \
 pt_mlocale \

contains(DEFINES, HAVE_ICU) {
SUBDIRS += \
 pt_mcalendar \
 pt_mcharsetdetector
}

include(shell.pri)

