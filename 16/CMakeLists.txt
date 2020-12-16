add_library(16_ticket_translation STATIC ticket_translation.hpp ticket_translation.cpp)
target_include_directories(16_ticket_translation PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(16_ticket_translation PUBLIC fmt::fmt range-v3)
add_executable(advent16 advent16.cpp)
target_link_libraries(advent16 PUBLIC 16_ticket_translation)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(16_ticket_translation_test ticket_translation.t.cpp)
    target_link_libraries(16_ticket_translation_test PUBLIC Catch 16_ticket_translation)
    add_test(NAME TestAdvent16 COMMAND 16_ticket_translation_test)
    enable_coverage(16_ticket_translation)
endif()
