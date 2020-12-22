add_library(22_crab_combat STATIC crab_combat.hpp crab_combat.cpp)
target_include_directories(22_crab_combat PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(22_crab_combat PUBLIC fmt::fmt range-v3)
add_executable(advent22 advent22.cpp)
target_link_libraries(advent22 PUBLIC 22_crab_combat)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(22_crab_combat_test crab_combat.t.cpp)
    target_link_libraries(22_crab_combat_test PUBLIC Catch 22_crab_combat)
    add_test(NAME TestAdvent22 COMMAND 22_crab_combat_test)
    enable_coverage(22_crab_combat)
endif()
