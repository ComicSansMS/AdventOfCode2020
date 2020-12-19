add_library(19_monster_messages STATIC monster_messages.hpp monster_messages.cpp)
target_include_directories(19_monster_messages PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(19_monster_messages PUBLIC fmt::fmt range-v3)
add_executable(advent19 advent19.cpp)
target_link_libraries(advent19 PUBLIC 19_monster_messages)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(19_monster_messages_test monster_messages.t.cpp)
    target_link_libraries(19_monster_messages_test PUBLIC Catch 19_monster_messages)
    add_test(NAME TestAdvent19 COMMAND 19_monster_messages_test)
    enable_coverage(19_monster_messages)
endif()
