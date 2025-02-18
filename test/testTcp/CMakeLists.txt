
project(testTcp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
link_libraries(tcp)
add_executable(tcpserver server.cc)

