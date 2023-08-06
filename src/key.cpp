//
// 暗号鍵の生成
//
#include "ideaencrypt/key.hpp"

#include <iomanip>
#include <iostream>
#include <random>

namespace ideaencrypt {

IDEAKey::IDEAKey() {
    std::random_device random;
    for (size_t i = 0; i < 16; i++) {
        parts[i] = random();
    }
}

IDEAKey::IDEAKey(const std::string& phrase) {
    auto length = phrase.length();

    // フレーズがゼロ文字なら0で初期化
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

IDEAKey::IDEAKey(const uint8_t (&parts)[16]) {
    for (size_t i = 0; i < 16; i++) {
        this->parts[i] = parts[i];
    }
}

int IDEAKey::at(uint8_t index) const {
    if (index >= 16) {
        return -1;
    }
    return parts[index];
}

IDEAKey::iterator IDEAKey::subKeys() {
    return IDEAKeyIterator(*this);
}

IDEAKeyIterator::IDEAKeyIterator(const IDEAKey key) : currentIndex(0) {
    for (size_t i = 0; i < 8; i++) {
        currentKey[i] = (key.at(i * 2) << 8) | key.at(i * 2 + 1);
    }
}

void IDEAKeyIterator::rotateKey(size_t nbits) {
    auto rotateCount = nbits;

    // 1要素内に収まらないローテートの場合は要素単位でずらす
    while (rotateCount >= 16) {
        auto first = currentKey[0];
        for (size_t i = 0; i < 7; i++) {
            currentKey[i] = currentKey[i + 1];
        }
        currentKey[7] = first;
        rotateCount -= 16;
    }

    // そこで終わるなら戻る
    if (rotateCount == 0) {
        return;
    }

    // 残りは地道にやる
    auto first = currentKey[0] >> (16 - rotateCount);
    for (size_t i = 0; i < 8; i++) {
        // 上位bitを捨て、次の要素の上位ビットを下位に移動
        currentKey[i] <<= rotateCount;
        currentKey[i] |= (i < 7) ? currentKey[i + 1] >> (16 - rotateCount) : first;
    }
}

IDEAKeyIterator& IDEAKeyIterator::operator++() {
    ++currentIndex;
    if (currentIndex > 7) {
        rotateKey(25);
        currentIndex = 0;
    }
    return *this;
}

IDEAKeyIterator IDEAKeyIterator::operator++(int) {
    auto current = *this;
    this->operator++();
    return current;
}

}  // namespace ideaencrypt
