cmake_minimum_required (VERSION 3.10)

set(PROJECT_NAME artwork_extractor)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} artwork_extractor.cpp)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    mediainfo
    dl
)
