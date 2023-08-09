#
# CLI
#

cmake_minimum_required(VERSION 3.21)

# libparseroptのフェッチと有効化
include(FetchContent)
FetchContent_Declare(
    libparseropt
    GIT_REPOSITORY https://github.com/GrapeJuicer/libparseropt
    GIT_TAG release-v1.4.0
)
FetchContent_MakeAvailable(libparseropt)

macro(add_ideaencrypt_cli executable_name)
    add_executable(${executable_name})
    target_sources(${executable_name} PRIVATE
        ${executable_name}.cpp
        argparse.cpp
    )
    target_include_directories(${executable_name} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )
    target_link_libraries(${executable_name} PRIVATE
        ideaencrypt
        parseropt
    )
    target_compile_options(${executable_name} PRIVATE
        -Wall
    )
endmacro()

add_ideaencrypt_cli(ideakeygen)
add_ideaencrypt_cli(idea)
