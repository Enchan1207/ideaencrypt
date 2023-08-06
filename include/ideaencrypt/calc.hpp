//
// 計算処理
//

#ifndef IDEAENCRYPT_CALC_H
#define IDEAENCRYPT_CALC_H

#include <cstdint>

namespace ideaencrypt {

/**
 * @brief 2^16を法とした加法の逆元を計算する
 *
 * @param number
 * @return uint16_t
 */
uint16_t addinv(const uint16_t& number);

/**
 * @brief 2^16を法とした乗法の逆元を計算する
 *
 * @param number
 * @return uint16_t
 */
uint16_t mulinv(const uint16_t& number);

/**
 * @brief 2^16+1を法とした乗算を計算する
 *
 * @param x
 * @param y
 * @return uint16_t
 *
 * @note Low-Highアルゴリズムにより演算を行います。
 */
uint16_t mul(const uint16_t& x, const uint16_t& y);

}  // namespace ideaencrypt

#endif /* IDEAENCRYPT_CALC_H */
