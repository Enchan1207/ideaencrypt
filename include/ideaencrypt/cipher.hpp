//
// 暗号機
//

#ifndef IDEAENCRYPT_CIPHER_H
#define IDEAENCRYPT_CIPHER_H

#include <memory>
#include <sstream>
#include <vector>

#include "ideaencrypt/block.hpp"

namespace ideaencrypt {

/**
 * @brief 暗号ストリーム
 */
class CipherStream final {
   public:
    using CipherBlockList = std::vector<std::shared_ptr<AbstractCipherBlock>>;

   private:
    // 暗号鍵
    const IDEAKey& key;

    // 内部バッファ
    std::ostringstream streamBuffer;

    // 暗号ブロック配列
    CipherBlockList cipherBlocks;

    /**
     * @brief 暗号ブロック配列にデータを通す
     *
     * @param input 入力
     * @param output 暗号ブロック配列に通した結果
     */
    void code(const uint16_t (&input)[4], uint16_t (&output)[4]) const;

   public:
    /**
     * @brief 暗号ブロック配列をもとに暗号ストリームを初期化
     *
     * @param key 暗号鍵
     * @param blocks 暗号ブロックのリスト
     */
    CipherStream(const IDEAKey& key, CipherBlockList blocks);

    /**
     * @brief IDEA本来の構造で暗号ストリームを初期化
     *
     * @note 暗号ストリームは フルブロック8段 + ハーフブロック1段 の構造で構成されます。
     */
    CipherStream(const IDEAKey& key);

    /**
     * @brief 暗号機を暗号化用に構成する
     *
     * @note コンストラクタを呼び出す際に自動で呼び出されます。
     */
    void configureForEncode();

    /**
     * @brief 暗号機を復号用に構成する
     */
    void configureForDecode();

    /**
     * @brief 符号化ストリームへの挿入
     *
     * @param element
     * @return CipherStream&
     */
    template <typename T>
    CipherStream& operator<<(const T& element) {
        streamBuffer << element;
        return *this;
    }

    /**
     * @brief 符号化ストリームへのテンプレート関数の挿入
     *
     * @param f
     * @return CipherStream&
     *
     * @note std::endlの挿入に使用します。
     */
    CipherStream& operator<<(std::ostream& (*f)(std::ostream&));

    /**
     * @brief 符号化結果の抽出
     *
     * @param data
     * @return std::string
     */
    std::string operator>>(std::string& data);

    /**
     * @brief 暗号ストリームのシリアライズ
     *
     * @return std::string
     * @note 暗号ブロックの構造のみシリアライズされます(鍵は保存されません)。
     */
    std::string serialize() const;

    /**
     * @brief 暗号ストリームのデシリアライズ
     * @param serialized
     * @param deserialized
     * @return bool
     */
    static bool deserialize(const std::string& serialized, CipherStream& deserialized);
};

}  // namespace ideaencrypt

#endif /* IDEAENCRYPT_CIPHER_H */
