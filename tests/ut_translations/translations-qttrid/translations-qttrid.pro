QMAKE_EXTRA_TARGETS += check
check.depends = .
check.commands = echo "nothing to do here for “make check”"

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = .
check-xml.commands = echo "nothing to do here for “make check-xml”"

LANGUAGES = ar_EG en_GB de_DE
CATALOGNAME = ut_translations-qttrid
SOURCEDIR = $$PWD/..
TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = $$[QT_INSTALL_LIBS]/libmlocale-tests5/translations-qttrid
# these include files are installed to $$[QT_INSTALL_DATA]/mkspecs/features
# and included in the "libmlocale-dev" package:
include($${M_BUILD_TREE}/mkspecs/features/mlocale_defines.prf)
include($${M_SOURCE_TREE}/mkspecs/features/mlocale_translations.prf)
