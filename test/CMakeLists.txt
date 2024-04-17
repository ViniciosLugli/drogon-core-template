cmake_minimum_required(VERSION 3.5)
project(DrogonApp_test CXX)

file(GLOB TEST_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
add_executable(${PROJECT_NAME} ${TEST_SRC_FILES})

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_custom_target(${PROJECT_NAME}_headers SOURCES ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

ParseAndAddDrogonTests(${PROJECT_NAME})
