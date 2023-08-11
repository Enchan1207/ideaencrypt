//
// IDEA暗号CLI (鍵生成)
//
#include <fstream>
#include <iomanip>
#include <iostream>
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
    parser.addOption("pass", ArgumentParser::OptionArgType::Optional, 'p', "passphrase");
    parser.addOption("output", ArgumentParser::OptionArgType::Required, 'o', "output");
    parser.addOption("help", ArgumentParser::OptionArgType::None, "help");

    ArgumentParser::OptionMap options;
    ArgumentParser::ArgList arguments;
    if (!parser.parseArgs(argc, argv, options, arguments)) {
        std::cout << "Invalid argument" << std::endl;
        printHelp();
        return 1;
    }

    // ヘルプを表示して終了
    if (options.find("help") != options.end()) {
        printHelp();
        return 0;
    }

    // outputは必須
    if (options.find("output") == options.end()) {
        std::cout << "option 'output' is not specified." << std::endl;
        printHelp();
        return 1;
    }

    // 引数は取らない
    if (arguments.size() > 0) {
        std::cout << "Invalid argument" << std::endl;
        printHelp();
        return 1;
    }

    // IDEA暗号鍵を生成
    IDEAKey key;
    auto passPhrase = options.find("pass");
    if (passPhrase != options.end()) {
        key = IDEAKey(passPhrase->second);
    } else {
        SubKey subkeys[8];
        std::random_device rand;
        for (auto& subkey : subkeys) {
            subkey = rand();
        }
        key = IDEAKey(subkeys);
    }

    // 副鍵の情報をコンソールに出力
    std::cout << "IDEA key generated successfully." << std::endl
              << std::endl;
    std::cout << "encryption subkeys:" << std::endl;
    auto encsubkeys = key.subKeys();
    for (size_t i = 0; i < 52; i++) {
        std::cout << std::hex << std::setw(4) << std::setfill('0')
                  << encsubkeys.next() << " ";

        if ((i + 1) % 8 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl
              << std::endl;
    std::cout << "NOTE: these subkeys won’t be stored in key file." << std::endl
              << "it only contains main key (first 8 bytes)." << std::endl;

    // ファイルに出力
    auto filename = options.find("output")->second;
    std::ofstream file(filename);
    if (!file) {
        std::cout << "failed to open file: " << filename << std::endl;
        return 1;
    }
    file << key.serialize();
    return 0;
}

void printHelp() {
    std::cout << "usage: ideakeygen [options...]" << std::endl
              << std::endl
              << "available options:" << std::endl
              << "  -p, --passphrase : pass phrase to generate key (optional)" << std::endl
              << "  -o, --output     : output destination of generated key (required)" << std::endl
              << std::endl
              << "if no pass phrase was passed, use random sequence instead." << std::endl;
}
