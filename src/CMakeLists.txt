set(PUBLIC_HEADERS
    mbreakiterator.h
    mlocale.h
    mlocalebuckets.h
    mlocaleexport.h
    mcountry.h
    mcity.h
    mlocationdatabase.h
    mlocaleabstractconfigitem.h
    mlocaleabstractconfigitemfactory.h
    mlocalenullconfigitem.h
    mlocalenullconfigitemfactory.h
)

set(PRIVATE_HEADERS
    mcalendar_p.h
    debug.h
)

set(SOURCES
    mbreakiterator.cpp
    mlocale.cpp
    mlocalebuckets.cpp
    mcountry.cpp
    mcity.cpp
    mlocationdatabase.cpp
    mlocaleabstractconfigitem.cpp
    mlocalenullconfigitem.cpp
    mlocalenullconfigitemfactory.cpp
    debug.cpp
)

if(ENABLE_ICU)
    list(APPEND PUBLIC_HEADERS
        mcalendar.h
        mcollator.h
        mcharsetdetector.h
        mcharsetmatch.h
        mstringsearch.h
    )

    list(APPEND PRIVATE_HEADERS
        micubreakiterator.h
        micuconversions.h
    )

    list(APPEND SOURCES
        mcalendar.cpp
        mcollator.cpp
        micubreakiterator.cpp
        micuconversions.cpp
        mcharsetdetector.cpp
        mcharsetmatch.cpp
        mstringsearch.cpp
    )
else()
    list(APPEND PRIVATE_HEADERS
        mnullbreakiterator.h
    )

    list(APPEND SOURCES
        mnullbreakiterator.cpp
    )
endif()

set(RESOURCES i18n.qrc)

add_library(mlocale6
    ${SOURCES}
    ${PUBLIC_HEADERS}
    ${PRIVATE_HEADERS}
    ${RESOURCES}
)

add_library(mlocale6::mlocale6 ALIAS mlocale6)

target_compile_definitions(mlocale6 PRIVATE M_BUILD_MLOCALE)
target_compile_definitions(mlocale6 PRIVATE QT_DEPRECATED_WARNINGS)

target_include_directories(mlocale6
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/mlocale6>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(Qt6 COMPONENTS Core5Compat QUIET)

target_link_libraries(mlocale6
    PUBLIC
        Qt6::Core
    PRIVATE
        Qt6::Xml
)

if(Qt6Core5Compat_FOUND)
    target_link_libraries(mlocale6 PRIVATE Qt6::Core5Compat)
    target_compile_definitions(mlocale6 PRIVATE HAVE_QT5COMPAT)
endif()

if(ENABLE_ICU)
    target_link_libraries(mlocale6 PRIVATE ICU::uc ICU::i18n ICU::data)
endif()

if(NOT MSVC)
    target_compile_options(mlocale6 PRIVATE
        -Wno-long-long
        -Wno-variadic-macros
        -fvisibility=hidden
    )
endif()

set_target_properties(mlocale6 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME mlocale6
    AUTOMOC_MOC_OPTIONS "-p;${CMAKE_CURRENT_SOURCE_DIR}"
)

install(TARGETS mlocale6
    EXPORT mlocale6Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mlocale6
)

file(GLOB FORWARDING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/M*")
install(FILES ${FORWARDING_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mlocale6
)

install(EXPORT mlocale6Targets
    FILE mlocale6Targets.cmake
    NAMESPACE mlocale6::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mlocale6
)
