file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/windows/*.cpp"
        )

message("Surces: ${SOURCES}" )

add_library(osapi STATIC ${SOURCES})

target_include_directories(osapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/windows)
target_include_directories(osapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)