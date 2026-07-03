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
    allegro_audio-5
    allegro_acodec-5
    allegro_main-5
)

include_directories(
    include
    ${ALLEGRO_INCLUDE_DIRS}
)

if(WIN32)
    add_compile_definitions(_POSIX_C_SOURCE=200809L)
    add_compile_definitions(__STDC_WANT_LIB_EXT1__=1)
endif()

link_directories(${ALLEGRO_LIBRARY_DIRS})

file(GLOB SRC_FILES
    src/*.cpp
)

add_executable(JogoLP1 ${SRC_FILES})

target_link_libraries(JogoLP1 ${ALLEGRO_LIBRARIES})