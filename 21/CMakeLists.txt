add_library(21_allergen_assessment STATIC allergen_assessment.hpp allergen_assessment.cpp)
target_include_directories(21_allergen_assessment PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(21_allergen_assessment PUBLIC fmt::fmt range-v3)
add_executable(advent21 advent21.cpp)
target_link_libraries(advent21 PUBLIC 21_allergen_assessment)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(21_allergen_assessment_test allergen_assessment.t.cpp)
    target_link_libraries(21_allergen_assessment_test PUBLIC Catch 21_allergen_assessment)
    add_test(NAME TestAdvent21 COMMAND 21_allergen_assessment_test)
    enable_coverage(21_allergen_assessment)
endif()
