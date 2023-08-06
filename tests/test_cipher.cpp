//
// IDEA暗号機の操作
//
#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "ideaencrypt/cipher.hpp"

using namespace ideaencrypt;

// 暗号機の構成
TEST(CipherTest, TestInstantiate) {
    CipherStream stream;

    stream << "Hello, World!"
           << "cipher streaming!";
    stream << std::hex << std::setw(4) << std::setfill('0');
    stream << 0xAE << std::endl;
    stream << std::dec;
    stream << 3.1415;
    stream << std::endl;
    stream << "continuous!";

    std::string result;
    stream >> result;
    std::cout << result << std::endl;
}
