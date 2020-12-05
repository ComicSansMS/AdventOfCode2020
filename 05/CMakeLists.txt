add_library(05_binary_boarding STATIC binary_boarding.hpp binary_boarding.cpp)
target_include_directories(05_binary_boarding PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(05_binary_boarding PUBLIC fmt::fmt range-v3)
add_executable(advent05 advent05.cpp)
target_link_libraries(advent05 PUBLIC 05_binary_boarding)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(05_binary_boarding_test binary_boarding.t.cpp)
    target_link_libraries(05_binary_boarding_test PUBLIC Catch 05_binary_boarding)
    add_test(NAME TestAdvent05 COMMAND 05_binary_boarding_test)
    enable_coverage(05_binary_boarding)
endif()
