add_library(04_passport_processing STATIC passport_processing.hpp passport_processing.cpp)
target_include_directories(04_passport_processing PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(04_passport_processing PUBLIC fmt::fmt range-v3)
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC 04_passport_processing)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(04_passport_processing_test passport_processing.t.cpp)
    target_link_libraries(04_passport_processing_test PUBLIC Catch 04_passport_processing)
    add_test(NAME TestAdvent04 COMMAND 04_passport_processing_test)
    enable_coverage(04_passport_processing)
endif()
