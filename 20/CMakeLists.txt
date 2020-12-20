add_library(20_jurassic_jigsaw STATIC jurassic_jigsaw.hpp jurassic_jigsaw.cpp)
target_include_directories(20_jurassic_jigsaw PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(20_jurassic_jigsaw PUBLIC fmt::fmt range-v3)
add_executable(advent20 advent20.cpp)
target_link_libraries(advent20 PUBLIC 20_jurassic_jigsaw)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(20_jurassic_jigsaw_test jurassic_jigsaw.t.cpp)
    target_link_libraries(20_jurassic_jigsaw_test PUBLIC Catch 20_jurassic_jigsaw)
    add_test(NAME TestAdvent20 COMMAND 20_jurassic_jigsaw_test)
    enable_coverage(20_jurassic_jigsaw)
endif()
