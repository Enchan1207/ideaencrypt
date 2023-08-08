#
# ideaencrypt
#
cmake_minimum_required(VERSION 3.21)
project(ideaencrypt
    VERSION 0.1.0
    DESCRIPTION "C++ implementation of IDEA(International Data Encryption Algorithm)"
    LANGUAGES C CXX
)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ライブラリソースディレクトリ
add_subdirectory(src)

# CLIソースディレクトリ
add_subdirectory(cli)

# 依存関係としてビルドされている場合は、tests/ を除外する
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(IDEAENCRYPT_BUILD_TESTS "whether to build tests" ON)

    if(IDEAENCRYPT_BUILD_TESTS)
        message(NOTICE "testing enabled")
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()
