add_library(02_password_philosophy STATIC password_philosophy.hpp password_philosophy.cpp)
target_include_directories(02_password_philosophy PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(02_password_philosophy PUBLIC fmt::fmt range-v3)
add_executable(advent02 advent02.cpp)
target_link_libraries(advent02 PUBLIC 02_password_philosophy)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(02_password_philosophy_test password_philosophy.t.cpp)
    target_link_libraries(02_password_philosophy_test PUBLIC Catch 02_password_philosophy)
    add_test(NAME TestAdvent02 COMMAND 02_password_philosophy_test)
    enable_coverage(02_password_philosophy)
endif()
