//
// IDEA暗号鍵の生成
//
#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "ideaencrypt/key.hpp"

using namespace ideaencrypt;

// 主鍵の生成
TEST(KeyGenerationTest, TestGenerateMainKeys) {
    // 乱数で初期化
    auto randomKey = MainKey();
    for (size_t i = 0; i < 16; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << randomKey.at(i) << " ";
    }
    std::cout << std::endl;

    // フレーズをもとに初期化
    std::string shortPhrase = "short";
    auto shortPhraseKey = MainKey(shortPhrase);
    for (size_t i = 0; i < 16; i++) {
        EXPECT_EQ(shortPhraseKey.at(i), shortPhrase[i % shortPhrase.length()]);
    }
    std::string longPhrase = "tooolooongphrasee";
    auto longPhraseKey = MainKey(longPhrase);
    for (size_t i = 0; i < 16; i++) {
        EXPECT_EQ(longPhraseKey.at(i), longPhrase[i]);
    }

    // フレーズが空なら全ビット0
    auto zeroKey = MainKey("");
    for (size_t i = 0; i < 16; i++) {
        EXPECT_EQ(zeroKey.at(i), 0x00);
    }
}