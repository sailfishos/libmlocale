find_package(Qt6 REQUIRED COMPONENTS Test Gui LinguistTools)

function(add_mlocale_test TEST_NAME)
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME})
    
    if(EXISTS ${TEST_DIR}/${TEST_NAME}.cpp)
        add_executable(${TEST_NAME} ${TEST_DIR}/${TEST_NAME}.cpp)
        
        target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${TEST_DIR}
        )
        
        target_link_libraries(${TEST_NAME} PRIVATE
            mlocale6
            Qt6::Core
            Qt6::Gui
            Qt6::Test
            $<$<BOOL:${ENABLE_ICU}>:ICU::uc>
            $<$<BOOL:${ENABLE_ICU}>:ICU::i18n>
        )
        
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endif()
endfunction()

add_mlocale_test(ut_translations)
add_mlocale_test(ut_phonenumberformatting)
add_mlocale_test(ut_mlocationdatabase)

if(ENABLE_ICU)
    add_mlocale_test(ft_breakiterator)
    add_mlocale_test(ft_locales)
    add_mlocale_test(ft_mlocalebuckets)
    add_mlocale_test(ft_numbers)
    add_mlocale_test(ft_sorting)
    add_mlocale_test(ft_mcharsetdetector)
    add_mlocale_test(ft_mstringsearch)
    add_mlocale_test(ut_mcalendar)
endif()

# Compile the test .ts into .qm files
set(tr_src ${CMAKE_CURRENT_SOURCE_DIR}/ut_translations/translations-tr)
qt_add_lrelease(TS_FILES
        ${tr_src}/ut_translations-tr_en_GB.ts
        ${tr_src}/ut_translations-tr_fi_FI.ts
    QM_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/translations-tr
    LRELEASE_TARGET ut_translations_tr_qm)
add_dependencies(ut_translations ut_translations_tr_qm)

# Generate the engineering english catalog with lupdate.
# "!! " is prefixed as an indicator that engineering english (not real english) is used.
set(qttrid_src ${CMAKE_CURRENT_SOURCE_DIR}/ut_translations/translations-qttrid)
set(qttrid_eng_ts ${CMAKE_CURRENT_BINARY_DIR}/translations-qttrid/ut_translations-qttrid.ts)
qt_add_lupdate(SOURCE_TARGETS ut_translations
    TS_FILES ${qttrid_eng_ts}
    LUPDATE_TARGET ut_translations_qttrid_lupdate
    NO_GLOBAL_TARGET)
qt_add_lrelease(TS_FILES
        ${qttrid_eng_ts}
        ${qttrid_src}/ut_translations-qttrid_ar_EG.ts
        ${qttrid_src}/ut_translations-qttrid_de_DE.ts
        ${qttrid_src}/ut_translations-qttrid_en_GB.ts
    QM_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/translations-qttrid
    LRELEASE_TARGET ut_translations_qttrid_qm
    OPTIONS -markuntranslated "!! ")
add_dependencies(ut_translations_qttrid_qm ut_translations_qttrid_lupdate)
add_dependencies(ut_translations ut_translations_qttrid_qm)
