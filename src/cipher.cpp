//
// 暗号機
//

#include "ideaencrypt/cipher.hpp"

#include <string>

namespace ideaencrypt {

CipherStream::CipherStream(const IDEAKey& key, CipherBlockList blocks) : key(key), streamBuffer(""), cipherBlocks(blocks) {
    configureForEncode();
}

CipherStream::CipherStream(const IDEAKey& key) : key(key), streamBuffer() {
    for (size_t i = 0; i < 8; i++) {
        cipherBlocks.push_back(std::make_shared<CipherFullBlock>(CipherFullBlock()));
    }
    cipherBlocks.push_back(std::make_shared<CipherHalfBlock>(CipherHalfBlock()));
    configureForEncode();
}

void CipherStream::configureForEncode() {
    auto iter = key.subKeys();
    for (const auto& block : cipherBlocks) {
        block->applyKeyForEncode(iter);
    }
}

void CipherStream::configureForDecode() {
    // 復号の際は鍵を逆順に入れる
    auto iter = key.subKeys();
    for (auto block = cipherBlocks.rbegin(); block != cipherBlocks.rend(); ++block) {
        auto dist = std::distance(block, cipherBlocks.rend());
        (*block)->applyKeyForDecode(iter, dist);
    }
}

void CipherStream::code(const uint16_t (&input)[4], uint16_t (&output)[4]) const {
    // 出力先に入力をコピー
    for (size_t i = 0; i < 4; i++) {
        output[i] = input[i];
    }

    for (const auto& block : cipherBlocks) {
        uint16_t prevOutputs[4] = {0};
        block->code(output, prevOutputs);

        // 出力をバッファにコピー
        for (size_t i = 0; i < 4; i++) {
            output[i] = prevOutputs[i];
        }
    }
}

CipherStream& CipherStream::operator<<(std::ostream& (*f)(std::ostream&)) {
    f(streamBuffer);
    return *this;
}

std::string CipherStream::operator>>(std::string& data) {
    auto bufferedContent = streamBuffer.str();
    for (size_t i = 0; i < bufferedContent.length(); i += 8) {
        // 8文字ずつ取り出す
        auto part = bufferedContent.substr(i, 8);
        part.resize(8, '\0');

        // uint16_tの配列に変換する
        uint16_t inputBuffer[4] = {0};
        for (size_t i = 0; i < 4; i++) {
            auto hs = static_cast<uint8_t>(part[i * 2]);
            auto ls = static_cast<uint8_t>(part[i * 2 + 1]);
            inputBuffer[i] = (hs << 8) | ls;
        }

        // 符号化
        uint16_t output[4] = {0};
        code(inputBuffer, output);

        // 出力
        for (size_t i = 0; i < 4; i++) {
            data += output[i] >> 8;
            data += output[i] & 0xFF;
        }
    }

    // バッファをクリア
    streamBuffer.str(std::string());

    return data;
}

}  // namespace ideaencrypt
