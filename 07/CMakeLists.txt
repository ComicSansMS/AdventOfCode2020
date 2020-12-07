add_library(07_handy_haversacks STATIC handy_haversacks.hpp handy_haversacks.cpp)
target_include_directories(07_handy_haversacks PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(07_handy_haversacks PUBLIC fmt::fmt range-v3)
add_executable(advent07 advent07.cpp)
target_link_libraries(advent07 PUBLIC 07_handy_haversacks)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(07_handy_haversacks_test handy_haversacks.t.cpp)
    target_link_libraries(07_handy_haversacks_test PUBLIC Catch 07_handy_haversacks)
    add_test(NAME TestAdvent07 COMMAND 07_handy_haversacks_test)
    enable_coverage(07_handy_haversacks)
endif()
