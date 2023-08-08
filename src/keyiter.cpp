//
// 暗号鍵イテレータ
//

#include "ideaencrypt/key.hpp"

namespace ideaencrypt {

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
