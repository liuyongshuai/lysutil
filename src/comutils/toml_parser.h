/*************************************************************************
 * File:	toml_parser.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2020-01-09 10:44
 ************************************************************************/
#ifndef _QUERY_ANALYSE_TOML_PARSER_H_202001091044_
#define _QUERY_ANALYSE_TOML_PARSER_H_202001091044_

#include<string>
#include<vector>
#include<map>
#include "comutils/cpptoml.h"
#include <stdint.h>

namespace lysutil{
    namespace comutils{
        class TomlParser{
        public:
            TomlParser(const std::string &toml_file) throw(cpptoml::parse_exception);

            int getInt64(const std::string &key, int64_t &out);

            int getInt32(const std::string &key, int32_t &out);

            int getInt(const std::string &key, int &out);

            int getDouble(const std::string &key, double &out);

            int getBool(const std::string &key, bool &out);

            int getString(const std::string &key, std::string &out);

            int getStringList(const std::string &key, std::vector< std::string > &out);

            int getInt64List(const std::string &key, std::vector< int64_t > &out);

            int getDoubleList(const std::string &key, std::vector< double > &out);

        private:
            std::shared_ptr< cpptoml::table > gTable;
            std::map< std::string, std::shared_ptr< cpptoml::table>> tableCache;

            std::string formatKey(const std::string &key);

            std::string lastKey(const std::string &key);

            std::shared_ptr< cpptoml::table > getTable(const std::string &key);
        };
    }
}
#endif //_QUERY_ANALYSE_TOML_PARSER_H_202001091044_
