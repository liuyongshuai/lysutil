/*************************************************************************
 * File:	http_router.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:17
 ************************************************************************/
#include<iostream>
#include "httpsvr/http_router.h"

namespace lysutil{
    namespace httpsvr{
        std::shared_ptr <httpRouter> httpRouter::instance_ = nullptr;

        //添加一条路由信息
        void httpRouter::addRouter(const routerItem *r){
            this->addRouter(r->type, r->config, r->func, r->extParam);
        }


        /**
         * 添加路由信息
         */
        void httpRouter::addRouter(ROUTER_TYPE t, const std::string &c, httpFunc f, const std::string &static_file ,const std::string &extParam){
            std::string tmpUri = c;
            comutils::strUtils::trimChar(tmpUri, '/');
            routerItem *ritem = new routerItem;
            ritem->type = t;
            ritem->config = tmpUri;
            ritem->static_file = static_file;
            ritem->func = f;
            ritem->extParam = extParam;
            this->routerList.push_back(ritem);
        }

        void httpRouter::addRouter(const routerItem &router){
            std::string tmpUri = router.config;
            comutils::strUtils::trimChar(tmpUri, '/');
            routerItem *ritem = new routerItem;
            ritem->type = router.type;
            ritem->config = tmpUri;
            ritem->static_file = router.static_file;
            ritem->func = router.func;
            ritem->extParam = router.extParam;
            this->routerList.push_back(ritem);
        }

        /**
         * 根据请求的url匹配路由规则
         * @param uri   请求的url
         * @param args  从url里提取的参数信息
         * @return
         */
        const routerItem *httpRouter::matchRouter(const std::string &uri, std::map <std::string, std::string> &args) const{
            std::string tmpUri = uri;
            comutils::strUtils::trimChar(tmpUri, '/');
            std::vector< routerItem * >::const_iterator iter;
            for (iter = routerList.begin(); iter != routerList.end(); iter++){
                if ((*iter)->type == ROUTER_TYPE_PATH_INFO){
                    if (this->matchPathInfoRouter(tmpUri, *iter, args)){
                        return *iter;
                    }
                }
                else if ((*iter)->type == ROUTER_TYPE_REGEXP){
                    if (this->matchRegexpRouter(tmpUri, *iter, args)){
                        return *iter;
                    }
                }
            }
            return nullptr;
        }

        /**
         * 匹配全路径路由
         *      :arg	只配置数字类型
         *      :arg:	可以匹配任意类型
         * 如：
         *      /ggtest/:id	匹配下面的URL，并把 id作为参数回传给controller层
         *          /ggtest/6666
         *          /ggtest/89999
         *      /ggtest/:name:	匹配下面的URL。并把name作为参数回传给controller层
         *          /ggtest/wendao
         *          /ggtest/abc
         *          /ggtest/44444
         **/
        bool httpRouter::matchPathInfoRouter(const std::string &uri, const routerItem *router, std::map <std::string, std::string> &args) const{
            if (router->config.find(":") == std::string::npos && router->config == uri){
                return true;
            }
            std::map <std::string, std::string> tmpArgs;
            //请求的URL的切片
            std::vector <std::string> pathInfo;
            comutils::strUtils::strSplit(uri, '/', pathInfo);
            //事先配置好的路由的切片，要和URL逐个比对，若有:arg、:arg:这样的还要替换
            std::vector <std::string> confInfo;
            comutils::strUtils::strSplit(router->config, '/', confInfo);
            if (pathInfo.size() > confInfo.size()){
                return false;
            }
            size_t i;
            for (i = 0; i < confInfo.size(); i++){
                std::string val = confInfo[i];
                //当前段以冒号“:”开头
                if (val[0] == ':'){
                    if (pathInfo.size() > i){
                        std::string pathVal = pathInfo[i];
                        //非数字必须 :arg:，数字只能 :arg
                        if (!comutils::strUtils::isAllNumber(pathVal) && val[val.size() - 1] != ':'){
                            return false;
                        }
                        comutils::strUtils::trimChar(val, ':');
                        tmpArgs[val] = pathVal;
                    }
                }
                else if (pathInfo.size() <= i || pathInfo[i] != val){
                    return false;
                }
            }
            std::map< std::string, std::string >::const_iterator iter;
            for (iter = tmpArgs.begin(); iter != tmpArgs.end(); iter++){
                args[iter->first] = iter->second;
            }
            return true;
        }

        /**
         * 匹配正则路由，直接用正则表达式去匹配请求的URL，并把捕获的参数回传到Param配置里,如
         * conf 为 `^ggtest/aid(\w+?)/cid(\d+)$`，Param 为 aid=$1&cid=$2
         * 则将请求中aid后面的字符串挑出来赋给aid，cid后面的字符串挑出来赋给cid
         */
        bool httpRouter::matchRegexpRouter(const std::string &uri, const routerItem *router, std::map <std::string, std::string> &args) const{
            std::map <std::string, std::string> tmpArgs;
            comutils::pcreUtils reg(router->config);
            if (reg.reg_match(uri) != 0){
                std::cout << "httpRouter::matchRegexpRouter:regexp not match\t" << router->config << "\t" << uri << std::endl;
                return false;
            }
            std::vector <std::vector< std::string >> subList;
            reg.reg_match_all(uri, subList);
            std::string argStr = router->extParam;
            size_t i = 1;
            if (subList.size() > 0){
                std::vector < std::vector < std::string >> ::const_iterator
                iter;
                for (iter = subList.begin(); iter != subList.end(); iter++){
                    std::vector< std::string >::const_iterator iter1;
                    for (iter1 = iter->begin(); iter1 != iter->end(); iter1++){
                        if (*iter1 == uri){
                            continue;
                        }
                        std::string tmpArgStr = argStr;
                        char buf[256] = {0};
                        sprintf(buf, "$%lu", i++);
                        comutils::strUtils::strReplace(argStr, buf, *iter1, tmpArgStr);
                        argStr = tmpArgStr;
                    }
                }
            }

            std::vector <std::string> tmpArgVec;
            size_t pos;
            comutils::strUtils::strSplit(argStr, '&', tmpArgVec);
            std::vector< std::string >::const_iterator vecStrIter;
            for (vecStrIter = tmpArgVec.begin(); vecStrIter != tmpArgVec.end(); vecStrIter++){
                pos = vecStrIter->find_first_of("=", 0);
                if (pos == std::string::npos){
                    continue;
                }
                std::string k(*vecStrIter, 0, pos);
                std::string v(*vecStrIter, pos + 1);
                tmpArgs[k] = v;
            }

            std::map< std::string, std::string >::const_iterator iter;
            for (iter = tmpArgs.begin(); iter != tmpArgs.end(); iter++){
                args[iter->first] = iter->second;
            }
            return true;
        }

        httpRouter::~httpRouter(){
            std::vector< routerItem * >::iterator iter;
            for (iter = routerList.begin(); iter != routerList.end(); iter++){
                delete *iter;
            }
        }
    }
} //namespace Project
