//
// IDEA暗号CLI (暗号化/復号)
//
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>

#include "ideacli/argparse.hpp"
#include "ideaencrypt/cipher.hpp"

using namespace ideaencrypt;
using namespace ideacli;

/**
 * @brief コマンドのヘルプを表示
 */
void printHelp();

int main(int argc, char* argv[]) {
    // コマンドライン引数をパース
    ArgumentParser parser;
    parser.addOption("enc", ArgumentParser::OptionArgType::None, 'e', "encode");
    parser.addOption("dec", ArgumentParser::OptionArgType::None, 'd', "decode");
    parser.addOption("keypath", ArgumentParser::OptionArgType::Required, 'k', "key");
    parser.addOption("in", ArgumentParser::OptionArgType::Required, 'i', "input");
    parser.addOption("out", ArgumentParser::OptionArgType::Required, 'o', "output");

    ArgumentParser::OptionMap options;
    ArgumentParser::ArgList arguments;
    if (!parser.parseArgs(argc, argv, options, arguments)) {
        std::cout << "Invalid argument" << std::endl;
        printHelp();
        return 1;
    }

    // 鍵ファイルは必須
    if (options.find("keypath") == options.end()) {
        std::cout << "option 'k' is not specified." << std::endl;
        printHelp();
        return 1;
    }
    auto keypath = options.find("keypath")->second;

    // eかdを指定する 同時に指定することはできない
    auto encpair = options.find("enc");
    auto decpair = options.find("dec");
    if ((encpair == decpair) || (encpair != options.end() && decpair != options.end())) {
        std::cout << "either option 'e' or 'd' must be specified." << std::endl;
        printHelp();
        return 1;
    }
    bool isDecMode = decpair != options.end();

    // 入出力ソースの特定
    bool isInputspecified = options.find("in") != options.end();
    auto inputPath = isInputspecified ? options.find("in")->second : "";
    bool isOutputspecified = options.find("out") != options.end();
    auto outputPath = isOutputspecified ? options.find("out")->second : "";

    // IDEAKeyの読み込みとデシリアライズ
    std::ifstream keyfile(keypath);
    if (!keyfile) {
        std::cout << "failed to open key file." << std::endl;
        return 1;
    }
    std::string serialized;
    keyfile >> serialized;
    IDEAKey key;
    if (!IDEAKey::deserialize(serialized, key)) {
        std::cout << "failed to load key file." << std::endl;
        return 1;
    }

    // 暗号ストリームを初期化
    CipherStream cipherStream(key);
    if (isDecMode) {
        cipherStream.configureForDecode();
    }

    // 入力を取得
    std::string input;
    if (isInputspecified) {
        std::ifstream inputStream(inputPath, std::ios::binary);
        if (!inputStream) {
            std::cout << "failed to open input." << std::endl;
            return 1;
        }
        std::istreambuf_iterator<char> inputStreamIterator(inputStream);
        std::istreambuf_iterator<char> last;
        for (; inputStreamIterator != last; ++inputStreamIterator) {
            input += *inputStreamIterator;
        }
    } else {
        for (const auto& arg : arguments) {
            input += arg;
        }
    }
    if (input.length() == 0) {
        std::cout << "empty file or arguments passed." << std::endl;
        return 1;
    }

    // 暗号ストリームに入力
    cipherStream << input;

    // 暗号結果の出力と保存
    std::string encoded;
    cipherStream >> encoded;

    if (isOutputspecified) {
        std::ofstream outputStream(outputPath, std::ios::binary);
        if (!outputStream) {
            std::cout << "failed to open output." << std::endl;
            return 1;
        }
        outputStream << encoded;
    } else {
        std::cout << encoded;
    }
    return 0;
}

void printHelp() {
    std::cout << "usage: idea [options...] [arguments...]" << std::endl
              << std::endl
              << "available options:" << std::endl
              << "  -k, --key         : IDEA key file path (required)" << std::endl
              << "  -e, --encode      : encode" << std::endl
              << "  -d, --decode      : decode" << std::endl
              << "  -i, --input       : input source" << std::endl
              << "  -o, --output      : output destination (optional)" << std::endl
              << std::endl
              << "option -e and -d can't specify at same time." << std::endl
              << "if option -o is not specified, use standard output instead." << std::endl;
}
