# 引入gtest库
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 添加测试源文件
set(TEST_SOURCES
    test.cpp
    ../src/GreedyFAS.cpp
    ../src/SortFAS.cpp
    ../src/PageRankFAS.cpp
)

# 定义测试用例
add_executable(test ${TEST_SOURCES})

# 链接gtest和Boost库
target_link_libraries(test ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES})

# 指定测试和可执行文件
add_test(NAME MinFeedBackArcSetTest COMMAND test)