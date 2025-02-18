
project(http)


set(LIBRARY_OUTPUT_PATH ${LIBDIR})

link_libraries(tcp)
aux_source_directory(${PROJECT_SOURCE_DIR} SRC)
include_directories(${PROJECT_SOURCE_DIR}/../comm/)
add_library(http SHARED ${SRC})


