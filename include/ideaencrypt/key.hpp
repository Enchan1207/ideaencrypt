//
// 暗号鍵の生成
//

#ifndef IDEAENCRYPT_KEY_H
#define IDEAENCRYPT_KEY_H

#include <cstdint>
#include <string>

namespace ideaencrypt {

class IDEAKeyIterator;

/**
 * @brief IDEA暗号の副鍵
 */
using SubKey = uint16_t;

/**
 * @brief IDEA暗号鍵 (128bit)
 */
class IDEAKey final {
    friend IDEAKeyIterator;

   private:
    /**
     * @brief 鍵格納領域
     */
    SubKey parts[8];

   public:
    typedef IDEAKeyIterator iterator;

    /**
     * @brief ランダムな値から鍵を生成
     */
    IDEAKey();

    /**
     * @brief フレーズから鍵を生成
     *
     * @param phrase 生成元の文字列
     * @note 引数phraseはnullを除いて16文字であることが望ましく、
     *       それ以上であれば切り捨て、それ以下であれば同じ文字列を繰り返し参照します。
     *       (例: `tooolooongphrasee` -> `tooolooongphrase`, `short` -> `shortshortshorts`)
     */
    IDEAKey(const std::string& phrase);

    /**
     * @brief 配列から鍵を生成
     *
     * @param parts 生成元となる数列
     * @note 引数partsの要素数は8である必要があります。
     */
    IDEAKey(const SubKey (&parts)[8]);

    /**
     * @brief 副鍵イテレータを取得
     *
     * @return IDEAKey::iterator 生成された副鍵イテレータ
     */
    IDEAKey::iterator subKeys();
};

/**
 * @brief 鍵イテレータ
 */
class IDEAKeyIterator final {
    friend IDEAKey;

   private:
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

    /**
     * @brief 鍵をもとに副鍵イテレータを初期化
     *
     * @param key
     */
    IDEAKeyIterator(const IDEAKey key);

   public:
    IDEAKeyIterator& operator++();

    IDEAKeyIterator operator++(int);

    const SubKey& operator*();

    /**
     * @brief イテレータを進め、値を返す
     *
     * @return const SubKey& 副鍵
     */
    const SubKey next();
};

}  // namespace ideaencrypt

#endif /* IDEAENCRYPT_KEY_H */
