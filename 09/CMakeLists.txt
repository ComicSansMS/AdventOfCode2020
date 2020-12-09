add_library(09_encoding_error STATIC encoding_error.hpp encoding_error.cpp)
target_include_directories(09_encoding_error PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(09_encoding_error PUBLIC fmt::fmt range-v3)
add_executable(advent09 advent09.cpp)
target_link_libraries(advent09 PUBLIC 09_encoding_error)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(09_encoding_error_test encoding_error.t.cpp)
    target_link_libraries(09_encoding_error_test PUBLIC Catch 09_encoding_error)
    add_test(NAME TestAdvent09 COMMAND 09_encoding_error_test)
    enable_coverage(09_encoding_error)
endif()
