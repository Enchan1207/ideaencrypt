#
# ideaencrypt cli
#

cmake_minimum_required(VERSION 3.21)

add_executable(ideaencrypt_cli)
target_sources(ideaencrypt_cli PRIVATE
    main.cpp
)
target_include_directories(ideaencrypt_cli PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(ideaencrypt_cli PRIVATE
    ideaencrypt
)
target_compile_options(ideaencrypt_cli PRIVATE
    -Wall
)