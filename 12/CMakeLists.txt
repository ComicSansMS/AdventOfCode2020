add_library(12_rain_risk STATIC rain_risk.hpp rain_risk.cpp)
target_include_directories(12_rain_risk PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(12_rain_risk PUBLIC fmt::fmt range-v3)
add_executable(advent12 advent12.cpp)
target_link_libraries(advent12 PUBLIC 12_rain_risk)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(12_rain_risk_test rain_risk.t.cpp)
    target_link_libraries(12_rain_risk_test PUBLIC Catch 12_rain_risk)
    add_test(NAME TestAdvent12 COMMAND 12_rain_risk_test)
    enable_coverage(12_rain_risk)
endif()
