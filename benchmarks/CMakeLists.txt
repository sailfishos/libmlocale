find_package(Qt6 REQUIRED COMPONENTS Test)

function(add_mlocale_benchmark BENCH_NAME)
    set(BENCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${BENCH_NAME})
    
    if(EXISTS ${BENCH_DIR}/${BENCH_NAME}.cpp)
        add_executable(${BENCH_NAME} ${BENCH_DIR}/${BENCH_NAME}.cpp)
        
        target_include_directories(${BENCH_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${BENCH_DIR}
        )
        
        target_link_libraries(${BENCH_NAME} PRIVATE
            mlocale5
            Qt6::Core
            Qt6::Test
        )
    endif()
endfunction()

add_mlocale_benchmark(pt_mlocationdatabase)
add_mlocale_benchmark(pt_mlocale)

if(ENABLE_ICU)
    add_mlocale_benchmark(pt_mcalendar)
    add_mlocale_benchmark(pt_mcharsetdetector)
endif()
