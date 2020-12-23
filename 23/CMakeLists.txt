add_library(23_crab_cups STATIC crab_cups.hpp crab_cups.cpp)
target_include_directories(23_crab_cups PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(23_crab_cups PUBLIC fmt::fmt range-v3)
add_executable(advent23 advent23.cpp)
target_link_libraries(advent23 PUBLIC 23_crab_cups)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(23_crab_cups_test crab_cups.t.cpp)
    target_link_libraries(23_crab_cups_test PUBLIC Catch 23_crab_cups)
    add_test(NAME TestAdvent23 COMMAND 23_crab_cups_test)
    enable_coverage(23_crab_cups)
endif()
