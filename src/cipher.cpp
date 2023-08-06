//
// 暗号機
//

#include "ideaencrypt/cipher.hpp"

#include <string>

namespace ideaencrypt {

CipherStream::CipherStream(CipherBlockList blocks) : streamBuffer(""), cipherBlocks(blocks) {}

CipherStream::CipherStream() : streamBuffer() {
    for (size_t i = 0; i < 8; i++) {
        cipherBlocks.push_back(std::make_shared<CipherFullBlock>(CipherFullBlock()));
    }
    cipherBlocks.push_back(std::make_shared<CipherHalfBlock>(CipherHalfBlock()));
}

CipherStream& CipherStream::operator<<(int element) {
    streamBuffer << element;
    return *this;
}

CipherStream& CipherStream::operator<<(double element) {
    streamBuffer << element;
    return *this;
}

CipherStream& CipherStream::operator<<(std::ostream& (*f)(std::ostream&)) {
    f(streamBuffer);
    return *this;
}

std::string CipherStream::operator>>(std::string& data) {
    data = streamBuffer.str();

    return data;
}

}  // namespace ideaencrypt
