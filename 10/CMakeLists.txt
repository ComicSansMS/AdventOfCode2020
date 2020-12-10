add_library(10_adapter_array STATIC adapter_array.hpp adapter_array.cpp)
target_include_directories(10_adapter_array PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(10_adapter_array PUBLIC fmt::fmt range-v3)
add_executable(advent10 advent10.cpp)
target_link_libraries(advent10 PUBLIC 10_adapter_array)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(10_adapter_array_test adapter_array.t.cpp)
    target_link_libraries(10_adapter_array_test PUBLIC Catch 10_adapter_array)
    add_test(NAME TestAdvent10 COMMAND 10_adapter_array_test)
    enable_coverage(10_adapter_array)
endif()
