cmake_minimum_required(VERSION 3.8)

project(FAS)

set(CMAKE_CXX_STANDARD 17)
set(CAMKE_CXX_FLAGS "-O2")

# 添加Boost库
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# 添加项目源文件
set(SOURCES
    src/main.cpp
    src/GreedyFAS.cpp
    src/SortFAS.cpp
    src/PageRankFAS.cpp
)

# 添加头文件目录
include_directories(include)

# 定义可执行文件
add_executable(FASSolver ${SOURCES})

# 链接Boost库
target_link_libraries(FASSolver ${Boost_LIBRARIES})

# 引入gtest库
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 添加测试目录
add_subdirectory(test)
