#
# ideaencrypt sources
#

cmake_minimum_required(VERSION 3.21)

## 外部依存関係を追加...

add_library(ideaencrypt)
target_sources(ideaencrypt PRIVATE
    ideaencrypt.cpp
    key.cpp
)
target_include_directories(ideaencrypt PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(ideaencrypt PRIVATE
    -Wall
)