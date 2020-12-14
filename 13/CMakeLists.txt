add_library(13_shuttle_search STATIC shuttle_search.hpp shuttle_search.cpp)
target_include_directories(13_shuttle_search PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(13_shuttle_search PUBLIC fmt::fmt range-v3)
add_executable(advent13 advent13.cpp)
target_link_libraries(advent13 PUBLIC 13_shuttle_search)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(13_shuttle_search_test shuttle_search.t.cpp)
    target_link_libraries(13_shuttle_search_test PUBLIC Catch 13_shuttle_search)
    add_test(NAME TestAdvent13 COMMAND 13_shuttle_search_test)
    enable_coverage(13_shuttle_search)
endif()
