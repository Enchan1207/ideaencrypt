#
# ideaencrypt tests
#
cmake_minimum_required(VERSION 3.21)

# GoogleTestのフェッチと有効化
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# テストターゲットの構成
add_executable(ideaencrypt_test)
target_sources(ideaencrypt_test PRIVATE
    test_keygen.cpp
    test_cipher.cpp
    test_calc.cpp
)
target_link_libraries(ideaencrypt_test PRIVATE
    ideaencrypt
    GTest::gtest_main
)
target_include_directories(ideaencrypt_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

# テストの実行
gtest_discover_tests(ideaencrypt_test)
