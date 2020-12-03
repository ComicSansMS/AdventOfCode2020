add_library(03_toboggan_trajectory STATIC toboggan_trajectory.hpp toboggan_trajectory.cpp)
target_include_directories(03_toboggan_trajectory PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(03_toboggan_trajectory PUBLIC fmt::fmt range-v3)
add_executable(advent03 advent03.cpp)
target_link_libraries(advent03 PUBLIC 03_toboggan_trajectory)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(03_toboggan_trajectory_test toboggan_trajectory.t.cpp)
    target_link_libraries(03_toboggan_trajectory_test PUBLIC Catch 03_toboggan_trajectory)
    add_test(NAME TestAdvent03 COMMAND 03_toboggan_trajectory_test)
    enable_coverage(03_toboggan_trajectory)
endif()
