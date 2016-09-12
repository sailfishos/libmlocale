include(../common_top.pri)

TARGET = ft_locales

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mlocale.cpp \ 

# Input
HEADERS += ft_locales.h
SOURCES += ft_locales.cpp
LIBS += -licuuc

support_files.files += \
    *.txt

include(../common_bot.pri)
