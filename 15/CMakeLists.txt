add_library(15_rambunctious_recitation STATIC rambunctious_recitation.hpp rambunctious_recitation.cpp)
target_include_directories(15_rambunctious_recitation PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(15_rambunctious_recitation PUBLIC fmt::fmt range-v3)
add_executable(advent15 advent15.cpp)
target_link_libraries(advent15 PUBLIC 15_rambunctious_recitation)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(15_rambunctious_recitation_test rambunctious_recitation.t.cpp)
    target_link_libraries(15_rambunctious_recitation_test PUBLIC Catch 15_rambunctious_recitation)
    add_test(NAME TestAdvent15 COMMAND 15_rambunctious_recitation_test)
    enable_coverage(15_rambunctious_recitation)
endif()
