file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        )

message("Surces: ${SOURCES}" )

add_library(render STATIC ${SOURCES})

target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
target_link_libraries(render PUBLIC libjpeg)
