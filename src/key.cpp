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

MainKey::MainKey(const uint8_t (&parts)[16]) {
    for (size_t i = 0; i < 16; i++) {
        this->parts[i] = parts[i];
    }
}

int MainKey::at(uint8_t index) const {
    if (index >= 16) {
        return -1;
    }
    return parts[index];
}

void SubKeyIterator::rotateKey(size_t nbits) {
    for (size_t i = 0; i < 25; i++) {
        // 最上位を取り出しておく
        auto tmp = currentKey[0] >> 15;
        for (size_t i = 0; i < 8; i++) {
            // 上位bitを捨て、次の要素の最上位ビットを最下位に移動
            currentKey[i] <<= 1;
            currentKey[i] |= (i < 7) ? currentKey[i + 1] >> 15 : tmp;
        }
    }
}

void SubKeyIterator::reset() {
    currentIndex = 0;
    for (size_t i = 0; i < 8; i++) {
        currentKey[i] = (mainKey.at(i * 2) << 8) | mainKey.at(i * 2 + 1);
    }
}

SubKey SubKeyIterator::next() {
    // 鍵を使い切ったら左に25bitローテートし、インデックスを戻す
    if (currentIndex > 7) {
        rotateKey(25);
        currentIndex = 0;
    }
    return currentKey[currentIndex++];
}

}  // namespace ideaencrypt
