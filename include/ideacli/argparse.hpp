//
// 実行引数パーサ (libparseroptのthin-wrapper)
//

#ifndef IDEAENCRYPT_ARGPARSE_H
#define IDEAENCRYPT_ARGPARSE_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "parseropt.h"

namespace ideacli {

class ArgumentParser {
   public:
    using OptionMap = std::map<std::string, std::string>;
    using ArgList = std::vector<std::string>;

    /**
     * @brief オプション引数の種別
     */
    enum class OptionArgType {
        // オプションは引数を取らない
        None,

        // オプションは引数を要求する
        Required,

        // オプションは引数を取るが、要求しない(optional)
        Optional
    };

   private:
    /**
     * @brief 定義済みのコマンドラインオプション
     */
    std::vector<PsrArgumentObject_t> options;

    /**
     * @brief 現在のオプションID
     */
    uint16_t currentOptionID = 0;

    /**
     * @brief オプションIDとオプション名のテーブル
     */
    std::map<int, std::string> nametable;

    /**
     * @brief IDに該当するオプションを配列から引いてインデックスを返す
     *
     * @param id 検索対象のID
     * @return int 検索結果 (-1: 存在しない)
     */
    int getOptionByID(int id);

    /**
     * @brief OptionType列挙体をparseropt構造体へ代入可能な形にアンラップする
     *
     * @param opttype
     * @return PsrArgument_t
     */
    PsrArgument_t unwrapOptionType(OptionArgType opttype);

   public:
    void addOption(std::string name, OptionArgType opttype, char shortID);

    void addOption(std::string name, OptionArgType opttype, char shortID, std::string longID);

    void addOption(std::string name, OptionArgType opttype, std::string longID);

    /**
     * @brief コマンドライン引数のパース
     *
     * @param argc
     * @param argv
     * @param map
     * @param list
     * @return true
     * @return false
     */
    bool parseArgs(int argc, char *argv[], OptionMap &map, ArgList &list);
};

}  // namespace ideacli

#endif /* IDEAENCRYPT_ARGPARSE_H */
