project(widget_lib)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(include)

add_library(widget_lib SHARED src/widgets.cpp)

target_link_libraries(widget_lib sfml-graphics sfml-audio)