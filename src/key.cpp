//
// 暗号鍵の生成
//
#include "ideaencrypt/key.hpp"

#include <random>

namespace ideaencrypt {

MainKey::MainKey() {
    std::random_device random;
    for (size_t i = 0; i < 16; i++) {
        parts[i] = random();
    }
}

MainKey::MainKey(const std::string& phrase) {
    auto length = phrase.length();

    // ゼロ文字なら主鍵は0で初期化
    if (length == 0) {
        for (size_t i = 0; i < 16; i++) {
            parts[i] = 0x00;
        }
        return;
    }

    // 1文字ずつ取り出して割り当て
    for (size_t i = 0; i < 16; i++) {
        parts[i] = phrase[i % length];
    }
}

int MainKey::at(uint8_t index) {
    if (index >= 16) {
        return -1;
    }
    return parts[index];
}

}  // namespace ideaencrypt
