//
// 実行引数パーサ
//

#include "ideacli/argparse.hpp"

#include <memory>

namespace ideacli {

void ArgumentParser::addOption(std::string name, OptionArgType opttype, char shortID) {
    auto optionID = currentOptionID++;
    nametable.insert(std::make_pair(optionID, name));
    options.push_back({optionID,
                       shortID,
                       NONE_LONG_OPT,
                       unwrapOptionType(opttype),
                       NONE_PRIORITY,
                       NONE_CALLFUNC});
}

void ArgumentParser::addOption(std::string name, OptionArgType opttype, char shortID, std::string longID) {
    auto optionID = currentOptionID++;
    nametable.insert(std::make_pair(optionID, name));
    PsrArgumentObject_t option = {optionID,
                                  shortID,
                                  "",
                                  unwrapOptionType(opttype),
                                  NONE_PRIORITY,
                                  NONE_CALLFUNC};

    auto optLen = (longID.length() > PSR_BUF_SIZE) ? PSR_BUF_SIZE : longID.length();
    for (size_t i = 0; i < optLen; i++) {
        option.long_opt[i] = longID[i];
    }
    options.push_back(option);
}

void ArgumentParser::addOption(std::string name, OptionArgType opttype, std::string longID) {
    auto optionID = currentOptionID++;
    nametable.insert(std::make_pair(optionID, name));
    PsrArgumentObject_t option = {optionID,
                                  NONE_SHORT_OPT,
                                  "",
                                  unwrapOptionType(opttype),
                                  NONE_PRIORITY,
                                  NONE_CALLFUNC};

    auto optLen = (longID.length() > PSR_BUF_SIZE) ? PSR_BUF_SIZE : longID.length();
    for (size_t i = 0; i < optLen; i++) {
        option.long_opt[i] = longID[i];
    }
    options.push_back(option);
}

int ArgumentParser::getOptionByID(int id) {
    int candidate = -1;
    for (size_t i = 0; i < options.size(); i++) {
        if (id == options[i].id) {
            candidate = i;
            break;
        }
    }
    return candidate;
}

PsrArgument_t ArgumentParser::unwrapOptionType(OptionArgType opttype) {
    switch (opttype) {
        case OptionArgType::None:
            return NO_ARGUMENT;

        case OptionArgType::Required:
            return REQUIRE_ARGUMENT;

        case OptionArgType::Optional:
            return OPTIONAL_ARGUMENT;

        default:
            return NO_ARGUMENT;
    }
}

bool ArgumentParser::parseArgs(int argc, char *argv[], OptionMap &map, ArgList &list) {
    auto optconf = std::make_unique<PsrArgumentObject_t[]>(options.size() + 1);
    for (size_t i = 0; i < options.size(); i++) {
        optconf[i] = options[i];
    }
    optconf[options.size()] = PSR_ARG_END;

    int id = 0;
    char *optarg = nullptr;
    int optind = 0;
    while ((id = parseropt(argc, argv, optconf.get(), &optarg, &optind)) != PSR_NOT_FOUND) {
        // エラー処理
        if (id < 0) {
            return false;
        }

        // idからPsrArgumentObject_tオブジェクトを検索
        int optionIndex = getOptionByID(id);
        if (optionIndex < 0) {
            return false;
        }

        // オプションIDとオプション文字列でペアを作り、mapへ追加
        bool hasOption = (optconf[optionIndex].has_arg != NO_ARGUMENT) && (optarg != nullptr);
        const char *a = hasOption ? optarg : "";
        map.insert(std::make_pair(nametable.at(id), a));
    }

    // 残りはvectorへ
    for (int i = optind; i < argc; i++) {
        list.push_back(std::string(argv[i]));
    }
    return true;
}

}  // namespace ideacli
