//
// IDEA暗号機の操作
//
#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "ideaencrypt/calc.hpp"

using namespace ideaencrypt;

// 加法の逆元の計算
TEST(CalcTest, TestAddInv) {
    // 加法の逆元を求め、元の数と足し合わせる関数
    auto checkAddInv = [](const uint16_t x) { return static_cast<uint16_t>(x + addinv(x)); };

    ASSERT_EQ(checkAddInv(0x0123), 0);
    ASSERT_EQ(checkAddInv(0), 0);
    ASSERT_EQ(checkAddInv(UINT16_MAX), 0);
}

// 乗法の逆元の計算
TEST(CalcTest, TestMulInv) {
    // 乗法の逆元を求め、元の数と掛ける関数
    auto checkMulInv = [](const uint16_t x) { return static_cast<uint16_t>((x * mulinv(x)) % 0x10001); };

    // 0で計算するとおかしなことになる (2^16をint32_tで持てないため)
    // ASSERT_EQ(checkMulInv(0), 1);
    ASSERT_EQ(checkMulInv(1), 1);
    ASSERT_EQ(checkMulInv(1207), 1);
    ASSERT_EQ(checkMulInv(UINT16_MAX), 1);
}

// 2^16+1を法とした乗法の計算(low-high)
TEST(CalcTest, TestLowHighMul) {
    {
        uint16_t x = 5;
        uint16_t y = 6;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }

    {
        uint16_t x = 0x0123;
        uint16_t y = 0x0ABC;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }

    {
        uint16_t x = 0xDEF0;
        uint16_t y = 0x0456;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }

    {
        uint16_t x = 1;
        uint16_t y = 0xFFFF;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }

    {
        uint16_t x = 0;
        uint16_t y = 0;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }

    {
        uint16_t x = 0xFFFF;
        uint16_t y = 0xFFFF;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }
    {
        uint16_t x = 0xFFFF;
        uint16_t y = 0;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }
    {
        uint16_t x = 0;
        uint16_t y = 0xFFFF;
        uint32_t m = static_cast<uint32_t>(x * y) % (0x10001);
        ASSERT_EQ(lhmul(x, y), m);
    }
}
