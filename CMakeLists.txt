cmake_minimum_required(VERSION 3.20)

project(JogoLP1)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED
    allegro-5
    allegro_image-5
    allegro_font-5
    allegro_ttf-5
    allegro_primitives-5
    allegro_main-5
)

include_directories(
    include
    ${ALLEGRO_INCLUDE_DIRS}
)

link_directories(${ALLEGRO_LIBRARY_DIRS})

file(GLOB SRC_FILES
    src/*.cpp
)

add_executable(JogoLP1 ${SRC_FILES})

target_link_libraries(JogoLP1 ${ALLEGRO_LIBRARIES})