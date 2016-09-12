include(../common_top.pri)

TARGET = ft_mcharsetdetector

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mlocale.cpp \

# Input
HEADERS += ft_mcharsetdetector.h
SOURCES += ft_mcharsetdetector.cpp

support_files.files += \
    *.txt

include(../common_bot.pri)
