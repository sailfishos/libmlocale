cmake_minimum_required(VERSION 3.16)

project(mlocale6
    VERSION 0.1.1
    DESCRIPTION "MeeGo Touch Locale Library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

option(ENABLE_ICU "Enable ICU support for full locale functionality" ON)
option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

find_package(Qt6 REQUIRED COMPONENTS Core Xml)

if(ENABLE_ICU)
    find_package(ICU REQUIRED COMPONENTS uc i18n data)
    add_compile_definitions(HAVE_ICU)
endif()

include(GNUInstallDirs)
set(ML_INSTALL_LIBS ${CMAKE_INSTALL_LIBDIR})
set(ML_INSTALL_HEADERS ${CMAKE_INSTALL_INCLUDEDIR}/mlocale6)
set(ML_TRANSLATION_DIR ${CMAKE_INSTALL_DATADIR}/l10n/mlocale6)
set(ML_ICUEXTRADATA_DIR ${CMAKE_INSTALL_DATADIR}/mlocale6/icu)

add_compile_definitions(
    M_VERSION="${PROJECT_VERSION}"
    M_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
    M_MINOR_VERSION=${PROJECT_VERSION_MINOR}
    M_PATCH_VERSION=${PROJECT_VERSION_PATCH}
    TRANSLATION_DIR="${CMAKE_INSTALL_PREFIX}/${ML_TRANSLATION_DIR}"
    ML_ICUEXTRADATA_DIR="${CMAKE_INSTALL_PREFIX}/${ML_ICUEXTRADATA_DIR}"
    NO_COLOR
    QT_USE_FAST_CONCATENATION
    QT_USE_FAST_OPERATOR_PLUS
    QT_STRICT_ITERATORS
)

add_subdirectory(src)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
