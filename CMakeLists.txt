cmake_minimum_required(VERSION 2.8.3)
project(video_distance)

find_package(OpenCV REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})


add_library(video_distance video_distance.cpp)

add_executable(main main.cpp)


add_dependencies(main video_distance)


target_link_libraries(video_distance
  ${OpenCV_LIBS}
)

target_link_libraries(main
  video_distance
)
