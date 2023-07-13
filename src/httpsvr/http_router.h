/*************************************************************************
 * File:	http_router.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:17
 ************************************************************************/
#ifndef _TRANSTOPIC_HTTP_ROUTER_H_202306271117_
#define _TRANSTOPIC_HTTP_ROUTER_H_202306271117_

#include<string>
#include<vector>
#include<map>
#include "httpsvr/http_common.h"
#include "comutils/str_utils.h"
#include "comutils/pcre_utils.h"

namespace lysutil{
    namespace httpsvr{
        class httpRouter{
        public:
            static std::shared_ptr< httpRouter > get_instance(){
                if (nullptr != instance_){
                    return instance_;
                }
                instance_ = std::shared_ptr< httpRouter >(new httpRouter);
                return instance_;
            }

            /**
             * 添加路由信息
             */
            void addRouter(const routerItem *r);

            /**
             * 添加路由信息
             * @param t         路由类型
             * @param c         路径规则信息
             * @param f         执行的回调函数
             * @param extParam  额外的参数
             */
            void addRouter(ROUTER_TYPE t, const std::string &c, httpFunc f, const std::string &static_file = "", const std::string &extParam = "");

            void addRouter(const routerItem &router);

            /**
             * 根据请求的url匹配路由规则
             * @param uri   请求的url
             * @param args  从url里提取的参数信息
             * @return
             */
            const routerItem *matchRouter(const std::string &uri, std::map< std::string, std::string > &args) const;

            /**
             * 析构
             */
            ~httpRouter();

        private:
            httpRouter(){};
            httpRouter(const httpRouter &) = delete;
            httpRouter &operator=(const httpRouter &) = delete;
            static std::shared_ptr< httpRouter > instance_;

            /**
             * 注册的路由列表
             */
            std::vector< routerItem * > routerList;

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
            bool matchPathInfoRouter(const std::string &uri, const routerItem *router, std::map< std::string, std::string > &args) const;

            /**
             * 匹配正则路由，直接用正则表达式去匹配请求的URL，并把捕获的参数回传到Param配置里,如
             * conf 为 `^ggtest/aid(\w+?)/cid(\d+)$`，Param 为 aid=$1&cid=$2
             * 则将请求中aid后面的字符串挑出来赋给aid，cid后面的字符串挑出来赋给cid
             */
            bool matchRegexpRouter(const std::string &uri, const routerItem *router, std::map< std::string, std::string > &args) const;
        };
    }
} //namespace Project


#endif //_TRANSTOPIC_HTTP_ROUTER_H_202306271117_
