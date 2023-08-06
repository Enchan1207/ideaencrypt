//
// 計算処理
//

#include "ideaencrypt/calc.hpp"

namespace ideaencrypt {

// TODO: まじめにやる

uint16_t addinv(const uint16_t& number) {
    return UINT16_MAX - number;
}

uint16_t mulinv(const uint16_t& number) {
    return number;
}

uint16_t mul(const uint16_t& x, const uint16_t& y) {
    return x * y;
}

}  // namespace ideaencrypt
