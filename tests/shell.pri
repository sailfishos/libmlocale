equals(QT_MAJOR_VERSION, 4): ml_unittest_install_path = $$[QT_INSTALL_LIBS]/libmlocale-tests
equals(QT_MAJOR_VERSION, 5): ml_unittest_install_path = $$[QT_INSTALL_LIBS]/libmlocale-tests5

shell_scripts.commands += $$PWD/gen-tests-xml.sh $${ml_unittest_install_path} > $$OUT_PWD/tests.xml
shell_scripts.files += $$OUT_PWD/tests.xml
shell_scripts.CONFIG += no_check_exist

include(../mkspecs/common.pri)

equals(QT_MAJOR_VERSION, 4): shell_scripts.path += $$ML_INSTALL_DATA/libmlocale-tests
equals(QT_MAJOR_VERSION, 5): shell_scripts.path += $$ML_INSTALL_DATA/libmlocale-tests5
shell_scripts.path += $$ML_INSTALL_DATA/libmlocale-tests
shell_scripts.depends = FORCE

INSTALLS    += \
              shell_scripts
