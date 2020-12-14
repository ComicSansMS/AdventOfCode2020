add_library(14_docking_data STATIC docking_data.hpp docking_data.cpp)
target_include_directories(14_docking_data PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(14_docking_data PUBLIC fmt::fmt range-v3)
add_executable(advent14 advent14.cpp)
target_link_libraries(advent14 PUBLIC 14_docking_data)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(14_docking_data_test docking_data.t.cpp)
    target_link_libraries(14_docking_data_test PUBLIC Catch 14_docking_data)
    add_test(NAME TestAdvent14 COMMAND 14_docking_data_test)
    enable_coverage(14_docking_data)
endif()
