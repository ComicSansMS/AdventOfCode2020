add_library(01_report_repair STATIC report_repair.hpp report_repair.cpp)
target_include_directories(01_report_repair PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(01_report_repair PUBLIC fmt::fmt range-v3)
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC 01_report_repair)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(01_report_repair_test report_repair.t.cpp)
    target_link_libraries(01_report_repair_test PUBLIC Catch 01_report_repair)
    add_test(NAME TestAdvent01 COMMAND 01_report_repair_test)
    enable_coverage(01_report_repair)
endif()
