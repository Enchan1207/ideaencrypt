//
// 暗号鍵の生成
//

#ifndef IDEAENCRYPT_KEY_H
#define IDEAENCRYPT_KEY_H

#include <cstdint>
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
     * @brief 配列から主鍵を生成
     *
     * @param parts 生成元となる数列
     * @note 引数partsの要素数は16である必要があります。
     */
    MainKey(const uint8_t (&parts)[16]);

    /**
     * @brief 主鍵の一部を取り出す
     *
     * @param index 取り出す位置(byte単位, 0~15)
     * @return int 結果
     *
     * @note 範囲外のインデックスが渡された場合は-1が返ります。
     */
    int at(uint8_t index) const;
};

/**
 * @brief IDEA暗号の副鍵
 */
using SubKey = uint16_t;

/**
 * @brief 副鍵イテレータ
 */
class SubKeyIterator final {
   private:
    /**
     * @brief 副鍵のもとになる主鍵
     */
    const MainKey mainKey;

    /**
     * @brief 現在参照している副鍵配列
     */
    SubKey currentKey[8] = {0};

    /**
     * @brief 副鍵配列の参照位置
     */
    uint8_t currentIndex = 0;

    /**
     * @brief 鍵を左ローテートする
     *
     * @param nbits 移動量
     */
    void rotateKey(size_t nbits);

   public:
    /**
     * @brief 主鍵をもとに副鍵イテレータを初期化
     *
     * @param mainKey
     */
    SubKeyIterator(const MainKey mainKey) : mainKey(mainKey) {
        reset();
    }

    /**
     * @brief イテレータを初期状態に戻す
     *
     * @note ローテートの状態もリセットされます。
     */
    void reset();

    /**
     * @brief 副鍵を取り出す
     *
     * @return SubKey 主鍵から取り出した副鍵
     */
    SubKey next();
};

}  // namespace ideaencrypt

#endif /* IDEAENCRYPT_KEY_H */
