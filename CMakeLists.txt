cmake_minimum_required(VERSION 3.12)
project(VoronoiX)
set(CMAKE_CXX_STANDARD 17)

set(
        HEADER_FILES
        include/vx/math/vec.hpp
        include/vx/solver.hpp
        include/vx/model/polygon.hpp
        include/vx/model/graph.hpp
        include/vx/core.hpp
        src/dcel/edge.hpp
        src/dcel/face.hpp
        src/dcel/vertex.hpp
        src/voronoi.hpp
        src/core_impl.hpp
        src/core_extended.hpp
)

set(
        SOURCE_FILES
        src/dcel/edge.cpp
        src/dcel/face.cpp
        src/dcel/vertex.cpp
        src/math/vec.cpp
        src/model/polygon.cpp
        src/model/graph.cpp
        src/solver.cpp
        src/voronoi.cpp
        src/core_impl.cpp
        src/core_extended.cpp
)

# VoronoiX Library

add_library(VoronoiX ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(
        VoronoiX
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# VoronoiX runnable

add_executable(main main.cpp)

target_link_libraries(main VoronoiX)