find_package(Qt6 REQUIRED COMPONENTS Test Gui)

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
