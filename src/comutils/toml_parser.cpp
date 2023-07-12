/*************************************************************************
 * File:	toml_parser.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2020-01-09 10:47
 ************************************************************************/
#include "comutils/toml_parser.h"
#include "comutils/str_utils.h"

namespace lysutil{
    namespace comutils{
        TomlParser::TomlParser(const std::string &f) throw(cpptoml::parse_exception){
            gTable = cpptoml::parse_file(f);
        }

        std::string TomlParser::formatKey(const std::string &key){
            std::string ret = "";
            std::vector< std::string > tmps;
            strUtils::strSplit(key, '.', tmps);
            int i = 0;
            for (; i < tmps.size() - 1; i++){
                ret += ".";
                ret += tmps[i];
            }
            strUtils::trimChar(ret, '.');
            return ret;
        }

        std::string TomlParser::lastKey(const std::string &key){
            std::vector< std::string > tmps;
            strUtils::strSplit(key, '.', tmps);
            if (tmps.empty()){
                return "";
            }
            return tmps[tmps.size() - 1];
        }

        //提取表对象
        std::shared_ptr< cpptoml::table > TomlParser::getTable(const std::string &key){
            if (gTable == nullptr){
                return nullptr;
            }
            std::string tKey = formatKey(key);
            if (tKey.empty()){
                return nullptr;
            }
            if (tableCache.find(tKey) != tableCache.end()){
                return tableCache.find(tKey)->second;
            }
            std::vector< std::string > tmps;
            strUtils::strSplit(tKey, '.', tmps);
            std::vector< std::string > tmps1;
            int i = 0;
            std::shared_ptr< cpptoml::table > tmpTable = gTable;
            for (; i < tmps.size(); i++){
                int j = 0;
                std::string k = "";
                for (; j <= i; j++){
                    k += ".";
                    k += tmps[j];
                }
                strUtils::trimChar(k, '.');
                //如果缓存里有的话
                if (tableCache.find(k) != tableCache.end()){
                    tmpTable = tableCache.find(k)->second;
                }
                else{
                    auto t = tmpTable->get_table(tmps[i]);
                    tableCache[k] = t;
                    tmpTable = t;
                }
            }
            if (tableCache.find(tKey) != tableCache.end()){
                return tableCache.find(tKey)->second;
            }
            return nullptr;
        }

        int TomlParser::getBool(const std::string &key, bool &out){
            std::string lastk = lastKey(key);
            auto t = getTable(key);
            if (t == nullptr){
                return -1;
            }
            auto tmpRet = t->get_as< bool >(lastk);
            out = (bool) *tmpRet;
            return 0;
        }


        int TomlParser::getInt64(const std::string &key, int64_t &out){
            std::string lastk = lastKey(key);
            auto t = getTable(key);
            if (t == nullptr){
                return -1;
            }
            auto tmpRet = t->get_as< int64_t >(lastk);
            out = *tmpRet;
            return 0;
        }

        int TomlParser::getInt32(const std::string &key, int32_t &out){
            int64_t i64;
            int r = getInt64(key, i64);
            if (r != 0){
                return r;
            }
            out = (int32_t) i64;
            return 0;
        }

        int TomlParser::getInt(const std::string &key, int &out){
            int64_t i64;
            int r = getInt64(key, i64);
            if (r != 0){
                return r;
            }
            out = (int) i64;
            return 0;
        }

        int TomlParser::getDouble(const std::string &key, double &out){
            std::string lastk = lastKey(key);
            auto t = getTable(key);
            if (t == nullptr){
                return -1;
            }
            auto tmpRet = t->get_as< double >(lastk);
            out = *tmpRet;
            return 0;
        }

        int TomlParser::getString(const std::string &key, std::string &out){
            std::string lastk = lastKey(key);
            auto t = getTable(key);
            if (t == nullptr){
                return -1;
            }
            auto tmpRet = t->get_as< std::string >(lastk);
            out = *tmpRet;
            return 0;
        }

        int TomlParser::getStringList(const std::string &key, std::vector< std::string > &out){
            std::string lastk = lastKey(key);
            auto t = getTable(key);
            if (t == nullptr){
                return -1;
            }
            auto tmpOut = t->get_array_of< std::string >(lastk);
            for (auto &tp: *tmpOut){
                out.push_back(tp);
            }
            return 0;
        }

        int TomlParser::getInt64List(const std::string &key, std::vector< int64_t > &out){
            std::string lastk = lastKey(key);
            auto t = getTable(key);
            if (t == nullptr){
                return -1;
            }
            auto tmpOut = t->get_array_of< int64_t >(lastk);
            for (auto &tp: *tmpOut){
                out.push_back(tp);
            }
            return 0;
        }

        int TomlParser::getDoubleList(const std::string &key, std::vector< double > &out){
            std::string lastk = lastKey(key);
            auto t = getTable(key);
            if (t == nullptr){
                return -1;
            }
            auto tmpOut = t->get_array_of< double >(lastk);
            for (auto &tp: *tmpOut){
                out.push_back(tp);
            }
            return 0;
        }
    }
} //namespace queryanalyse