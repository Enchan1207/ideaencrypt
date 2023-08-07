//
// 計算処理
//

#include "ideaencrypt/calc.hpp"

namespace ideaencrypt {

uint16_t addinv(const uint16_t& number) {
    return -number;
}

uint16_t mulinv(const uint16_t& number) {
    // 乗法の逆元は「掛けて1になる数」 つまり a に対して ax \equiv 1 (mod 2^16) な数
    // ax + 2^16y = 1 な x がこの解となる -> 拡張ユークリッドの互除法による計算
    int32_t a = number;
    int32_t b = 0x10001;
    int32_t x = 1, y = 0, x1 = 0, y1 = 1;

    while (b != 0) {
        // 通常のユークリッドの互除法でGCDを計算していく 最終的にaにGCDが入る
        int32_t q = a / b;
        int32_t r = a % b;
        a = b;
        b = r;

        // 係数を更新する
        int32_t tmp_x = x - q * x1;
        int32_t tmp_y = y - q * y1;
        x = x1;
        y = y1;
        x1 = tmp_x;
        y1 = tmp_y;
    }

    // xが求めたい解となる マイナスなら法を足す
    auto result = x % 0x10001;
    if (result < 0) {
        result += 0x10001;
    }
    return result;
}

uint16_t lhmul(const uint16_t& x, const uint16_t& y) {
    uint32_t m = x * y;
    uint16_t m_l = m & 0xFFFF;
    uint16_t m_h = m >> 16;

    int32_t sub = m_l - m_h;
    sub = (sub < 0) ? sub + 0x10001 : sub;
    return static_cast<uint16_t>(sub);
}

}  // namespace ideaencrypt
