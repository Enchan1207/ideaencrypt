//
// 暗号ブロック
//

#ifndef IDEAENCRYPT_BLOCK_H
#define IDEAENCRYPT_BLOCK_H

#include <cstdint>
#include <string>

#include "ideaencrypt/key.hpp"

namespace ideaencrypt {

/**
 * @brief IDEA暗号ブロック(インタフェース)
 */
class AbstractCipherBlock {
   public:
    virtual void code(const uint16_t (&input)[4], uint16_t (&output)[4]) const = 0;

    /**
     * @brief 暗号鍵イテレータを用いて、暗号化用に鍵を適用する
     *
     * @param iterator 暗号鍵イテレータ
     */
    virtual void applyKeyForEncode(IDEAKey::iterator& iterator) = 0;

    /**
     * @brief 暗号鍵イテレータを用いて、復号用に鍵を適用する
     *
     * @param iterator 暗号鍵イテレータ
     * @param step 暗号ブロックのどこに位置しているか
     */
    virtual void applyKeyForDecode(IDEAKey::iterator& iterator, const size_t& step) = 0;

    virtual std::string dumpCipherBlock() = 0;

    virtual ~AbstractCipherBlock() = default;
};

/**
 * @brief フルブロック
 *
 * @note IDEA暗号の基本構造のうち全ての処理を行います。
 */
class CipherFullBlock : public AbstractCipherBlock {
   private:
    /**
     * @brief 副鍵配列
     */
    SubKey subKeys[6] = {0};

   public:
    void code(const uint16_t (&input)[4], uint16_t (&output)[4]) const override;
    void applyKeyForEncode(IDEAKey::iterator& iterator) override;
    void applyKeyForDecode(IDEAKey::iterator& iterator, const size_t& step) override;
    std::string dumpCipherBlock() override;
};

/**
 * @brief ハーフブロック
 *
 * @note IDEA暗号の基本構造のうち、XORを計算する処理まで行います。
 */
class CipherHalfBlock : public AbstractCipherBlock {
   private:
    /**
     * @brief 副鍵配列
     */
    SubKey subKeys[4] = {0};

   public:
    void code(const uint16_t (&input)[4], uint16_t (&output)[4]) const override;
    void applyKeyForEncode(IDEAKey::iterator& iterator) override;
    void applyKeyForDecode(IDEAKey::iterator& iterator, const size_t& step) override;
    std::string dumpCipherBlock() override;
};

}  // namespace ideaencrypt

#endif /* IDEAENCRYPT_BLOCK_H */
