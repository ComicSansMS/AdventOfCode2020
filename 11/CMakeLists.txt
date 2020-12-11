add_library(11_seating_system STATIC seating_system.hpp seating_system.cpp)
target_include_directories(11_seating_system PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(11_seating_system PUBLIC fmt::fmt range-v3)
add_executable(advent11 advent11.cpp)
target_link_libraries(advent11 PUBLIC 11_seating_system)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(11_seating_system_test seating_system.t.cpp)
    target_link_libraries(11_seating_system_test PUBLIC Catch 11_seating_system)
    add_test(NAME TestAdvent11 COMMAND 11_seating_system_test)
    enable_coverage(11_seating_system)
endif()
