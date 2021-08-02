cmake_minimum_required (VERSION 3.10)

set(PROJECT_NAME artwork_extractor)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

project(${PROJECT_NAME})

add_library(${PROJECT_NAME} SHARED artwork_extractor.cpp artwork_extractor.def)

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND copy \"${CMAKE_CURRENT_SOURCE_DIR}\\win32\\MediaInfo.dll\" \"${CMAKE_CURRENT_SOURCE_DIR}\\${CMAKE_BUILD_TYPE}\"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_include_directories(
        ${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/win32
    )
    target_link_libraries(
        ${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/win32/MediaInfo.lib
    )
else ()
    target_include_directories(
        ${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(
        ${PROJECT_NAME} PUBLIC
        mediainfo
        dl
    )
endif ()

add_executable(${PROJECT_NAME}-bin artwork_extractor.cpp)

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}-bin PRE_BUILD
        COMMAND copy \"${CMAKE_CURRENT_SOURCE_DIR}\\win32\\MediaInfo.dll\" \"${CMAKE_CURRENT_SOURCE_DIR}\\${CMAKE_BUILD_TYPE}\"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_include_directories(
        ${PROJECT_NAME}-bin PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/win32
    )
    target_link_libraries(
        ${PROJECT_NAME}-bin PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/win32/MediaInfo.lib
    )
else ()
    target_include_directories(
        ${PROJECT_NAME}-bin PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(
        ${PROJECT_NAME}-bin PUBLIC
        mediainfo
        dl
    )
endif ()

set_target_properties(artwork_extractor
        PROPERTIES OUTPUT_NAME artwork_extractor)