shell_scripts.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
shell_scripts.files += $$OUT_PWD/tests.xml
shell_scripts.CONFIG += no_check_exist

include(../mkspecs/common.pri)

shell_scripts.path += $$ML_INSTALL_DATA/libmlocale-tests
shell_scripts.depends = FORCE

INSTALLS    += \
              shell_scripts
