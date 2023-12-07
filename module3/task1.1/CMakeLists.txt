cmake_minimum_required(VERSION 3.13)

project(cpp_tasks)
set(CMAKE_CXX_STANDARD 17)


set(HEADERS
        include/IGraph.hpp
        include/ArcGraph.hpp
        include/ListGraph.hpp
        include/MatrixGraph.hpp
        include/SetGraph.hpp
)
set(SOURCES
        source/ArcGraph.cpp
        source/ListGraph.cpp
        source/MatrixGraph.cpp
        source/SetGraph.cpp
)


add_executable(main main.cpp
        ${HEADERS}
        ${SOURCES}
)
