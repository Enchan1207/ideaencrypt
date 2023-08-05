//
// 暗号鍵の生成
//

#ifndef IDEAENCRYPT_KEY_H
#define IDEAENCRYPT_KEY_H

#include <cstdint>
#include <iterator>
#include <string>

namespace ideaencrypt {

/**
 * @brief IDEA暗号の主鍵 (128bit)
 */
class MainKey final {
   private:
    /**
     * @brief 鍵格納領域
     */
    uint8_t parts[16];

   public:
    /**
     * @brief ランダムな値から主鍵を生成
     */
    MainKey();

    /**
     * @brief フレーズから主鍵を生成
     *
     * @param phrase 生成元の文字列
     * @note 引数phraseはnullを除いて16文字であることが望ましく、
     *       それ以上であれば切り捨て、それ以下であれば同じ文字列を繰り返し参照します。
     *       (例: `tooolooongphrasee` -> `tooolooongphrase`, `short` -> `shortshortshorts`)
     */
    MainKey(const std::string& phrase);

    /**
     * @brief 主鍵の一部を取り出す
     *
     * @param index 取り出す位置(byte単位, 0~15)
     * @return int 結果
     *
     * @note 範囲外のインデックスが渡された場合は-1が返ります。
     */
    int at(uint8_t index);
};

/**
 * @brief IDEA暗号の副鍵
 */
using SubKey = uint16_t;

}  // namespace ideaencrypt

#endif /* IDEAENCRYPT_KEY_H */
