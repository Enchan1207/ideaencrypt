//
// IDEA暗号機の操作
//
#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "ideaencrypt/cipher.hpp"

using namespace ideaencrypt;

void dumpStringAsHex(const std::string& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(static_cast<unsigned char>(str[i])) << " ";
        if ((i + 1) % 16 == 0) {
            std::cout << std::endl;
        }
    }
    if (str.size() < 16) {
        std::cout << std::endl;
    }
}

// 暗号ブロックの構成
TEST(CipherTest, TestConfigureEncodeBlock) {
    // IDEAと同じブロックを構成
    CipherStream::CipherBlockList blocks;
    for (size_t i = 0; i < 8; i++) {
        blocks.push_back(std::make_shared<CipherFullBlock>(CipherFullBlock()));
    }
    blocks.push_back(std::make_shared<CipherHalfBlock>(CipherHalfBlock()));

    // 鍵を適用
    IDEAKey key("fujisawayoshinori");
    auto iter = key.subKeys();
    for (const auto& block : blocks) {
        block->applyKeyForEncode(iter);
    }

    // 検証
    std::vector<std::string> expected = {
        "6675 6a69 7361 7761 796f 7368 ",
        "696e 6f72 d2e6 c2ee c2f2 dee6 ",
        "d0d2 dcde e4cc ead4 dd85 e5bd ",
        "cda1 a5b9 bdc9 99d5 a9a5 cd85 ",
        "7b9b 434b 737b 9333 ab53 4b9b ",
        "0bbb 0bcb 96e6 f726 6756 a697 ",
        "3617 7617 96f7 3686 4cce ad4d ",
        "2e6c 2eec 2f2d ee6d 0d2d cdee ",
        "9a5c d85d d85e 5bdc "};

    bool succeeded = true;
    for (size_t i = 0; i < blocks.size(); i++) {
        auto stored = blocks[i]->dumpCipherBlock();
        auto expect = expected[i];

        int correct = 1;
        for (size_t i = 0; i < stored.size(); i++) {
            auto s = stored[i];
            auto e = expect[i];
            correct *= s == e;
        }
        if (correct != 1) {
            std::cout << "INCOMPLIANT: " << stored << " != " << expect << std::endl;
            succeeded = false;
        }
    }
    EXPECT_TRUE(succeeded);
}

// 復号ブロックの構成
TEST(CipherTest, TestConfigureDecodeBlock) {
    // IDEAと同じブロックを構成
    CipherStream::CipherBlockList blocks;
    for (size_t i = 0; i < 8; i++) {
        blocks.push_back(std::make_shared<CipherFullBlock>(CipherFullBlock()));
    }
    blocks.push_back(std::make_shared<CipherHalfBlock>(CipherHalfBlock()));

    // 鍵を適用
    IDEAKey key("fujisawayoshinori");
    auto iter = key.subKeys();
    for (auto block = blocks.rbegin(); block != blocks.rend(); ++block) {
        auto dist = std::distance(block, blocks.rend());
        (*block)->applyKeyForDecode(iter, dist);
    }

    // 検証
    std::vector<std::string> expected = {
        "c2b6 27a3 27a2 759b 0d2d cdee ",
        "6fe9 d0d3 d114 c594 4cce ad4d ",
        "be72 6909 89e9 5190 6756 a697 ",
        "2fe9 691a f435 013e ab53 4b9b ",
        "27b1 8c85 bcb5 18c5 a9a5 cd85 ",
        "aa44 4237 5a47 3360 dd85 e5bd ",
        "6fac 1b34 2322 b594 c2f2 dee6 ",
        "b0ae 2d1a 908e be3b 796f 7368 ",
        "bb7f 9597 8c9f 823c "};

    bool succeeded = true;
    for (size_t i = 0; i < blocks.size(); i++) {
        auto stored = blocks[i]->dumpCipherBlock();
        auto expect = expected[i];

        int correct = 1;
        for (size_t i = 0; i < stored.size(); i++) {
            auto s = stored[i];
            auto e = expect[i];
            correct *= s == e;
        }
        if (correct != 1) {
            std::cout << "INCOMPLIANT: " << stored << " != " << expect << std::endl;
            succeeded = false;
        }
    }
    EXPECT_TRUE(succeeded);
}

// 既知のメッセージの暗号化/復号
TEST(CipherTest, TestProcessKnownMessage) {
    IDEAKey key("fujisawayoshinori");
    CipherStream stream(key);

    // 平文を構成する サンプルコードでは2byteのブロックに1文字を入れているので、char(0)でパディングする
    std::ostringstream contentStream;
    std::string message = "fujisawa";
    for (size_t i = 0; i < message.length(); i++) {
        contentStream << char(0) << message[i];
    }
    std::string content = contentStream.str();
    std::cout << "Message  : ";
    dumpStringAsHex(content);

    // 暗号ストリームに投げる
    stream << content;

    // 結果を取り出す
    std::string encrypted;
    stream >> encrypted;

    const uint8_t expectedCipher[16] = {
        0x54, 0x6b, 0x9c, 0x24, 0xdc, 0x2d, 0xcf, 0x48,
        0x21, 0xb8, 0x36, 0x99, 0x97, 0x76, 0x3a, 0x41};
    std::cout << "Encrypted: ";
    dumpStringAsHex(encrypted);
    EXPECT_EQ(encrypted.length(), 16);
    for (size_t i = 0; i < encrypted.length(); i++) {
        EXPECT_EQ(static_cast<uint8_t>(expectedCipher[i]), static_cast<uint8_t>(encrypted[i]));
    }

    // 暗号機をデコード用に再構成し、ストリームに挿入
    stream.configureForDecode();
    stream << encrypted;

    // 結果を取り出す
    std::string decrypted;
    stream >> decrypted;

    // 比較する
    std::cout << "Decrypted: ";
    dumpStringAsHex(decrypted);
    EXPECT_EQ(decrypted.length(), 16);
    for (size_t i = 0; i < decrypted.length(); i++) {
        EXPECT_EQ(static_cast<uint8_t>(content[i]), static_cast<uint8_t>(decrypted[i]));
    }
    std::cout << "Message  : " << content << std::endl;
    std::cout << "Decrypted: " << decrypted << std::endl;
}

// 任意のメッセージの暗号化/復号
TEST(CipherTest, TestProcessMessages) {
    // 暗号ストリームを構成
    const IDEAKey key("encryptionkey");
    CipherStream stream(key);

    // ストリームに挿入して暗号文を入手
    std::string message = "ideacrypideacrypcryptestcryptestidea";
    stream << message;
    std::string encrypted;
    stream >> encrypted;

    // 復号用にストリームを再構成して挿入し、復号結果を入手
    stream.configureForDecode();
    stream << encrypted;
    std::string decrypted;
    stream >> decrypted;

    // 確認
    std::cout << "Message : " << std::endl;
    dumpStringAsHex(message);
    std::cout << std::endl;
    std::cout << "Encrypted: " << std::endl;
    dumpStringAsHex(encrypted);
    std::cout << std::endl;
    std::cout << "Decrypted: " << std::endl;
    dumpStringAsHex(decrypted);
    std::cout << std::endl;

    // 検証
    for (size_t i = 0; i < decrypted.length(); i++) {
        EXPECT_EQ(static_cast<uint8_t>(message[i]), static_cast<uint8_t>(decrypted[i]));
    }
}
