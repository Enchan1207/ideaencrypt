//
// 暗号鍵の生成
//
#include "ideaencrypt/key.hpp"

#include <iomanip>
#include <random>

namespace ideaencrypt {

IDEAKey::IDEAKey() {
    std::random_device random;
    for (size_t i = 0; i < 8; i++) {
        parts[i] = random();
    }
}

IDEAKey::IDEAKey(const std::string& phrase) {
    auto length = phrase.length();

    // フレーズがゼロ文字なら0で初期化
    if (length == 0) {
        for (size_t i = 0; i < 8; i++) {
            parts[i] = 0x0000;
        }
        return;
    }

    // 2文字ずつ取り出して割り当て
    for (size_t i = 0; i < 8; i++) {
        parts[i] = (phrase[(i * 2) % length] << 8) | phrase[(i * 2 + 1) % length];
    }
}

IDEAKey::IDEAKey(const SubKey (&parts)[8]) {
    for (size_t i = 0; i < 8; i++) {
        this->parts[i] = parts[i];
    }
}

IDEAKey::iterator IDEAKey::subKeys() const {
    return IDEAKeyIterator(*this);
}

std::string IDEAKey::serialize() const {
    std::stringstream ss;
    for (const auto& part : parts) {
        char hs = part >> 8;
        char ls = part & 0xFF;
        ss << hs << ls;
    }
    return ss.str();
}

bool IDEAKey::deserialize(const std::string& serialized, IDEAKey& deserialized) {
    if (serialized.length() != 16) {
        return false;
    }

    for (size_t i = 0; i < 8; i++) {
        auto hs = serialized[i * 2];
        auto ls = serialized[i * 2 + 1];
        deserialized.parts[i] = (hs << 8) | ls;
    }
    return true;
}

IDEAKeyIterator::IDEAKeyIterator(const IDEAKey key) : currentIndex(0) {
    for (size_t i = 0; i < 8; i++) {
        currentKey[i] = key.parts[i];
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
    if (++currentIndex > 7) {
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

const SubKey& IDEAKeyIterator::operator*() {
    return currentKey[currentIndex];
}

const SubKey IDEAKeyIterator::next() {
    auto current = this->operator*();
    this->operator++();
    return current;
}

}  // namespace ideaencrypt
