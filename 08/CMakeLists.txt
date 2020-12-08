add_library(08_handheld_halting STATIC handheld_halting.hpp handheld_halting.cpp)
target_include_directories(08_handheld_halting PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(08_handheld_halting PUBLIC fmt::fmt range-v3)
add_executable(advent08 advent08.cpp)
target_link_libraries(advent08 PUBLIC 08_handheld_halting)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(08_handheld_halting_test handheld_halting.t.cpp)
    target_link_libraries(08_handheld_halting_test PUBLIC Catch 08_handheld_halting)
    add_test(NAME TestAdvent08 COMMAND 08_handheld_halting_test)
    enable_coverage(08_handheld_halting)
endif()
