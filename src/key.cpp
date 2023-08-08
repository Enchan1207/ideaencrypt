//
// 暗号鍵の生成
//
#include "ideaencrypt/key.hpp"

#include <sstream>

namespace ideaencrypt {

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

}  // namespace ideaencrypt
