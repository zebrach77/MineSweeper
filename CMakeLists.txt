cmake_minimum_required(VERSION 3.21)

project(Mines)

set(CMAKE_CXX_STANDARD 20)

include_directories(widgets_lib/include include)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_subdirectory(widgets_lib)

enable_testing()
find_package(GTest REQUIRED)

include_directories(test)
add_executable(Test test/test.cpp
                    src/MineSweeperMap.cpp
)

target_link_libraries(Test GTest::gtest GTest::gtest_main)

add_executable(Mines  main.cpp
                      src/MineSweeperMap.cpp
                      src/GameSingleton.cpp

)

target_link_libraries(Mines sfml-graphics sfml-audio)
target_link_libraries(Mines widget_lib)