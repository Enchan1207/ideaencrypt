//
// IDEA暗号ブロック
//

#include "ideaencrypt/block.hpp"

#include <iomanip>
#include <sstream>

#include "ideaencrypt/calc.hpp"

namespace ideaencrypt {

void CipherFullBlock::code(const uint16_t (&input)[4], uint16_t (&output)[4]) const {
    // MAの前処理
    uint16_t a = lhmul(subKeys[0], input[0]);
    uint16_t b = subKeys[1] + input[1];
    uint16_t c = subKeys[2] + input[2];
    uint16_t d = lhmul(subKeys[3], input[3]);
    uint16_t e = a ^ c;
    uint16_t f = b ^ d;

    // MAブロック
    uint16_t g = lhmul(subKeys[4], e);
    uint16_t h = g + f;
    uint16_t i = lhmul(subKeys[5], h);
    uint16_t j = i + g;

    // 後処理
    output[0] = a ^ i;
    output[1] = c ^ i;
    output[2] = b ^ j;
    output[3] = d ^ j;
}

void CipherFullBlock::applyKeyForEncode(IDEAKey::iterator& iterator) {
    for (size_t i = 0; i < 6; i++, iterator++) {
        subKeys[i] = *iterator;
    }
}

void CipherFullBlock::applyKeyForDecode(IDEAKey::iterator& iterator, const size_t& step) {
    subKeys[4] = iterator.next();
    subKeys[5] = iterator.next();
    subKeys[0] = mulinv(iterator.next());
    if (step == 1) {
        subKeys[1] = addinv(iterator.next());
        subKeys[2] = addinv(iterator.next());
    } else {
        subKeys[2] = addinv(iterator.next());
        subKeys[1] = addinv(iterator.next());
    }
    subKeys[3] = mulinv(iterator.next());
}

std::string CipherFullBlock::dumpCipherBlock() {
    std::ostringstream dumpstream;
    for (const auto& key : subKeys) {
        dumpstream << std::hex << std::setw(4) << std::setfill('0') << key << " ";
    }
    return dumpstream.str();
}

void CipherHalfBlock::code(const uint16_t (&input)[4], uint16_t (&output)[4]) const {
    output[0] = lhmul(subKeys[0], input[0]);
    output[1] = subKeys[1] + input[2];
    output[2] = subKeys[2] + input[1];
    output[3] = lhmul(subKeys[3], input[3]);
}

void CipherHalfBlock::applyKeyForEncode(IDEAKey::iterator& iterator) {
    for (size_t i = 0; i < 4; i++, iterator++) {
        subKeys[i] = *iterator;
    }
}

void CipherHalfBlock::applyKeyForDecode(IDEAKey::iterator& iterator, const size_t& step) {
    subKeys[0] = mulinv(iterator.next());
    subKeys[1] = addinv(iterator.next());
    subKeys[2] = addinv(iterator.next());
    subKeys[3] = mulinv(iterator.next());
}

std::string CipherHalfBlock::dumpCipherBlock() {
    std::ostringstream dumpstream;
    for (const auto& key : subKeys) {
        dumpstream << std::hex << std::setw(4) << std::setfill('0') << key << " ";
    }
    return dumpstream.str();
}

}  // namespace ideaencrypt
