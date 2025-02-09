cmake_minimum_required(VERSION 3.22.1)

project(meta_programing_utils)
enable_language(CXX)

include(GNUInstallDirs REQUIRED)

set(CMAKE_CXX_STANDARD 20)
message( "${PROJECT_SOURCE_DIR}/src/*")

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE PROJECT_PUB_INC_FILES "${PROJECT_SOURCE_DIR}/include/*")
file(GLOB_RECURSE PROJECT_SRC_FILES "${PROJECT_SOURCE_DIR}/src/*")

set_target_properties("${PROJECT_NAME}" PROPERTIES PUBLIC_HEADER "${PROJECT_PUB_INC_FILES}")

target_include_directories("${PROJECT_NAME}" PUBLIC
       "${PROJECT_SOURCE_DIR}/include"
)

target_sources("${PROJECT_NAME}" PRIVATE
    "${PROJECT_SRC_FILES}"
)

install(TARGETS "${PROJECT_NAME}"
        PUBLIC_HEADER
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/"${PROJECT_NAME}"
)