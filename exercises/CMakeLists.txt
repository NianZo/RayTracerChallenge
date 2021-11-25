set(BINARY ${CMAKE_PROJECT_NAME})
set(SOURCES main.cpp)

add_executable(${BINARY}_exercises ${SOURCES})
target_include_directories(${BINARY}_exercises PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${BINARY}_exercises PUBLIC ${CMAKE_PROJECT_NAME}_lib)
target_link_libraries(${BINARY}_exercises PRIVATE project_warnings)
