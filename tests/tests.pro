TEMPLATE    = subdirs

include(../mkspecs/common.pri)

HEADERS += \
    utils.h \

SUBDIRS     =  \
    ut_translations \
    ut_translations/translations-tr \
    ut_translations/translations-qttrid \
    ut_phonenumberformatting \
    ut_mlocationdatabase \

# enable only when we have icu available

contains(DEFINES, HAVE_ICU) {
SUBDIRS += \
    ft_breakiterator \
    ft_locales \
    ft_localedata \
    ft_mlocalebuckets \
    ft_numbers \
    ft_sorting \
    ft_mcharsetdetector \
    ft_mstringsearch \
    ut_mcalendar
}

QMAKE_STRIP = echo
include(shell.pri)
include(runtests.pri)


check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check


check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml **/*.log
