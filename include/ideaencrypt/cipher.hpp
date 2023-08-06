//
// 暗号機
//

#ifndef IDEAENCRYPT_CIPHER_H
#define IDEAENCRYPT_CIPHER_H

#include <list>
#include <sstream>

#include "ideaencrypt/block.hpp"

namespace ideaencrypt {

/**
 * @brief 暗号ストリーム
 */
class CipherStream final {
   private:
    // 内部バッファ
    std::ostringstream streamBuffer;

    using CipherBlockList = std::list<std::shared_ptr<AbstractCipherBlock>>;

    // 暗号ブロック配列
    CipherBlockList cipherBlocks;

   public:
    /**
     * @brief 暗号ブロック配列をもとに暗号ストリームを初期化
     *
     * @param blocks 暗号ブロックのリスト
     */
    CipherStream(CipherBlockList blocks);

    /**
     * @brief IDEA本来の構造で暗号ストリームを初期化
     *
     * @note 暗号ストリームは フルブロック8段 + ハーフブロック1段 の構造で構成されます。
     */
    CipherStream();

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
    CipherStream& operator<<(int element);
    CipherStream& operator<<(double element);

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
};

}  // namespace ideaencrypt

#endif /* IDEAENCRYPT_CIPHER_H */
