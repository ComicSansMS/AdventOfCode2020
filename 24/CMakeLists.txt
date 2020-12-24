add_library(24_lobby_layout STATIC lobby_layout.hpp lobby_layout.cpp)
target_include_directories(24_lobby_layout PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(24_lobby_layout PUBLIC fmt::fmt range-v3)
add_executable(advent24 advent24.cpp)
target_link_libraries(advent24 PUBLIC 24_lobby_layout)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(24_lobby_layout_test lobby_layout.t.cpp)
    target_link_libraries(24_lobby_layout_test PUBLIC Catch 24_lobby_layout)
    add_test(NAME TestAdvent24 COMMAND 24_lobby_layout_test)
    enable_coverage(24_lobby_layout)
endif()
