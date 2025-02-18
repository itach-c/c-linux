
project(testHttp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
link_libraries(http)
add_executable(httpserver test1.cc)

