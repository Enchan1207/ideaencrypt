//
// IDEA暗号鍵の生成
//
#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "ideaencrypt/key.hpp"

using namespace ideaencrypt;

// 鍵の生成
TEST(KeyGenerationTest, TestGenerateKey) {
    // 乱数で初期化
    auto randomKey = IDEAKey();
    auto randomSubKey = randomKey.subKeys();
    for (size_t i = 0; i < 8; i++, randomSubKey++) {
        std::cout << std::hex << std::setw(4) << std::setfill('0') << *randomSubKey << " ";
    }
    std::cout << std::endl;

    // フレーズをもとに初期化
    std::string shortPhrase = "short";
    auto shortPhraseKey = IDEAKey(shortPhrase);
    auto shortPhraseSubKey = shortPhraseKey.subKeys();
    for (size_t i = 0; i < 8; i++, shortPhraseSubKey++) {
        auto phraseLen = shortPhrase.length();
        uint8_t chr1 = shortPhrase[(i * 2) % phraseLen];
        uint8_t chr2 = shortPhrase[(i * 2 + 1) % phraseLen];
        uint16_t chrs = chr1 << 8 | chr2;

        EXPECT_EQ(*shortPhraseSubKey, chrs);
    }

    std::string longPhrase = "tooolooongphrasee";
    auto longPhraseKey = IDEAKey(longPhrase);
    auto longPhraseSubkey = longPhraseKey.subKeys();
    for (size_t i = 0; i < 8; i++, longPhraseSubkey++) {
        auto phraseLen = longPhrase.length();
        uint8_t chr1 = longPhrase[(i * 2) % phraseLen];
        uint8_t chr2 = longPhrase[(i * 2 + 1) % phraseLen];
        uint16_t chrs = chr1 << 8 | chr2;

        EXPECT_EQ(*longPhraseSubkey, chrs);
    }

    // フレーズが空なら全ビット0
    auto zeroKey = IDEAKey("");
    auto zeroSubKey = zeroKey.subKeys();
    for (size_t i = 0; i < 16; i++, zeroSubKey++) {
        EXPECT_EQ(*zeroSubKey, 0x00);
    }

    // 即値で初期化
    const uint16_t keyConstants[] = {0x1234, 0x5678, 0x90ab, 0xcdef, 0x2143, 0x6587, 0x09ba, 0xdcfe};
    auto constKey = IDEAKey(keyConstants);
    auto constSubKey = constKey.subKeys();
    for (size_t i = 0; i < 8; i++, constSubKey++) {
        EXPECT_EQ(*constSubKey, keyConstants[i]);
    }
}

// 副鍵の生成
TEST(KeyGenerationTest, TestGenerateSubKeys) {
    IDEAKey key({0, 2, 0, 3, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    auto iter = key.subKeys();

    // 一巡目
    EXPECT_EQ(*(iter++), 0x0002);
    EXPECT_EQ(*(iter++), 0x0003);
    EXPECT_EQ(*(iter++), 0x0005);
    for (size_t i = 0; i < 4; i++) {
        EXPECT_EQ(*(iter++), 0x0000);
    }
    EXPECT_EQ(*(iter++), 0x0001);

    // 二巡目 このタイミングで25ビット左にローテートされるはず
    // 0x0002 0003 0005 0000 0000 0000 0000 0001 から
    // 0x0600 0A00 0000 0000 0000 0000 0200 0400 となり、
    // 末尾にいた0x0001が副鍵の7つ目に0x0200=512となって現れる
    EXPECT_EQ(*(iter++), 0x0600);
    EXPECT_EQ(*(iter++), 0x0A00);
    for (size_t i = 0; i < 4; i++) {
        EXPECT_EQ(*(iter++), 0x0000);
    }
    EXPECT_EQ(*(iter++), 0x0200);
    EXPECT_EQ(*(iter++), 0x0400);
}

// フレーズベースの副鍵の生成
TEST(KeyGenerationTest, TestGenerateSubKeyFromPhrase) {
    IDEAKey phraseKey("fujisawayoshinori");  // マジックナンバー
    auto iter = phraseKey.subKeys();
    SubKey expected[52] = {
        0x6675, 0x6a69, 0x7361, 0x7761, 0x796f, 0x7368, 0x696e, 0x6f72,
        0xd2e6, 0xc2ee, 0xc2f2, 0xdee6, 0xd0d2, 0xdcde, 0xe4cc, 0xead4,
        0xdd85, 0xe5bd, 0xcda1, 0xa5b9, 0xbdc9, 0x99d5, 0xa9a5, 0xcd85,
        0x7b9b, 0x434b, 0x737b, 0x9333, 0xab53, 0x4b9b, 0x0bbb, 0x0bcb,
        0x96e6, 0xf726, 0x6756, 0xa697, 0x3617, 0x7617, 0x96f7, 0x3686,
        0x4cce, 0xad4d, 0x2e6c, 0x2eec, 0x2f2d, 0xee6d, 0x0d2d, 0xcdee,
        0x9a5c, 0xd85d, 0xd85e, 0x5bdc};
    for (size_t i = 0; i < 52; i++) {
        EXPECT_EQ(expected[i], *(iter++));
    }
}
