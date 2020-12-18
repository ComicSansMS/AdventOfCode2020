add_library(18_operation_order STATIC operation_order.hpp operation_order.cpp)
target_include_directories(18_operation_order PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(18_operation_order PUBLIC fmt::fmt range-v3)
add_executable(advent18 advent18.cpp)
target_link_libraries(advent18 PUBLIC 18_operation_order)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(18_operation_order_test operation_order.t.cpp)
    target_link_libraries(18_operation_order_test PUBLIC Catch 18_operation_order)
    add_test(NAME TestAdvent18 COMMAND 18_operation_order_test)
    enable_coverage(18_operation_order)
endif()
