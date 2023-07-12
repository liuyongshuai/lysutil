/*************************************************************************
 * File:	http_response.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:05
 ************************************************************************/
#include<iostream>
#include "httpsvr/http_response.h"

namespace lysutil{
    namespace httpsvr{

        /**
         * 获取http状态的描述信息
         */
        void getHttpStatusDesc(HTTP_STATUS status, std::string &desc){
            std::map< HTTP_STATUS, std::string >::const_iterator iter;
            iter = httpStatusDesc.find(status);
            if (iter == httpStatusDesc.end()){
                desc = "unknown status";
            }
            else{
                desc = iter->second;
            }
        }

        /**
         * 获取http状态的描述信息
         */
        std::string getHttpStatusDesc(HTTP_STATUS status){
            std::string desc;
            getHttpStatusDesc(status, desc);
            return desc;
        }

        httpResponse::httpResponse(){
            this->setHeader("Referrer-Policy", "origin-when-cross-origin");
            this->setHeader("X-Powered-By", "transtopic");
            this->setHeader("Server", "transtopic");
            this->setHeader("Cache-Control", "max-age=86400");
            this->setHeader("Expires", lysutil::comutils::httpUtils::genRespDate(86400));
            this->setHeader("Date", lysutil::comutils::httpUtils::genRespDate(0));
            this->status_ = HTTP_STATUS::OK;
            this->setHeader("Content-Type", "text/html; charset=utf-8");
        }

        //设置响应值
        void httpResponse::setStatus(HTTP_STATUS status){
            this->status_ = status;
        }

        //设置头信息
        void httpResponse::setHeader(const std::string &k, const std::string &v){
            std::string headerKey = k;
            comutils::httpUtils::canonicalHeaderKey(headerKey);
            std::vector< std::string > tmp;
            tmp.push_back(v);
            this->headers_[headerKey] = tmp;
        }

        void httpResponse::addHeader(const std::string &k, const std::string &v){
            std::string headerKey = k;
            comutils::httpUtils::canonicalHeaderKey(headerKey);
            this->headers_[headerKey].push_back(v);
        }

        void httpResponse::getHeader(const std::string &k, std::vector< std::string > &vs){
            std::string headerKey = k;
            comutils::httpUtils::canonicalHeaderKey(headerKey);
            std::vector< std::string > tmp;
            std::map< std::string, std::vector< std::string > >::const_iterator
                    iter;
            if ((iter = this->headers_.find(headerKey)) == this->headers_.end()){
                return;
            }
            vs = iter->second;
        }

        //设置cookie信息
        void httpResponse::setCookie(const std::string &k, const std::string &v, uint32_t maxAgeTime, const std::string &path, const std::string &domain, bool secure, bool httponly){
            if (k.empty()){
                return;
            }
            std::string ckBuf;
            ckBuf.append(k + "=" + v);

            //过期时间
            if (maxAgeTime > 0){
                std::string date = lysutil::comutils::httpUtils::genRespDate(maxAgeTime);
                char exp[256] = {0};
                sprintf(exp, "; Expires=%s; Max-Age=%u", date.c_str(), maxAgeTime);
                ckBuf.append(exp);
            }
            else{
                ckBuf.append("; Max-Age=0");
            }

            //path信息
            if (!path.empty()){
                char p[256] = {0};
                sprintf(p, "; Path=%s", path.c_str());
                ckBuf.append(p);
            }
            else{
                ckBuf.append("; Path=/");
            }

            //domain信息
            if (!domain.empty()){
                char d[256] = {0};
                sprintf(d, "; Domain=%s", domain.c_str());
                ckBuf.append(d);
            }

            //Secure
            if (secure){
                ckBuf.append("; Secure");
            }

            //httponly
            if (httponly){
                ckBuf.append("; HttpOnly");
            }
            this->addHeader("Set-Cookie", ckBuf);
        }

        //设置响应的body信息
        void httpResponse::setBody(const std::string &body){
            this->body_ = body;
        }

        //返回json信息
        void httpResponse::setJsonHeader(){
            this->setHeader("Content-Type", "application/json; charset=utf-8");
        }

        //生成header信息
        void httpResponse::genHeader(std::string &h){
            std::string statusStr = getHttpStatusDesc(this->status_);
            char r[64] = {0};
            sprintf(r, "HTTP/1.1 %d %s\r\n", this->status_, statusStr.c_str());
            h = r;
            std::map< std::string, std::vector< std::string >>::const_iterator mapIter;
            std::vector< std::string >::const_iterator vecIter;
            for (mapIter = this->headers_.begin(); mapIter != this->headers_.end(); mapIter++){
                std::string hkey = mapIter->first;
                for (vecIter = mapIter->second.begin(); vecIter != mapIter->second.end(); vecIter++){
                    h.append(hkey + ": " + *vecIter + "\r\n");
                }
            }
        }

        //获取要返回的响应信息
        void httpResponse::getRsp(std::string &rsp){
            this->genHeader(rsp);
            rsp += "\r\n";
            rsp += this->body_;
        }
    }
}
