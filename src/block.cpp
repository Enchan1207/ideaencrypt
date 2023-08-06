//
// IDEA暗号ブロック
//

#include "ideaencrypt/block.hpp"

#include "ideaencrypt/calc.hpp"

namespace ideaencrypt {

void CipherFullBlock::code(const uint16_t (&input)[4], uint16_t (&output)[4]) const {
}

void CipherFullBlock::applyKeyForEncode(IDEAKey::iterator& iterator) {
    for (size_t i = 0; i < 6; i++, iterator++) {
        subKeys[i] = *iterator;
    }
}

void CipherFullBlock::applyKeyForDecode(IDEAKey::iterator& iterator) {
    subKeys[0] = mulinv(iterator.next());
    subKeys[1] = addinv(iterator.next());
    subKeys[2] = addinv(iterator.next());
    subKeys[3] = mulinv(iterator.next());
    subKeys[4] = mulinv(iterator.next());
    subKeys[5] = mulinv(iterator.next());
}

void CipherHalfBlock::code(const uint16_t (&input)[4], uint16_t (&output)[4]) const {
}

void CipherHalfBlock::applyKeyForEncode(IDEAKey::iterator& iterator) {
    for (size_t i = 0; i < 4; i++, iterator++) {
        subKeys[i] = *iterator;
    }
}

void CipherHalfBlock::applyKeyForDecode(IDEAKey::iterator& iterator) {
    subKeys[0] = mulinv(iterator.next());
    subKeys[1] = addinv(iterator.next());
    subKeys[2] = addinv(iterator.next());
    subKeys[3] = mulinv(iterator.next());
}

}  // namespace ideaencrypt
