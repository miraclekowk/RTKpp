cmake_minimum_required(VERSION 3.21)
project(RTKpp)

set(CMAKE_CXX_STANDARD 11)


add_executable(RTKpp main.cpp utils.cpp utils.h rtk_request.cpp rtk_request.h stdafx.h rtk_response.cpp rtk_response.h rtk_io.cpp rtk_io.h rtk_threadpool.cpp rtk_threadpool.h rtk_epoll.cpp rtk_epoll.h rtk_timer.cpp rtk_timer.h)
add_subdirectory(./googletest)
include_directories(${PROJECT_SOURCE_DIR}/src/include ./googletest/include)
link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/googletest)
target_link_libraries(${PROJECT_NAME} gtest)