shell_scripts.commands += $$PWD/gen-benchmarks-xml.sh > $$OUT_PWD/tests.xml
shell_scripts.files += $$OUT_PWD/tests.xml
shell_scripts.CONFIG += no_check_exist

include(../mkspecs/common.pri)

equals(QT_MAJOR_VERSION, 4): shell_scripts.path += $$ML_INSTALL_DATA/libmlocale-benchmarks
equals(QT_MAJOR_VERSION, 5): shell_scripts.path += $$ML_INSTALL_DATA/libmlocale-benchmarks5
shell_scripts.depends = FORCE

INSTALLS    += \
              shell_scripts
