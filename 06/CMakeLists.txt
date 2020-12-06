add_library(06_custom_customs STATIC custom_customs.hpp custom_customs.cpp)
target_include_directories(06_custom_customs PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(06_custom_customs PUBLIC fmt::fmt range-v3)
add_executable(advent06 advent06.cpp)
target_link_libraries(advent06 PUBLIC 06_custom_customs)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(06_custom_customs_test custom_customs.t.cpp)
    target_link_libraries(06_custom_customs_test PUBLIC Catch 06_custom_customs)
    add_test(NAME TestAdvent06 COMMAND 06_custom_customs_test)
    enable_coverage(06_custom_customs)
endif()
