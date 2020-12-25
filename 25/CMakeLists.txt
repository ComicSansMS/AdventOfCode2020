add_library(25_combo_breaker STATIC combo_breaker.hpp combo_breaker.cpp)
target_include_directories(25_combo_breaker PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(25_combo_breaker PUBLIC fmt::fmt range-v3)
add_executable(advent25 advent25.cpp)
target_link_libraries(advent25 PUBLIC 25_combo_breaker)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(25_combo_breaker_test combo_breaker.t.cpp)
    target_link_libraries(25_combo_breaker_test PUBLIC Catch 25_combo_breaker)
    add_test(NAME TestAdvent25 COMMAND 25_combo_breaker_test)
    enable_coverage(25_combo_breaker)
endif()
