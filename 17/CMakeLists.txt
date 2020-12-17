add_library(17_conway_cubes STATIC conway_cubes.hpp conway_cubes.cpp)
target_include_directories(17_conway_cubes PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(17_conway_cubes PUBLIC fmt::fmt range-v3)
add_executable(advent17 advent17.cpp)
target_link_libraries(advent17 PUBLIC 17_conway_cubes)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(17_conway_cubes_test conway_cubes.t.cpp)
    target_link_libraries(17_conway_cubes_test PUBLIC Catch 17_conway_cubes)
    add_test(NAME TestAdvent17 COMMAND 17_conway_cubes_test)
    enable_coverage(17_conway_cubes)
endif()
