/*************************************************************************
 * File:	str_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 13:16
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netdb.h>
#include<stdio.h>
#include<stdint.h>
#include <bzlib.h>
#include "lz4.h"
#include "zlib.h"
#include "zconf.h"
#include <zstd.h>
#include "comutils/str_utils.h"
#include "comutils/file_utils.h"
#include "comutils/sys_utils.h"
#include "comutils/color_utils.h"
#include "comutils/http_utils.h"
#include "comutils/rune_utils.h"
#include "comutils/pcre_utils.h"
#include "comutils/terminal_table.h"
#include <curl/curl.h>
#include <arpa/inet.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <iconv.h>
//#include <event2/event.h>
//#include <event2/bufferevent.h>
//#include <event2/listener.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libconfig.h++>
#include <iomanip>
#include "hiredis/hiredis.h"
#include <mysql.h>
#include <mysqld_error.h>
#include <errmsg.h>
#include <db.h>
#include <Poco/Timer.h>
#include <Poco/Thread.h>
#include <zmq.h>
#include <libunwind.h>
#include <nlohmann/json.hpp>
#include "json/json.h"
#include "sqlite3.h"
#include "oneapi/tbb/tick_count.h"
#include "oneapi/tbb/blocked_range.h"
#include "oneapi/tbb/concurrent_vector.h"
#include "oneapi/tbb/concurrent_queue.h"
#include "oneapi/tbb/concurrent_hash_map.h"
#include "oneapi/tbb/parallel_for.h"
#include "oneapi/tbb/parallel_reduce.h"
#include "oneapi/tbb/parallel_scan.h"
#include "oneapi/tbb/parallel_pipeline.h"
#include "oneapi/tbb/spin_mutex.h"
#include "oneapi/tbb/queuing_mutex.h"
#include "oneapi/tbb/global_control.h"
#include <ev.h>
#include <nanomsg/pair.h>
#include <nanomsg/nn.h>
#include "lua.hpp"


#define BUF_SIZE 1024
#define SERVER_IP "192.168.56.11"
#define SERVER_PORT 8000

const char *call = "Hello, my remote server.";
int cnt = 5;

#define CHUNK 100000
#define MaxLen 10000000
#define MAX_PROMPT 1024
#define MAXLINE 4096 //the length of all args is ARG_MAX
#define MAXARG 20


struct parse_info;
struct passwd *pwd;
char *buffer;

void printVector1(const std::vector<std::string> &output) {
    std::vector<std::string>::const_iterator iter;
    std::cout << std::endl;
    std::cout << std::endl;
    for (iter = output.begin(); iter != output.end(); iter++) {
        std::cout << *iter << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void printMap1(const std::map<std::string, std::string> &output) {
    std::map<std::string, std::string>::const_iterator iter;
    std::cout << std::endl;
    std::cout << std::endl;
    for (iter = output.begin(); iter != output.end(); iter++) {
        std::cout << "k=" << iter->first << "\tv=" << iter->second << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

const std::string headersStrTmp = "GET /add1.php HTTP/1.1\r\n"
                                  "Connection: keep-alive\r\n"
                                  "Host: wendao.abc.com\r\n"
                                  "Origin: http://wendao.abc.com\r\n"
                                  "User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 6_1_4 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10B350 Safari/8536.25\r\n"
                                  "X-Requested-With: XMLHttpRequest\r\n"
                                  "Referer: http://wendao.abc.com/abc/def/ssssss/?id=8554443825&gin=987532\r\n"
                                  "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary5ca6082fa3e74\r\n"
                                  "Content-Length: 91099\r\n"
                                  "Cookie: ssuid=65535\r\n"
                                  ""
                                  "sffasfdassfdasdfasdfsasdfsasfdsasfd";
const char *testForTable[] = {
        "0|1152921533330227217|地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“地锅一家人（家常炒菜）ａｂｃａ@￥@#%#ｓｄ🎈🎉ｆ我E２３４３４５んエォサ６３＃＄％＾＄＆％＾（＆我“",
        "2|1152921628226355217|*&）……冰火公社麻辣烫",
        "3|1152921584588816406|杨铭宇黄\n鸡米饭(五星家园店)",
        "4|1152921538896068625|李克强从八个方面对过去一年的工作进行了回顾：创新和完善宏观调控，经济保持平稳运行；扎实打好三大攻坚战，重点任务取得积极进展；深化供给侧结构性改革，实体经济活力不断释放；深入实施创新驱动发展战略，创新能力和效率进一步提升；加大改革开放力度，发展动力继续增强；统筹城乡区域发展，良性互动格局加快形成；坚持在发展中保障和改善民生，改革发展成果更多更公平惠及人民群众；推进法治政府建设和治理创新，保持社会和谐稳定。",
        "5|3458764672814809215|淇帕创作（五星家园店）",
        "7|1152921708773769223|美丽家常菜馆",
        "8|11529216325884313821152921632588431382|美粥柒天（五星家园旗舰店）",
        "9|1152921711391014924|老北京炸酱面(五星家园店)",
        "10|1152921698518695953|三福酸菜鱼牛蛙店(清扬路店)",
        "11|3458764702137188504|吃货小店（五星家园店）",
        "20|1152921569279606801|山东水饺（茂业天地店）三维效果图\n"
        "\n"
        "\t最新进展\n"
        "\n"
        "\t大坝最高坝段已至高程685米\n"
        "\n"
        "\t成都还是二月天，春帷不揭，但600多公里外的宁南县与云南巧家县交界地，已草木深绿。一座新建不久的大桥横跨金沙江，将四川与云南连接。\n"
        "\n"
        "\t19日上午，白鹤滩工程建设部工作人员王洪玉，摸出手机看了当天天气信息：上午8点15分，坝区白天晴间多云，气温10~20°C，极大风力9~10级。\n"
        "\n"
        "\t“温度要比成都暖和。”他说，但大风在这里很常见。在他带领下，记者探访了这处世界在建的最大水电站——白鹤滩水电站。\n"
        "\n"
        "\t沿纵横交错的山体隧道网穿行10余公里，站在大坝的观测高点，可以俯瞰整个在建大坝，十分雄伟。整个拦河大坝为椭圆线型混凝土双曲拱坝，最大坝高289米，坝顶中心线弧长709米，坝顶宽度仅14米，坝体将浇筑混凝土818万立方米。\n"
        "\n"
        "\t“坝体浇筑每天都有进展，并传输到手机上。”王洪玉点开手机上的实时数据，大坝从2017年4月12日浇筑混凝土，目前最高坝段浇筑至高程685米，最高上升高度140米，累计浇筑340万立方米混凝土。",
        "13|1152921618071945228|吴记酸辣汤",
        "14|1152921524996145174|阿姨奶茶(清名一村店)",
        "12|1152921752692326422|漓江云姨桂林米粉（南下塘店）丁书苗为原铁道部部长刘志军身边的关键女商人，号称高铁一姐。此番拍卖丁书苗资产的法院，即是负责丁书苗案的北京市二中院。早在2018年12月14日，新京报独家报道，丁书苗旗下位于北京CBD的一处五星级的伯豪瑞廷大酒店，即将被北京市二中院拍卖，评估价16亿元，起拍价11.27亿元。不过，12月25日，法院发布北京伯豪瑞廷酒店有限责任公司的100%股权变更公告，\"因出现法定事由，决定暂缓拍卖\"。据介绍，北京伯豪瑞廷酒店于2008年7月开业，2009年被评定为五星级饭店。是一家集客房、餐饮、会议、康乐功能为一体的商务酒店。酒店营业性建筑面积43000平方米。地处CBD核心商圈，交通位置便利，服务功能齐全，获评“2015年度优秀会议酒店”，第十五届中国饭店金马奖。不过，在2018年12月25日上午9时16分，法院发布北京伯豪瑞廷酒店有限责任公司的100%股权变更公告，“因出现法定事由，决定暂缓拍卖”。据北京市二中院2月15日发布的通告，第一次拍卖被暂缓，盖因法院接到多个电话实名举报，反映在拍卖过程中存在串标、围标、毁标等严重影响拍卖公平公正等行为，经研究决定，二中院依法暂缓了该次拍卖，并就举报涉及的情况进行了相关的调查及处理。今年1月，新京报报道，北京市第二中级人民法院将于2019年2月2日10时至2019年2月3日10时止（延时的除外）进行公开拍卖活动，拍卖标的为北京伯豪瑞廷酒店有限责任公司的100%股权，评估价161064.566232万元，起拍价144958.1096万元，保证金14000万元，增价幅度600万元。新京报记者注意到，重新开拍后，该资产的起拍价提高了约3亿元。”，",
        "15|3458764732797550744|墨冉小橱（家常炒菜）",
        "16|1152921734510018567|堡莱坞炸鸡汉堡（芦庄店）",
        "17|1152921594567065617|1+7水饺店(金科世界城店)",
        "18|1152921562266730504|豫味一品土豆粉（家乐福店）",
        "21|1152921556910604301|享吃私房披萨店",
        "26|1152921596790046742|沙县小吃(五星人民医院店)<div class=\"dropdown global-dropdown\">\n"
        "<button class=\"global-dropdown-toggle\" data-toggle=\"dropdown\" type=\"button\">\n"
        "<span class=\"sr-only\">Toggle navigation</span>\n"
        "<i aria-hidden='true' data-hidden = \"true\" class=\"fa fa-bars\"></i>\n"
        "</button>\n"
        "\t\t<div class=\"dropdown-menu-nav global-dropdown-menu\">\n"
        "<ul>\n"
        "\t<li class=\"home active\">\n"
        "\t\t<a title=\"Projects\" class=\"dashboard-shortcuts-projects\" href=\"/dashboard/projects\">\n"
        "\t\t<div class=\"shortcut-mappings\">\n"
        "\t\t\t<div class=\"key\">\n"
        "\t\t\t\t\t<i aria-label=\"hidden\" class=\"fa fa-arrow-up\"></i>\n"
        "\t\t\t\t\t</div>\n"
        "\t\t\t\t</div>\n"
        "\t\t\t</a>\n"
        "\t\t</li>\n"
        "\t</ul>\n"
        "\t</div>\n"
        "</div>",
        "22|1152921634152906774|食之味骨汤麻辣烫（通扬路店）",
        "23|1152921623264493590|腾宇记黄焖鸡米饭(瑞星家园店)",
        "24|1152921753204031511|胥先生鸭血粉丝（人民医院店）",
        "25|1152921708723437585|家常菜馆(中南路店)",
        "27|1152921573658460169|小胖子功夫龙虾（无锡总店）",
        "28|1152921644617695244|小四川家常菜（扬名店）",
        "29|1152921546546479126|特色圆盅(前进路店)",
        "30|1152921509242339341|桥头排骨\n（金城路店）加拿大籍人员康明凯（Michael John Kovrig）窃取、刺探国家秘密和情报案侦办工作已取得重要进展。有关部门介绍，康明凯自2017年以来，经常持普通护照和商务签证入境，通过中国境内的关系人，窃取、刺探中国敏感信息和情报。迈克尔（Spavor Michael Peter Todd)是康明凯的重要情报关系人，向康明凯提供情报。",
};

int testbz2() {
    std::string file_name = "xxxx.bin.bz2";
    std::vector<short> orginVec;
    if (access(file_name.c_str(), F_OK) != 0) {
        printf("ERROR: file %s not exists!\n", file_name.c_str());
        return -99;
    } else {
        int bzerror;

        FILE *fp = fopen(file_name.c_str(), "rb");
        if (!fp) {
            printf("ERROR: open file %s fail!\n", file_name.c_str());
            return -1;
        }
        BZFILE *bzFp = BZ2_bzReadOpen(&bzerror, fp, 4, 0, NULL, 0);
        if (bzerror != BZ_OK) {
            BZ2_bzReadClose(&bzerror, bzFp);
            fclose(fp);
            printf("ERROR: read file %s fail!\n", file_name.c_str());
            return -1;
        }

        while (bzerror == BZ_OK) {
            short orginData = 0;
            BZ2_bzRead(&bzerror, bzFp, (char *) (&orginData), sizeof(short));
            if (bzerror == BZ_OK) {
                orginVec.push_back(orginData);
            } else if (bzerror == BZ_STREAM_END) {
                break;
            } else {
                if (bzerror == BZ_DATA_ERROR) {
                    printf("ERROR: file %s BZ_DATA_ERROR!\n", file_name.c_str());
                } else if (bzerror == BZ_PARAM_ERROR) {
                    printf("ERROR: file %s BZ_PARAM_ERROR!\n", file_name.c_str());
                } else if (bzerror == BZ_SEQUENCE_ERROR) {
                    printf("ERROR: file %s BZ_SEQUENCE_ERROR!\n", file_name.c_str());
                } else if (bzerror == BZ_IO_ERROR) {
                    printf("ERROR: file %s BZ_IO_ERROR!\n", file_name.c_str());
                } else if (bzerror == BZ_UNEXPECTED_EOF) {
                    printf("ERROR: file %s BZ_UNEXPECTED_EOF!\n", file_name.c_str());
                } else if (bzerror == BZ_DATA_ERROR_MAGIC) {
                    printf("ERROR: file %s BZ_DATA_ERROR_MAGIC!\n", file_name.c_str());
                } else if (bzerror == BZ_MEM_ERROR) {
                    printf("ERROR: file %s BZ_MEM_ERROR!\n", file_name.c_str());
                }
            }
        }
        BZ2_bzReadClose(&bzerror, bzFp);
        fclose(fp);
    }
}

int testlz4() {
    const char *input = "Hello, World!";
    int inputSize = strlen(input) + 1;

    int maxOutputSize = LZ4_compressBound(inputSize);
    char *compressedData = (char *) malloc(maxOutputSize);
    int compressedSize = LZ4_compress_default(input, compressedData, inputSize, maxOutputSize);

    printf("Compressed data length: %d\n", compressedSize);
    char *decompressedData = (char *) malloc(inputSize);

    int decompressedSize = LZ4_decompress_safe(compressedData, decompressedData, compressedSize, inputSize);
    printf("Decompressed data: %s\n", decompressedData);

    free(compressedData);
    free(decompressedData);
    return 0;
}

void encodeZip(const std::string &buffer, std::string &zipBuf, int &zipLen) {
    unsigned char _zipSrc[MaxLen];
    unsigned char _zipDst[MaxLen];
    unsigned char _scBuffer[MaxLen];

    //先对原始内容进行压缩工作
    unsigned int tmpLen = buffer.length();
    memcpy(_zipSrc, buffer.c_str(), tmpLen);
    z_stream c_stream;
    c_stream.zalloc = (alloc_func) 0;
    c_stream.zfree = (free_func) 0;
    c_stream.opaque = (voidpf) 0;
    c_stream.next_in = (Bytef *) _zipSrc;
    c_stream.avail_in = tmpLen;
    c_stream.next_out = (Bytef *) _zipDst;
    c_stream.avail_out = (unsigned int) MaxLen;
    int ret = deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        std::cout << "|" << "deflateInit2 error " << std::endl;
        return;
    }
    ret = deflate(&c_stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&c_stream);
        std::cout << "|" << "ret != Z_STREAM_END err=" << ret << std::endl;
        return;
    }

    zipLen = c_stream.total_out;
    ret = deflateEnd(&c_stream);
    if (ret != Z_OK) {
        std::cout << "|" << "deflateEnd error " << std::endl;
        return;
    }
    //压缩完毕进行返回包组织
    memcpy(_scBuffer, _zipDst, zipLen);
    zipBuf = std::string(reinterpret_cast<const char *>(_scBuffer), zipLen);
    return;
}

int decodeZip(char *source, int len, char **dest) {
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];
    int totalsize = 0;

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    ret = inflateInit2(&strm, 31);

    if (ret != Z_OK)
        return ret;

    strm.avail_in = len;
    strm.next_in = (unsigned char *) source;

    /* run inflate() on input until output buffer not full */
    do {
        strm.avail_out = CHUNK;
        strm.next_out = out;
        ret = inflate(&strm, Z_NO_FLUSH);
        switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR; /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&strm);
                return ret;
        }

        have = CHUNK - strm.avail_out;
        totalsize += have;
        *dest = (char *) realloc(*dest, totalsize);
        memcpy(*dest + totalsize - have, out, have);
    } while (strm.avail_out == 0);

    /* clean up and return */
    inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int testzstd() {
    // compress
    size_t com_space_size;
    size_t peppa_pig_text_size;

    char *com_ptr = NULL;
    char peppa_pig_buf[2048] = "Narrator: It is raining today. So, Peppa and George cannot play outside.Peppa: Daddy, it's stopped raining. Can we go out to play?Daddy: Alright, run along you two.Narrator: Peppa loves jumping in muddy puddles.Peppa: I love muddy puddles.Mummy: Peppa. If you jumping in muddy puddles, you must wear your boots.Peppa: Sorry, Mummy.Narrator: George likes to jump in muddy puddles, too.Peppa: George. If you jump in muddy puddles, you must wear your boots.Narrator: Peppa likes to look after her little brother, George.Peppa: George, let's find some more pud dles.Narrator: Peppa and George are having a lot of fun. Peppa has found a lttle puddle. George hasfound a big puddle.Peppa: Look, George. There's a really big puddle.Narrator: George wants to jump into the big puddle first.Peppa: Stop, George. | must check if it's safe for you. Good. It is safe for you. Sorry, George. It'sonly mud.Narrator: Peppa and George love jumping in muddy puddles.Peppa: Come on, George. Let's go and show Daddy.Daddy: Goodness me.Peppa: Daddy. Daddy. Guess what we' ve been doing.Daddy: Let me think... Have you been wa tching television?Peppa: No. No. Daddy.Daddy: Have you just had a BATh?Peppa: No. No.Daddy: | know. You've been jumping in muddy puddles.Peppa: Yes. Yes. Daddy. We've been jumping in muddy puddles.Daddy: Ho. Ho. And look at the mess you're in.Peppa: Oooh....Daddy: Oh, well, it's only mud. Let's clean up quickly before Mummy sees the mess.Peppa: Daddy, when we've cleaned up, will you and Mummy Come and play, too?Daddy: Yes, we can all play in the garden.Narrator: Peppa and George are wearing their boots. Mummy and Daddy are wearing their boots.Peppa loves jumping up and down in muddy puddles. Everyone loves jumping up and down inmuddy puddles.Mummy: Oh, Daddy pig, look at the mess you're in. .Peppa: It's only mud.";

    peppa_pig_text_size = strlen(peppa_pig_buf);
    com_space_size = ZSTD_compressBound(peppa_pig_text_size);
    com_ptr = (char *) malloc(com_space_size);
    if (NULL == com_ptr) {
        std::cout << "compress malloc failed" << std::endl;
        return -1;
    }

    size_t com_size;
    com_size = ZSTD_compress(com_ptr, com_space_size, peppa_pig_buf, peppa_pig_text_size, ZSTD_fast);
    std::cout << "peppa pig text size:" << peppa_pig_text_size << std::endl;
    std::cout << "compress text size:" << com_size << std::endl;
    std::cout << "compress ratio:" << (float) peppa_pig_text_size / (float) com_size << std::endl << std::endl;


    // decompress
    char *decom_ptr = NULL;
    unsigned long long decom_buf_size;
    decom_buf_size = ZSTD_getFrameContentSize(com_ptr, com_size);

    decom_ptr = (char *) malloc((size_t) decom_buf_size);
    if (NULL == decom_ptr) {
        std::cout << "decompress malloc failed" << std::endl;
        return -1;
    }

    size_t decom_size;
    decom_size = ZSTD_decompress(decom_ptr, decom_buf_size, com_ptr, com_size);
    std::cout << "decompress text size:" << decom_size << std::endl;

    if (strncmp(peppa_pig_buf, decom_ptr, peppa_pig_text_size)) {
        std::cout << "decompress text is not equal peppa pig text" << std::endl;
    }

    free(com_ptr);
    free(decom_ptr);
    return 0;
}

int read_command(char **command, char **parameters, char *prompt) {
#ifdef READLINE_ON
    char* tmpbuffer = buffer
    buffer  = readline(prompt);
    if(feof(stdin) == 0)
    {
        printf("\n");
        exit(0);
    }
#else
    printf("%s", prompt);
    char *Res_fgets = fgets(buffer, MAXLINE, stdin);
    if (Res_fgets == NULL) {
        printf("\n");
        exit(0);
    }
#endif
    if (buffer[0] == '\0')
        return -1;
    char *pStart, *pEnd;
    int count = 0;
    int isFinished = 0;
    pStart = pEnd = buffer;
    while (isFinished == 0) {
        while ((*pEnd == ' ' && *pStart == ' ') || (*pEnd == '\t' && *pStart == '\t')) {
            pStart++;
            pEnd++;
        }

        if (*pEnd == '\0' || *pEnd == '\n') {
            if (count == 0)
                return -1;
            break;
        }

        while (*pEnd != ' ' && *pEnd != '\0' && *pEnd != '\n')
            pEnd++;


        if (count == 0) {
            char *p = pEnd;
            *command = pStart;
            while (p != pStart && *p != '/')
                p--;
            if (*p == '/')
                p++;
            //else //p==pStart
            parameters[0] = p;
            count += 2;
#ifdef DEBUG
            printf("\ncommand:  %s\n",*command);
#endif
        } else if (count <= MAXARG) {
            parameters[count - 1] = pStart;
            count++;
        } else {
            break;
        }

        if (*pEnd == '\0' || *pEnd == '\n') {
            *pEnd = '\0';
            isFinished = 1;
        } else {
            *pEnd = '\0';
            pEnd++;
            pStart = pEnd;
        }
    }

    parameters[count - 1] = NULL;

#ifdef DEBUG
    /*input analysis*/
    printf("input analysis:\n");
    printf("pathname:%s\ncommand:%s\nparameters:\n",*command,parameters[0]);
    int i;
    for(i=0;i<count-1;i++)
        printf("%s\n",parameters[i]);
#endif

//free the space of readline() ---error
/**
 * edit by reeves
 * 2015/07/03
 * 下面这样写会有错误，导致readline情况下输入命令却不执行
#ifdef READLINE_ON
    free(buffer);
    buffer = tmpbuffer;
#endif
    return count;
}
*/
//free the space of readline()---correct
#ifdef READLINE_ON
    buffer=NULL;
    free(tmpbuffer);
#endif
    return count;
}

#define MIN(a, b) ((a)>(b)?(b):(a))

void dumprawmsg(char *p, int len) {
    int i = 0;
    for (i = 0; i < len; i++) {
        unsigned char c = p[i];
        printf("%.2X ", c);
    }
    printf("\n");
}

int convmsg(char *src, char *des, int srclen, int deslen, const char *srctype, const char *destype) {
    if (strcmp(srctype, destype) == 0) {
        memcpy(des, src, MIN(srclen, deslen));
        return 0;
    }
    iconv_t conv = iconv_open(destype, srctype);
    if (conv == (iconv_t) -1) {
        printf("iconvopen err\n");
        return -1;
    }
    char *in = src;
    char *out = des;
    size_t avail = deslen;
    size_t insize = srclen;
    char *wrptr = des;
    char *inptr = src;
    while (avail > 0) {
        size_t nread;
        size_t nconv;
        printf("avail:%d\n", avail);
        /* Do the conversion.  */
        nconv = iconv(conv, &inptr, &insize, &wrptr, &avail);
        if (nconv == (size_t) -1) {
            /* Not everything went right.  It might only be
               an unfinished byte sequence at the end of the
               buffer.  Or it is a real problem.  */
            if (errno == EINVAL) {
                /* This is harmless.  Simply move the unused
                   bytes to the beginning of the buffer so that
                   they can be used in the next round.  */
                //memmove (inbuf, inptr, insize);
                printf("EINVAL\n");
            } else {
                /* It is a real problem.  Maybe we ran out of
                   space in the output buffer or we have invalid
                   input.  In any case back the file pointer to
                   the position of the last processed byte.  */
                printf("error\n");
                break;
            }
        }
    }
    iconv_close(conv);
    return 0;
}

////读取回调
//void read_cb(struct bufferevent *bev, void *ctx) {
//    //读取数据
//    char buf[BUF_SIZE] = {0x00};
//    size_t len = 0;
//    printf("Receive data:\n");
//    len = bufferevent_read(bev, buf, BUF_SIZE);
//    while (0 < len) {
//        printf("%s", buf);
//        memset(buf, 0x00, 1024);
//        len = bufferevent_read(bev, buf, BUF_SIZE);
//    }
//    printf("\n");
//}
//
////写回调
//void write_cb(struct bufferevent *bev, void *ctx) {
//    if (cnt--) {
//        printf("Send data to server.\n");
//        if (-1 == bufferevent_write(bev, call, strlen(call))) {
//            printf("Send data to server failed.\n");
//        }
//        sleep(5);
//    } else {
//        printf("send stop");
//        bufferevent_write(bev, "stop", strlen("stop"));
//    }
//}
//
////事件回调
//void event_cb(struct bufferevent *bev, short events, void *ctx) {
//    if (events & BEV_EVENT_ERROR) {
//        perror("Error from bufferevent.");
//    }
//    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
//        bufferevent_free(bev);
//    }
//}


void testXML() {
    xmlDocPtr bookDocPtr;
    xmlNodePtr booksNodePtr;
    char *bookDocName = "/Users/gxf/CLionProjects/untitled/books.xml";
    // libxml默认将各个节点间的空格当作一个节点
    xmlKeepBlanksDefault(0);
    bookDocPtr = xmlParseFile(bookDocName);
    if (bookDocPtr == NULL) {
        fprintf(stderr, "xmlReadFile fail");
        exit(1);
    }
    booksNodePtr = xmlDocGetRootElement(bookDocPtr);
    xmlNodePtr bookNodePtr = booksNodePtr->children;
    while (bookNodePtr) {
        xmlNodePtr namePtr = bookNodePtr->children;
        std::cout << xmlNodeGetContent(namePtr) << std::endl;
        xmlNodePtr authorPtr = namePtr->next;
        std::cout << xmlNodeGetContent(authorPtr) << std::endl;
        xmlNodePtr datePtr = authorPtr->next;
        std::cout << xmlNodeGetContent(datePtr) << std::endl;
        bookNodePtr = bookNodePtr->next;
    }
}

int parseConfigInfo() {
    libconfig::Config cfg;     //1.声明 Config对象

    cfg.readFile("example.cfg");    //读取配置文件

    // Get the store name.
    try {
        std::string name = cfg.lookup("name");
        std::cout << "Store name: " << name << std::endl << std::endl;
    }
    catch (const libconfig::SettingNotFoundException &nfex)         //配置没找到会有SettingNotFoundException 异常
    {
        std::cerr << "No 'name' setting in configuration file." << std::endl;
    }
    // Get the store name.
    try {
        std::string name = cfg.lookup("name");
        std::cout << "Store name: " << name << std::endl << std::endl;
    }
    catch (const libconfig::SettingNotFoundException &nfex) {
        std::cerr << "No 'name' setting in configuration file." << std::endl;
    }

    const libconfig::Setting &root = cfg.getRoot();

    // Output a list of all books in the inventory.
    try {
        const libconfig::Setting &books = root["inventory"]["books"];
        int count = books.getLength();

        std::cout << std::setw(30) << std::left << "TITLE" << "  "
                  << std::setw(30) << std::left << "AUTHOR" << "   "
                  << std::setw(6) << std::left << "PRICE" << "  "
                  << "QTY"
                  << std::endl;

        for (int i = 0; i < count; ++i) {
            const libconfig::Setting &book = books[i];

            // Only output the record if all the expected fields are present.
            std::string title, author;
            double price;
            int qty;

            if (!(book.lookupValue("title", title)
                  && book.lookupValue("author", author)
                  && book.lookupValue("price", price)
                  && book.lookupValue("qty", qty)))
                continue;

            std::cout << std::setw(30) << std::left << title << "  "
                      << std::setw(30) << std::left << author << "  "
                      << '$' << std::setw(6) << std::right << price << "  "
                      << qty
                      << std::endl;
        }
        std::cout << std::endl;
    }
    catch (const libconfig::SettingNotFoundException &nfex) {
        // Ignore.
    }

    // Output a list of all books in the inventory.
    try {
        const libconfig::Setting &movies = root["inventory"]["movies"];
        int count = movies.getLength();

        std::cout << std::setw(30) << std::left << "TITLE" << "  "
                  << std::setw(10) << std::left << "MEDIA" << "   "
                  << std::setw(6) << std::left << "PRICE" << "  "
                  << "QTY"
                  << std::endl;

        for (int i = 0; i < count; ++i) {
            const libconfig::Setting &movie = movies[i];

            // Only output the record if all of the expected fields are present.
            std::string title, media;
            double price;
            int qty;

            if (!(movie.lookupValue("title", title)
                  && movie.lookupValue("media", media)
                  && movie.lookupValue("price", price)
                  && movie.lookupValue("qty", qty)))
                continue;

            std::cout << std::setw(30) << std::left << title << "  "
                      << std::setw(10) << std::left << media << "  "
                      << '$' << std::setw(6) << std::right << price << "  "
                      << qty
                      << std::endl;
        }
        std::cout << std::endl;
    }
    catch (const libconfig::SettingNotFoundException &nfex) {
        // Ignore.
    }
}

int Send2Redis(const char *ip, int port, const char *key, const char *value, int len_value) {
    printf("Enter Send2Redis\n");
    redisContext *context = redisConnect(ip, port);
    if (context == NULL || context->err) {
        if (context) {
            printf("Error: %s\n", context->errstr);
            // handle error
            return -1;
        } else {
            printf("Can't allocate redis context\n");
            return -2;
        }
    }

    printf("send date key:[%s] value[%d]:[%s]\n", key, len_value, value);
    redisReply *reply;
    redisAppendCommand(context, "SET %s %b", key, value, (size_t) len_value);
    if (REDIS_OK != redisGetReply(context, (void **) &reply))// reply for SET
    {
        printf("ERR:after set: reply:[%s]\n", reply->str);
        freeReplyObject(reply);
        goto ERR;
    }
    printf("after set: reply:[%s]\n", reply->str);
    freeReplyObject(reply);

    printf("return \n");
    ERR:
    redisFree(context);
    return 0;
}

int ReceivFromRedis(const char *ip, int port, const char *key, char **rvalue, int *len_rvalue) {
    redisContext *context = redisConnect(ip, port);
    if (context == NULL || context->err) {
        if (context) {
            printf("Error: %s\n", context->errstr);
            // handle error
            return -1;
        } else {
            printf("Can't allocate redis context\n");
            return -2;
        }
    }

    redisReply *reply;

    redisAppendCommand(context, "GET %s ", key);
    if (REDIS_OK != redisGetReply(context, (void **) &reply))// reply for SET
    {
        printf("ERR:after set: reply:[%s]\n", reply->str);
        freeReplyObject(reply);
        goto ERR;
    }
    //printf("after GET: reply[%d]:[%s]\n",reply->len,reply->str);
    printf("after GET: replylen[%d]\n", reply->len);
    *rvalue = (char *) malloc(sizeof(char) * reply->len);
    memcpy(*rvalue, reply->str, sizeof(char) * reply->len);
    *len_rvalue = reply->len;
    freeReplyObject(reply);

    printf("return \n");
    ERR:
    redisFree(context);
    return 0;
}


void testMySQL() {
    MYSQL mysql;
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "localhost", "root", "I do not know", "demo", 0, NULL, 0)) {
        printf("Connect success\n");
        mysql_close(&mysql);
    } else {
        fprintf(stderr, "Connect failed:\n");
        if (mysql_errno(&mysql)) {
            printf("\terror code is %d\n\treason:%s\n", mysql_errno(&mysql), mysql_error(&mysql));
        }
    }
}

#define    DATABASE    "sequence.db"
#define    SEQUENCE    "my_sequence"

void testLibDb(int argc, char *argv[]) {
    extern int optind;
    DB *dbp;
    DB_SEQUENCE *seq;
    DBT key;
    int ch, i, ret, rflag;
    db_seq_t seqnum;
    const char *database, *progname = "ex_sequence";

    dbp = nullptr;
    seq = nullptr;

    rflag = 0;
    while ((ch = getopt(argc, argv, "r")) != EOF)
        switch (ch) {
            case 'r':
                rflag = 1;
                break;
            case '?':
            default:
                return;
        }
    argc -= optind;
    argv += optind;

    /* Accept optional database name. */
    database = *argv == nullptr ? DATABASE : argv[0];

    /* Optionally discard the database. */
    if (rflag)
        (void) remove(database);

    /* Create and initialize database object, open the database. */
    if ((ret = db_create(&dbp, nullptr, 0)) != 0) {
        fprintf(stderr, "%s: db_create: %s\n", progname, db_strerror(ret));
        return;
    }
    dbp->set_errfile(dbp, stderr);
    dbp->set_errpfx(dbp, progname);
    if ((ret = dbp->open(dbp, nullptr, database, nullptr, DB_BTREE, DB_CREATE, 0664)) != 0) {
        dbp->err(dbp, ret, "%s: open", database);
        goto err;
    }

    if ((ret = db_sequence_create(&seq, dbp, 0)) != 0) {
        dbp->err(dbp, ret, "db_sequence_create");
        goto err;
    }

    memset(&key, 0, sizeof(DBT));
    key.data = (void *) SEQUENCE;
    key.size = (u_int32_t) strlen(SEQUENCE);

    if ((ret = seq->open(seq, nullptr, &key, DB_CREATE)) != 0) {
        dbp->err(dbp, ret, "%s: DB_SEQUENCE->open", SEQUENCE);
        goto err;
    }

    for (i = 0; i < 10; i++) {
        if ((ret = seq->get(seq, nullptr, 1, &seqnum, 0)) != 0) {
            dbp->err(dbp, ret, "DB_SEQUENCE->get");
            goto err;
        }
    }

    /* Close everything down. */
    if ((ret = seq->close(seq, 0)) != 0) {
        seq = nullptr;
        dbp->err(dbp, ret, "DB_SEQUENCE->close");
        goto err;
    }
    if ((ret = dbp->close(dbp, 0)) != 0) {
        fprintf(stderr,
                "%s: DB->close: %s\n", progname, db_strerror(ret));
        return;
    }
    return;

    err:
    if (seq != nullptr)
        (void) seq->close(seq, 0);
    if (dbp != nullptr)
        (void) dbp->close(dbp, 0);
    return;

}

class TimerExample {
public:
    void onTimer(Poco::Timer &timer) {
        std::cout << "onTimer called." << std::endl;
    }
};

void testPOCO() {
    long totalTime = 2000,
            startInterval = 0,
            periodicInterval = 0;

    TimerExample te;
    Poco::Timer timer(startInterval, periodicInterval);
    timer.start(Poco::TimerCallback<TimerExample>(te, &TimerExample::onTimer));
    Poco::Thread::sleep(totalTime);
    return;
}


int testZMQServr() {
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");
    assert (rc == 0);

    while (1) {
        char buffer[10];
        zmq_recv(responder, buffer, 10, 0);
        printf("Received Hello\n");
        sleep(1);          //  Do some 'work'
        zmq_send(responder, "World", 5, 0);
    }
    return 0;
}

int testZMQClient() {
    printf("Connecting to hello world server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer[10];
        printf("Sending Hello %d…\n", request_nbr);
        zmq_send(requester, "Hello", 5, 0);
        zmq_recv(requester, buffer, 10, 0);
        printf("Received World %d\n", request_nbr);
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}

void testunwind() {
    unw_context_t ctx = {0};
    unw_cursor_t cursor = {0};
    unw_word_t offset = 0;
    unw_word_t pc = 0;
    int8_t func_name[128] = {0};
    int32_t ret = 0;

    printf("\n----------------------stack backtrace----------------------------\n");
    ret = unw_getcontext(&ctx);
    if (0 != ret) {
        printf("get context failed\n");
        return;
    }

    ret = unw_init_local(&cursor, &ctx);
    if (0 != ret) {
        printf("init local cursor failed\n");
        return;
    }

    while (0 < unw_step(&cursor)) {
        ret = unw_get_proc_name(&cursor, reinterpret_cast<char *>(func_name), sizeof(func_name), &offset);
        if (0 != ret) {
            printf("can not get func name\n");
        } else {
            unw_get_reg(&cursor, UNW_REG_IP, &pc);
            printf("0x%lx:(%s+0x%lx)\n", pc, func_name, offset);
        }
    }
}

// see http://llvm.org/docs/LibFuzzer.html
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    try {
        // step 1: parse input
        nlohmann::json const j1 = nlohmann::json::parse(data, data + size);

        try {
            // step 2: round trip

            // first serialization
            std::string const s1 = j1.dump();

            // parse serialization
            nlohmann::json const j2 = nlohmann::json::parse(s1);

            // second serialization
            std::string const s2 = j2.dump();

            // serializations must match
            assert(s1 == s2);
        }
        catch (const nlohmann::json::parse_error &) {
            // parsing a JSON serialization must not fail
            assert(false);
        }
    }
    catch (const nlohmann::json::parse_error &) {
        // parse errors are ok, because input may be random bytes
    }
    catch (const nlohmann::json::out_of_range &) {
        // out of range errors may happen if provided sizes are excessive
    }

    // return 0 - non-zero return values are reserved for future use
    return 0;
}

int testJsonCPP() {
    Json::Value root;
    Json::Value data;
    constexpr bool shouldUseOldWay = false;
    root["action"] = "run";
    data["number"] = 1;
    root["data"] = data;

    if (shouldUseOldWay) {
        Json::FastWriter writer;
        const std::string json_file = writer.write(root);
        std::cout << json_file << std::endl;
    } else {
        Json::StreamWriterBuilder builder;
        const std::string json_file = Json::writeString(builder, root);
        std::cout << json_file << std::endl;
    }
    return EXIT_SUCCESS;
}

int testSQLite3() {
    sqlite3 *db;
    char *zErrMsg = NULL;
    int rv;
    char szSql[128] = {0};
    sqlite3_stmt *stmt;
    rv = sqlite3_open("./test.db", &db);
    if (rv) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    } else
        fprintf(stderr, "sqlite3_open OK\n");
    //这里查询时，假定数据库中存在表test，并且有一列为COL1(text类型)
    strcpy(szSql, "SELECT COL1 FROM test where COL1=?");
    rv = sqlite3_prepare(db, szSql, 128, &stmt, NULL);
    if (rv != SQLITE_OK) {
        fprintf(stderr, "sqlite3_prepare(%d): %s\n", rv, sqlite3_errmsg(db));
        return -1;
    }
    rv = sqlite3_bind_text(stmt, 1, "VALUE1", strlen("VALUE1"), SQLITE_STATIC);
    if (rv != SQLITE_OK) {
        fprintf(stderr, "sqlite3_bind_text(%d): %s\n", rv, sqlite3_errmsg(db));
        return -1;
    }
    rv = sqlite3_step(stmt);
    if ((rv != SQLITE_OK) && (rv != SQLITE_DONE) && (rv != SQLITE_ROW)) {
        fprintf(stderr, "sqlite3_step(%d): %s\n", rv, sqlite3_errmsg(db));
        return -1;
    }
    while (rv == SQLITE_ROW) {
        fprintf(stderr, "result: %s\n", sqlite3_column_text(stmt, 0));
        rv = sqlite3_step(stmt);
    }
    rv = sqlite3_finalize(stmt);
    if (rv != SQLITE_OK) {
        fprintf(stderr, "sqlite3_finalize(%d): %s\n", rv, sqlite3_errmsg(db));
        return -1;
    }
    sqlite3_close(db);
    return 0;
}

//! Matrix 2x2 class
struct Matrix2x2 {
    //! Array of values
    int64_t v[2][2];

    Matrix2x2() {}

    Matrix2x2(int64_t v00, int64_t v01, int64_t v10, int64_t v11) {
        v[0][0] = v00;
        v[0][1] = v01;
        v[1][0] = v10;
        v[1][1] = v11;
    }

    Matrix2x2 operator*(const Matrix2x2 &to) const; //< Multiply two Matrices
};

//! Identity matrix
static const Matrix2x2 MatrixIdentity(1, 0, 0, 1);
//! Default matrix to multiply
static const Matrix2x2 Matrix1110(1, 1, 1, 0);

//! Raw arrays matrices multiply
void Matrix2x2Multiply(const int64_t a[2][2], const int64_t b[2][2], int64_t c[2][2]);

/////////////////////// Serial methods ////////////////////////

//! Plain serial sum
int64_t SerialFib(int n) {
    if (n < 2)
        return n;
    int64_t a = 0, b = 1, sum;
    int i;
    for (i = 2; i <= n; i++) { // n is really index of Fibonacci number
        sum = a + b;
        a = b;
        b = sum;
    }
    return sum;
}

//! Serial n-1 matrices multiplication
int64_t SerialMatrixFib(int n) {
    int64_t c[2][2], a[2][2] = {{1, 1},
                                {1, 0}}, b[2][2] = {{1, 1},
                                                    {1, 0}};
    int i;
    for (i = 2; i < n; i++) { // Using condition to prevent copying of values
        if (i & 1)
            Matrix2x2Multiply(a, c, b);
        else
            Matrix2x2Multiply(a, b, c);
    }
    return (i & 1) ? c[0][0] : b[0][0]; // get result from upper left cell
}

//! Recursive summing. Just for complete list of serial algorithms, not used
int64_t SerialRecursiveFib(int n) {
    int64_t result;
    if (n < 2)
        result = n;
    else
        result = SerialRecursiveFib(n - 1) + SerialRecursiveFib(n - 2);
    return result;
}

// GCC 4.8 C++ standard library implements std::this_thread::yield as no-op.
#if __TBB_GLIBCXX_THIS_THREAD_YIELD_BROKEN
static inline void yield() {
    sched_yield();
}
#else
using std::this_thread::yield;
#endif

//! Introducing of queue method in serial
int64_t SerialQueueFib(int n) {
    oneapi::tbb::concurrent_queue<Matrix2x2> Q;
    for (int i = 1; i < n; i++)
        Q.push(Matrix1110);
    Matrix2x2 A, B;
    while (true) {
        while (!Q.try_pop(A))
            yield();
        if (Q.empty())
            break;
        while (!Q.try_pop(B))
            yield();
        Q.push(A * B);
    }
    return A.v[0][0];
}

//! Trying to use concurrent_vector
int64_t SerialVectorFib(int n) {
    oneapi::tbb::concurrent_vector<int64_t> A;
    A.grow_by(2);
    A[0] = 0;
    A[1] = 1;
    for (int i = 2; i <= n; i++) {
        A.grow_to_at_least(i + 1);
        A[i] = A[i - 1] + A[i - 2];
    }
    return A[n];
}

///////////////////// Parallel methods ////////////////////////

// *** Serial shared by mutexes *** //

//! Shared glabals
int64_t SharedA = 0, SharedB = 1;
int SharedI = 1, SharedN;

//! Template task class which computes Fibonacci numbers with shared globals
template<typename M>
class SharedSerialFibBody {
    M &mutex;

public:
    SharedSerialFibBody(M &m) : mutex(m) {}

    //! main loop
    void operator()(const oneapi::tbb::blocked_range<int> &range) const {
        for (;;) {
            typename M::scoped_lock lock(mutex);
            if (SharedI >= SharedN)
                break;
            int64_t sum = SharedA + SharedB;
            SharedA = SharedB;
            SharedB = sum;
            ++SharedI;
        }
    }
};

template<>
void SharedSerialFibBody<std::mutex>::operator()(
        const oneapi::tbb::blocked_range<int> &range) const {
    for (;;) {
        std::lock_guard<std::mutex> lock(mutex);
        if (SharedI >= SharedN)
            break;
        int64_t sum = SharedA + SharedB;
        SharedA = SharedB;
        SharedB = sum;
        ++SharedI;
    }
}

//! Root function
template<class M>
int64_t SharedSerialFib(int n) {
    SharedA = 0;
    SharedB = 1;
    SharedI = 1;
    SharedN = n;
    M mutex;
    parallel_for(oneapi::tbb::blocked_range<int>(0, 4, 1), SharedSerialFibBody<M>(mutex));
    return SharedB;
}

// *** Serial shared by concurrent hash map *** //

//! Hash comparer
struct IntHashCompare {
    bool equal(const int j, const int k) const {
        return j == k;
    }

    std::size_t hash(const int k) const {
        return (std::size_t) k;
    }
};

//! NumbersTable type based on concurrent_hash_map
typedef oneapi::tbb::concurrent_hash_map<int, int64_t, IntHashCompare> NumbersTable;

//! task for serial method using shared concurrent_hash_map
class ConcurrentHashSerialFibTask {
    NumbersTable &Fib;
    int my_n;

public:
    //! constructor
    ConcurrentHashSerialFibTask(NumbersTable &cht, int n) : Fib(cht), my_n(n) {}

    //! executing task
    void operator()() const {
        for (int i = 2; i <= my_n; ++i) { // there is no difference in to recycle or to make loop
            NumbersTable::const_accessor f1, f2; // same as iterators
            if (!Fib.find(f1, i - 1) || !Fib.find(f2, i - 2)) {
                // Something is seriously wrong, because i-1 and i-2 must have been inserted
                // earlier by this thread or another thread.
                assert(0);
            }
            int64_t sum = f1->second + f2->second;
            NumbersTable::const_accessor fsum;
            Fib.insert(fsum, std::make_pair(i, sum)); // inserting
            assert(fsum->second == sum); // check value
        }
    }
};

//! Root function
int64_t ConcurrentHashSerialFib(int n) {
    NumbersTable Fib;
    bool okay;
    okay = Fib.insert(std::make_pair(0, 0));
    assert(okay); // assign initial values
    okay = Fib.insert(std::make_pair(1, 1));
    assert(okay);

    // task_list list;
    oneapi::tbb::task_group tg;
    // allocate tasks
    tg.run(ConcurrentHashSerialFibTask(Fib, n));
    tg.run(ConcurrentHashSerialFibTask(Fib, n));
    tg.wait();
    NumbersTable::const_accessor fresult;
    okay = Fib.find(fresult, n);
    assert(okay);
    return fresult->second;
}

// *** Queue with parallel_pipeline *** //

typedef oneapi::tbb::concurrent_queue<Matrix2x2> queue_t;
namespace parallel_pipeline_ns {
    std::atomic<int> N; //< index of Fibonacci number minus 1
    queue_t Queue;
} // namespace parallel_pipeline_ns

//! functor to fills queue
struct InputFunc {
    InputFunc() {}

    queue_t *operator()(oneapi::tbb::flow_control &fc) const {
        using namespace parallel_pipeline_ns;

        int n = --N;
        if (n <= 0) {
            fc.stop();
            return nullptr;
        }
        Queue.push(Matrix1110);
        return &Queue;
    }
};

//! functor to process queue
struct MultiplyFunc {
    MultiplyFunc() {}

    void operator()(queue_t *queue) const {
        //concurrent_queue<Matrix2x2> &Queue = *static_cast<concurrent_queue<Matrix2x2> *>(p);
        Matrix2x2 m1, m2;
        // get two elements
        while (!queue->try_pop(m1))
            yield();
        while (!queue->try_pop(m2))
            yield();
        m1 = m1 * m2; // process them
        queue->push(m1); // and push back
    }
};

//! Root function
int64_t ParallelPipeFib(int n) {
    using namespace parallel_pipeline_ns;

    N = n - 1;
    Queue.push(Matrix1110);

    oneapi::tbb::parallel_pipeline(
            n,
            oneapi::tbb::make_filter<void, queue_t *>(oneapi::tbb::filter_mode::parallel, InputFunc()) &
            oneapi::tbb::make_filter<queue_t *, void>(oneapi::tbb::filter_mode::parallel,
                                                      MultiplyFunc()));

    assert(Queue.unsafe_size() == 1);
    Matrix2x2 M;
    bool result = Queue.try_pop(M); // get last element
    assert(result);
    int64_t res = M.v[0][0]; // get value
    Queue.clear();
    return res;
}

// *** parallel_reduce *** //

//! Functor for parallel_reduce
struct parallel_reduceFibBody {
    Matrix2x2 sum;
    int split_flag; //< flag to make one less operation for split bodies
    //! Constructor fills sum with initial matrix
    parallel_reduceFibBody() : sum(Matrix1110), split_flag(0) {}

    //! Splitting constructor
    parallel_reduceFibBody(parallel_reduceFibBody &other, oneapi::tbb::split)
            : sum(Matrix1110),
              split_flag(1 /*note that it is split*/) {}

    //! Join point
    void join(parallel_reduceFibBody &s) {
        sum = sum * s.sum;
    }

    //! Process multiplications
    void operator()(const oneapi::tbb::blocked_range<int> &r) {
        for (int k = r.begin() + split_flag; k < r.end(); ++k)
            sum = sum * Matrix1110;
        split_flag = 0; // reset flag, because this method can be reused for next range
    }
};

//! Root function
int64_t parallel_reduceFib(int n) {
    parallel_reduceFibBody b;
    oneapi::tbb::parallel_reduce(oneapi::tbb::blocked_range<int>(2, n, 3),
                                 b); // do parallel reduce on range [2, n) for b
    return b.sum.v[0][0];
}

// *** parallel_scan *** //

//! Functor for parallel_scan
struct parallel_scanFibBody {
    /** Though parallel_scan is usually used to accumulate running sums,
        it can be used to accumulate running products too. */
    Matrix2x2 product;
    /** Pointer to output sequence */
    int64_t *const output;

    //! Constructor sets product to identity matrix
    parallel_scanFibBody(int64_t *output_) : product(MatrixIdentity), output(output_) {}

    //! Splitting constructor
    parallel_scanFibBody(parallel_scanFibBody &b, oneapi::tbb::split)
            : product(MatrixIdentity),
              output(b.output) {}

    //! Method for merging summary information from a, which was split off from *this, into *this.
    void reverse_join(parallel_scanFibBody &a) {
        // When using non-commutative reduction operation, reverse_join
        // should put argument "a" on the left side of the operation.
        // The reversal from the argument order is why the method is
        // called "reverse_join" instead of "join".
        product = a.product * product;
    }

    //! Method for assigning final result back to original body.
    void assign(parallel_scanFibBody &b) {
        product = b.product;
    }
    //! Compute matrix running product.
    /** Tag indicates whether is is the final scan over the range, or
        just a helper "prescan" that is computing a partial reduction. */
    template<typename Tag>
    void operator()(const oneapi::tbb::blocked_range<int> &r, Tag tag) {
        for (int k = r.begin(); k < r.end(); ++k) {
            // Code performs an "exclusive" scan, which outputs a value *before* updating the product.
            // For an "inclusive" scan, output the value after the update.
            if (tag.is_final_scan())
                output[k] = product.v[0][1];
            product = product * Matrix1110;
        }
    }
};

//! Root function
int64_t parallel_scanFib(int n) {
    int64_t *output = new int64_t[n];
    parallel_scanFibBody b(output);
    oneapi::tbb::parallel_scan(oneapi::tbb::blocked_range<int>(0, n, 3), b);
    // output[0..n-1] now contains the Fibonacci sequence (modulo integer wrap-around).
    // Check the last two values for correctness.
    assert(n < 2 || output[n - 2] + output[n - 1] == b.product.v[0][1]);
    delete[] output;
    return b.product.v[0][1];
}

/////////////////////////// Main ////////////////////////////////////////////////////

//! A closed range of int.
struct IntRange {
    int low;
    int high;

    void set_from_string(const char *s);

    IntRange(int low_, int high_) : low(low_), high(high_) {}
};

void IntRange::set_from_string(const char *s) {
    char *end;
    high = low = strtol(s, &end, 0);
    switch (*end) {
        case ':':
            high = strtol(end + 1, nullptr, 0);
            break;
        case '\0':
            break;
        default:
            printf("unexpected character = %c\n", *end);
    }
}

//! Tick count for start
static oneapi::tbb::tick_count t0;

//! Verbose output flag
static bool Verbose = false;

typedef int64_t (*MeasureFunc)(int);

//! Measure ticks count in loop [2..n]
int64_t Measure(const char *name, MeasureFunc func, int n) {
    int64_t result;
    if (Verbose)
        printf("%s", name);
    t0 = oneapi::tbb::tick_count::now();
    for (int number = 2; number <= n; number++)
        result = func(number);
    if (Verbose)
        printf("\t- in %f msec\n", (oneapi::tbb::tick_count::now() - t0).seconds() * 1000);
    return result;
}

//! program entry
int testOneTBB(int argc, char *argv[]) {
    if (argc > 1)
        Verbose = true;
    int NumbersCount = argc > 1 ? strtol(argv[1], nullptr, 0) : 500;
    IntRange NThread(1, 4); // Number of threads to use.
    if (argc > 2)
        NThread.set_from_string(argv[2]);
    unsigned long ntrial = argc > 3 ? (unsigned long) strtoul(argv[3], nullptr, 0) : 1;
    int64_t result, sum;

    if (Verbose)
        printf("Fibonacci numbers example. Generating %d numbers..\n", NumbersCount);

    result = Measure("Serial loop", SerialFib, NumbersCount);
    sum = Measure("Serial matrix", SerialMatrixFib, NumbersCount);
    assert(result == sum);
    sum = Measure("Serial vector", SerialVectorFib, NumbersCount);
    assert(result == sum);
    sum = Measure("Serial queue", SerialQueueFib, NumbersCount);
    assert(result == sum);
    // now in parallel
    for (unsigned long i = 0; i < ntrial; ++i) {
        for (int threads = NThread.low; threads <= NThread.high; threads *= 2) {
            oneapi::tbb::global_control c(oneapi::tbb::global_control::max_allowed_parallelism,
                                          threads);
            if (Verbose)
                printf("\nThreads number is %d\n", threads);

            sum = Measure("Shared serial (mutex)\t", SharedSerialFib<std::mutex>, NumbersCount);
            assert(result == sum);
            sum = Measure("Shared serial (spin_mutex)",
                          SharedSerialFib<oneapi::tbb::spin_mutex>,
                          NumbersCount);
            assert(result == sum);
            sum = Measure("Shared serial (queuing_mutex)",
                          SharedSerialFib<oneapi::tbb::queuing_mutex>,
                          NumbersCount);
            assert(result == sum);
            sum = Measure("Shared serial (Conc.HashTable)", ConcurrentHashSerialFib, NumbersCount);
            assert(result == sum);
            sum = Measure("Parallel pipe/queue\t", ParallelPipeFib, NumbersCount);
            assert(result == sum);
            sum = Measure("Parallel reduce\t\t", parallel_reduceFib, NumbersCount);
            assert(result == sum);
            sum = Measure("Parallel scan\t\t", parallel_scanFib, NumbersCount);
            assert(result == sum);
        }

#ifdef __GNUC__
        if (Verbose)
            printf("Fibonacci number #%d modulo 2^64 is %lld\n\n", NumbersCount, result);
#else
        if (Verbose)
            printf("Fibonacci number #%d modulo 2^64 is %I64d\n\n", NumbersCount, result);
#endif
    }
    if (!Verbose)
        printf("TEST PASSED\n");
    // flush to prevent bufferization on exit
    fflush(stdout);
    return 0;
}

// Utils

void Matrix2x2Multiply(const int64_t a[2][2], const int64_t b[2][2], int64_t c[2][2]) {
    for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++)
            c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j];
}

Matrix2x2 Matrix2x2::operator*(const Matrix2x2 &to) const {
    Matrix2x2 result;
    Matrix2x2Multiply(v, to.v, result.v);
    return result;
}

// every watcher type has its own typedef'd struct
// with the name ev_TYPE
ev_io stdin_watcher;
ev_timer timeout_watcher;

// all watcher callbacks have a similar signature
// this callback is called when data is readable on stdin
static void stdin_cb(EV_P_ ev_io *w, int revents) {
    puts("stdin ready");
    // for one-shot events, one must manually stop the watcher
    // with its corresponding stop function.
    ev_io_stop(EV_A_ w);
    // this causes all nested ev_run's to stop iterating
    ev_break(EV_A_ EVBREAK_ALL);
}

// another callback, this time for a time-out
static void timeout_cb(EV_P_ ev_timer *w, int revents) {
    puts("timeout");
    // this causes the innermost ev_run to stop iterating
    ev_break(EV_A_ EVBREAK_ONE);
}

int testLibEV(void) {
    // use the default event loop unless you have special needs
    struct ev_loop *loop = EV_DEFAULT;
    // initialise an io watcher, then start it
    // this one will watch for stdin to become readable
    ev_io_init (&stdin_watcher, stdin_cb,
    /* STDIN_FILENO*/ 0, EV_READ);
    ev_io_start(loop, &stdin_watcher);
    // initialise a timer watcher, then start it
    // simple non-repeating 5.5 second timeout
    ev_timer_init (&timeout_watcher, timeout_cb, 5.5, 0.);
    ev_timer_start(loop, &timeout_watcher);
    // now wait for events to arrive
    ev_run(loop, 0);
    // break was called, so exit
    return 0;
}

/***************************************************************************************************
动态库：libev nanomsg
概述：A B C 三个线程通过nanomsg通信，A线程作为主线程，控制中枢，B C请求均通过A.
demo示范：
    A为指令处理模块
    B为指令接收模块
    C为指令执行模块
    B -> A   开灯
    A -> C   开灯
    C :      执行开灯
    C -> A   OK
    A -> B   OK
总结：
    这只是简单的测试使用例子，你可以通过在这个框架的基础上做更多的功能，对于多线程编程这将是一个不
错的选择.
***************************************************************************************************/

typedef struct {
    int n;          //nanomsg socket
    int s;          //nanomsg recieve fd
} nanomsg_info_t;

typedef struct {
    nanomsg_info_t ab;
    nanomsg_info_t ac;
} Aloop_ctrl_t;

typedef struct {
    nanomsg_info_t ba;
} Bloop_ctrl_t;

typedef struct {
    nanomsg_info_t ca;
} Cloop_ctrl_t;

/*获取系统时间打印*/
uint32_t print_timenow() {
    time_t now;
    struct tm *tm_now;
    time(&now);
    tm_now = localtime(&now);
    uint32_t times = tm_now->tm_hour * 3600 + tm_now->tm_min * 60 + tm_now->tm_sec;
    printf("[%02d:%02d:%02d]\r\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
    return times;
}

/*****************************************子线程C相关**********************************************/
static void watcher_c_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    void *user_data = ev_userdata(loop);
    Cloop_ctrl_t *Cloop_ctrl = (Cloop_ctrl_t *) user_data;
    uint8_t *dat = nullptr;
    uint32_t bytes = nn_recv(Cloop_ctrl->ca.n, &dat, NN_MSG, NN_DONTWAIT);
    if (bytes <= 0) {
        return;
    }
    printf("C:%s (A->C)\r\n", (char *) dat);
    nn_freemsg(dat);
    //接收成功，发送OK
    char *str = "OK";
    uint8_t *udata = static_cast<uint8_t *>(nn_allocmsg(3, 0));
    if (nullptr != udata) {
        memcpy(udata, str, 3);
        nn_send(Cloop_ctrl->ca.n, &udata, NN_MSG, NN_DONTWAIT);
    }

}

int C_nanomsg_init(Cloop_ctrl_t *Cloop_ctrl) {
    Cloop_ctrl->ca.n = nn_socket(AF_SP, NN_PAIR);
    if (Cloop_ctrl->ca.n < 0) {
        return -1;
    }
    if (nn_connect(Cloop_ctrl->ca.n, "inproc://c2a_loop") < 0) {
        return -1;
    }
    size_t size = sizeof(size_t);
    if (nn_getsockopt(Cloop_ctrl->ca.n, NN_SOL_SOCKET, NN_RCVFD, (char *) &Cloop_ctrl->ca.s, &size) < 0) {
        return -1;
    }
    return 0;
}

struct ev_loop *C_loop_init(Cloop_ctrl_t *Cloop_ctrl) {
    static struct ev_io watcher_c;
    struct ev_loop *loop = ev_loop_new(EVBACKEND_EPOLL);
    if (nullptr == loop) {
        printf("create C loop failed\r\n");
        return nullptr;
    }
    ev_io_init (&watcher_c, watcher_c_cb, Cloop_ctrl->ca.s, EV_READ);
    ev_io_start(loop, &watcher_c);
    return loop;
}

void *C_thread(void *arg) {
    Cloop_ctrl_t Cloop_ctrl;
    if (C_nanomsg_init(&Cloop_ctrl) < 0) {
        printf("nanomsg init failed\r\n");
        return nullptr;
    }
    struct ev_loop *Cloop = C_loop_init(&Cloop_ctrl);
    if (nullptr == Cloop) {
        printf("Cloop init failed\r\n");
        return nullptr;
    }
    ev_set_userdata(Cloop, &Cloop_ctrl);
    ev_run(Cloop, 0);
    return nullptr;
}

/*****************************************子线程B相关**********************************************/
static void watcher_b_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    void *user_data = ev_userdata(loop);
    Bloop_ctrl_t *Bloop_ctrl = (Bloop_ctrl_t *) user_data;
    uint8_t *dat = nullptr;
    uint32_t bytes = nn_recv(Bloop_ctrl->ba.n, &dat, NN_MSG, NN_DONTWAIT);
    if (bytes <= 0) {
        return;
    }
    printf("B:%s (A->B)\r\n\r\n", (char *) dat);
    nn_freemsg(dat);
}

static void watcher_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents) {
    static int i = 1;
    char send_data[128] = {0};
    void *user_data = ev_userdata(loop);
    Bloop_ctrl_t *Bloop_ctrl = (Bloop_ctrl_t *) user_data;
    sprintf(send_data, "Please turn on LED[%d]", i);
    i++;
    int length = strlen(send_data) + 1;
    uint8_t *udata = static_cast<uint8_t *>(nn_allocmsg(length, 0));
    if (nullptr != udata) {
        memcpy(udata, send_data, length);
        nn_send(Bloop_ctrl->ba.n, &udata, NN_MSG, NN_DONTWAIT);
    }
    //如果定时器不重设，就会默认1秒进入一次回调
    w->repeat = 10;
    ev_timer_again(loop, w);
}


int B_nanomsg_init(Bloop_ctrl_t *Bloop_ctrl) {
    Bloop_ctrl->ba.n = nn_socket(AF_SP, NN_PAIR);
    if (Bloop_ctrl->ba.n < 0) {
        return -1;
    }
    if (nn_connect(Bloop_ctrl->ba.n, "inproc://b2a_loop") < 0) {
        return -1;
    }
    size_t size = sizeof(size_t);
    if (nn_getsockopt(Bloop_ctrl->ba.n, NN_SOL_SOCKET, NN_RCVFD, (char *) &Bloop_ctrl->ba.s, &size) < 0) {
        return -1;
    }
    return 0;
}

struct ev_loop *B_loop_init(Bloop_ctrl_t *Bloop_ctrl) {
    static struct ev_io watcher_b;
    static struct ev_timer watcher_timer;
    struct ev_loop *loop = ev_loop_new(EVBACKEND_EPOLL);
    if (nullptr == loop) {
        printf("create loop failed\r\n");
        return nullptr;
    }
    ev_io_init (&watcher_b, watcher_b_cb, Bloop_ctrl->ba.s, EV_READ);
    ev_timer_init(&watcher_timer, watcher_timer_cb, 5, 1);
    ev_io_start(loop, &watcher_b);
    ev_timer_start(loop, &watcher_timer);
    return loop;
}

void *B_thread(void *arg) {
    Bloop_ctrl_t Bloop_ctrl;
    if (B_nanomsg_init(&Bloop_ctrl) < 0) {
        printf("nanomsg init failed\r\n");
        return nullptr;
    }
    struct ev_loop *Bloop = B_loop_init(&Bloop_ctrl);
    if (nullptr == Bloop) {
        printf("Bloop init failed\r\n");
        return nullptr;
    }
    ev_set_userdata(Bloop, &Bloop_ctrl);
    ev_run(Bloop, 0);
    return nullptr;
}

/*****************************************主线程A相关**********************************************/
static void watcher_ab_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    void *user_data = ev_userdata(loop);
    Aloop_ctrl_t *Aloop_ctrl = (Aloop_ctrl_t *) user_data;
    uint8_t *dat = nullptr;
    uint32_t bytes = nn_recv(Aloop_ctrl->ab.n, &dat, NN_MSG, NN_DONTWAIT);
    if (bytes <= 0) {
        return;
    }
    //转发到C
    printf("A:%s (B->A)\r\n", (char *) dat);
    nn_send(Aloop_ctrl->ac.n, &dat, NN_MSG, NN_DONTWAIT);
}

static void watcher_ac_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    void *user_data = ev_userdata(loop);
    Aloop_ctrl_t *Aloop_ctrl = (Aloop_ctrl_t *) user_data;
    uint8_t *dat = nullptr;
    uint32_t bytes = nn_recv(Aloop_ctrl->ac.n, &dat, NN_MSG, NN_DONTWAIT);
    if (bytes <= 0) {
        return;
    }
    //转发到B
    printf("A:%s (C->A)\r\n", (char *) dat);
    nn_send(Aloop_ctrl->ab.n, &dat, NN_MSG, NN_DONTWAIT);
}

/*主事件nanomsg初始化*/
int A_nanomsg_init(Aloop_ctrl_t *Aloop_ctrl) {
    //ab通信的nanomsg初始化
    Aloop_ctrl->ab.n = nn_socket(AF_SP, NN_PAIR);
    if (Aloop_ctrl->ab.n < 0) {
        return -1;
    }
    if (nn_bind(Aloop_ctrl->ab.n, "inproc://b2a_loop") < 0) {
        return -1;
    }
    //获取此端口的接收数据fd描述符
    size_t size = sizeof(size_t);
    if (nn_getsockopt(Aloop_ctrl->ab.n, NN_SOL_SOCKET, NN_RCVFD, (char *) &Aloop_ctrl->ab.s, &size) < 0) {
        return -1;
    }
    //ac通信的nanomsg初始化
    Aloop_ctrl->ac.n = nn_socket(AF_SP, NN_PAIR);
    if (Aloop_ctrl->ac.n < 0) {
        return -1;
    }
    if (nn_bind(Aloop_ctrl->ac.n, "inproc://c2a_loop") < 0) {
        return -1;
    }
    //获取此端口的接收数据fd描述符
    if (nn_getsockopt(Aloop_ctrl->ac.n, NN_SOL_SOCKET, NN_RCVFD, (char *) &Aloop_ctrl->ac.s, &size) < 0) {
        return -1;
    }
    return 0;
}

/*主事件循环初始化*/
struct ev_loop *A_loop_init(Aloop_ctrl_t *Aloop_ctrl) {
    static struct ev_io watcher_ab, watcher_ac;
    struct ev_loop *loop = ev_loop_new(EVBACKEND_EPOLL);
    if (nullptr == loop) {
        printf("create loop failed\r\n");
        return nullptr;
    }
    //传参
    ev_set_userdata(loop, Aloop_ctrl);
    //初始化
    ev_io_init (&watcher_ab, watcher_ab_cb, Aloop_ctrl->ab.s, EV_READ);
    ev_io_init (&watcher_ac, watcher_ac_cb, Aloop_ctrl->ac.s, EV_READ);
    ev_io_start(loop, &watcher_ab);
    ev_io_start(loop, &watcher_ac);
    return loop;
}

/**************************************************************************************************/
int testLibEV_Nanomsg() {
    pthread_t pb, pc;
    Aloop_ctrl_t Aloop_ctrl;
    if (A_nanomsg_init(&Aloop_ctrl) < 0) {
        printf("nanomsg init failed\r\n");
        return -1;
    }
    struct ev_loop *Aloop = A_loop_init(&Aloop_ctrl);
    if (nullptr == Aloop) {
        printf("Aloop init failed\r\n");
        return -1;
    }
    //创建线程B
    if (0 != pthread_create(&pb, nullptr, B_thread, nullptr)) {
        printf("create pthread B failed\r\n");
        return -1;
    }
    //创建线程C
    if (0 != pthread_create(&pc, nullptr, C_thread, nullptr)) {
        printf("create pthread C failed\r\n");
        return -1;
    }
    //运行
    ev_run(Aloop, 0);
    return 0;
}

int testLua(int argc, char *argv[]) {
    ///< 创建lua句柄并初始化
    lua_State *pState = luaL_newstate();

    if (nullptr == pState) {
        std::cout << "Lua 初始化失败" << std::endl;
        return -1;
    }

    ///< 加载相关库文件
    luaL_openlibs(pState);

    ///< 加载lua文件
    if (luaL_loadfile(pState, "./testdata/test.lua")) {
        std::cout << "Lua 文件加载失败" << std::endl;
    } else {
        ///< 执行lua文件
        if (lua_pcall(pState, 0, 0, 0)) {
            std::cerr << lua_tostring(pState, -1) << std::endl;
        } else {
            ///< 获取值
            lua_getglobal(pState, "mystr");

            std::string str = lua_tostring(pState, -1);
            std::cout << str << std::endl;

            ///< 获取表中数据
            lua_getglobal(pState, "myTable");
            lua_getfield(pState, -1, "name");
            std::cout << lua_tostring(pState, -1) << std::endl;

            lua_getglobal(pState, "myTable");
            lua_getfield(pState, -1, "id");
            std::cout << lua_tonumber(pState, -1) << std::endl;

            ///< 调用函数
            lua_getglobal(pState, "print_hello");
            lua_pcall(pState, 0, 0, 0);

            ///< 调用计算函数
            lua_getglobal(pState, "_add");
            lua_pushnumber(pState, 10);
            lua_pushnumber(pState, 20);

            if (lua_pcall(pState, 2, 1, 0)) {
                std::cout << lua_tostring(pState, -1) << std::endl;
                lua_close(pState);
            } else {
                if (lua_isnumber(pState, -1)) {
                    std::cout << lua_tonumber(pState, -1) << std::endl;
                }
            }
        }
    }

    lua_close(pState);
    return 0;
}

int main(int argc, char *argv[]) {
    testbz2();
    std::cout << "\n---------utf8ToUnicodes---------" << std::endl;
    std::string s = "C/C++语言提供了几个标准库函数，可以将字符串转换为任意类型(整型、长整型、浮点型等)。";
    std::vector<int32_t> unicodes;
    lysutil::comutils::strUtils::utf8ToUnicodes(s, unicodes);
    std::vector<int32_t>::iterator iter;
    for (iter = unicodes.begin(); iter != unicodes.end(); iter++) {
        std::cout << *iter << "\t";
    }
    std::cout << std::endl;

    //再转回来
    std::string str;
    lysutil::comutils::strUtils::unicodesToUTF8(unicodes, str);
    std::cout << str << std::endl;


    std::cout << "\n---------base64Encode---------" << std::endl;
    std::string rawStr = "各地区、各部门要抓紧做好本地区、本领域的劳动节假期调整落实工作，采取有效措施，保障交通运力，加强旅游服务，丰富产品供给，强化综合调控，确保平稳运行。";
    std::string b64en;
    std::string b64de;
    lysutil::comutils::strUtils::base64Encode(rawStr, b64en);
    lysutil::comutils::strUtils::base64Decode(b64en, b64de);
    std::cout << rawStr << std::endl;
    std::cout << b64en << std::endl;
    std::cout << b64de << std::endl;


    std::cout << "\n---------urlEncode---------" << std::endl;
    std::string rawUrl = "http://wiki.intra.xiaojukeji.com/pages/viewpage.action?pageId=114204118#id-4.计价&收银服务-术语说明";
    std::string urlEn;
    std::string urlDe;
    lysutil::comutils::strUtils::urlEncode(rawUrl, urlEn);
    lysutil::comutils::strUtils::urlDecode(urlEn, urlDe);
    std::cout << rawUrl << std::endl;
    std::cout << urlEn << std::endl;
    std::cout << urlDe << std::endl;


    std::cout << "\n---------colorUtils---------" << std::endl;
    std::string out1;
    std::string out2;
    lysutil::comutils::colorUtils::Green(rawStr, out1, 1, 1);
    lysutil::comutils::colorUtils::Yellow(rawStr, out2, 0, 0);
    std::cout << out1 << std::endl;
    std::cout << out2 << std::endl;


    std::cout << "\n---------getTerminalSize---------" << std::endl;
    size_t screenWidth, screenHeight;
    lysutil::comutils::sysUtils::getTerminalSize(&screenWidth, &screenHeight);
    std::cout << "screenWidth=" << screenWidth << "\tscreenHeight=" << screenHeight << std::endl;


    std::cout << "\n---------trimSpace---------" << std::endl;
    std::string trimSp1 = "  c\n\t   ";
    lysutil::comutils::strUtils::trimSpace(trimSp1);
    std::cout << "trimSp=" << trimSp1 << "\tsize=" << trimSp1.size() << std::endl;
    std::string trimSp2 = "  c\n\t   ";
    lysutil::comutils::strUtils::trimLeftSpace(trimSp2);
    std::cout << "trimSp2=" << trimSp2 << "\tsize=" << trimSp2.size() << std::endl;
    std::string trimSp3 = "  c\n\t   ";
    lysutil::comutils::strUtils::trimRightSpace(trimSp3);
    std::cout << "trimSp3=" << trimSp3 << "\tsize=" << trimSp3.size() << std::endl;
    std::string trimCH1 = "abcaaaaaaa";
    lysutil::comutils::strUtils::trimChar(trimCH1, 'a');
    std::cout << "trimCH1=" << trimCH1 << "\tsize=" << trimCH1.size() << std::endl;


    std::cout << "\n-------lysutil::comutils::pcreUtils-------" << std::endl;
    lysutil::comutils::pcreUtils reg1(";");
    std::string ck = "a=c;d=f;a=adsfadfadfadfadfadf;d=453453453534;;;;;;;;-";
    std::vector<std::string> reg1SubList;
    reg1.reg_split(ck, reg1SubList);
    printVector1(reg1SubList);

    std::cout << "\n-------lysutil::comutils::strUtils::strSplit-------" << std::endl;
    std::vector<std::string> split2SubList;
    lysutil::comutils::strUtils::strSplit(ck, ';', split2SubList);
    printVector1(split2SubList);
    std::cout << std::endl;

    std::cout << "\n-------lysutil::comutils::httpr=Utils::splitRawCookie-------" << std::endl;
    std::map<std::string, std::string> cookie2SubList;
    lysutil::comutils::httpUtils::splitRawCookie(ck, cookie2SubList);
    printMap1(cookie2SubList);
    std::string ck1;
    lysutil::comutils::httpUtils::joinRawCookie(cookie2SubList, ck1);
    std::cout << ck1 << std::endl;
    std::cout << std::endl;

    std::cout << "\n---------/dev/urandom---------" << std::endl;
    std::string deviceRandom1;
    lysutil::comutils::sysUtils::getDeviceRandom(deviceRandom1, 30);
    std::cout << deviceRandom1 << std::endl;
    std::cout << std::endl;

    std::cout << "\n---------local ip---------" << std::endl;
    std::vector<std::string> localIP;
    std::string hostName;
    lysutil::comutils::sysUtils::getLocalIP(localIP, hostName);
    printVector1(localIP);
    std::cout << "hostName=" << hostName << std::endl;
    std::cout << std::endl;

    std::cout << "\n---------clock---------" << std::endl;
    uint64_t ms, ns;
    lysutil::comutils::sysUtils::getMicroSeconds(ms);
    lysutil::comutils::sysUtils::getNanoSeconds(ns);
    std::cout << "ms=" << ms << "\tns=" << ns << std::endl;
    std::cout << std::endl;

    std::cout << "\n---------random---------" << std::endl;
    size_t i;
    for (i = 0; i < 10; i++) {
        int r = lysutil::comutils::sysUtils::getRandom();
        std::cout << "random int=" << r << std::endl;
    }
    std::cout << std::endl;


    std::cout << "\n---------isFileExists---------" << std::endl;
    const char *fs[] = {
            "./str.cpp",
            "./abc",
            "/dev",
            "wendao",
    };
    size_t fsLen = sizeof(fs) / sizeof(char *);
    for (i = 0; i < fsLen; i++) {
        const char *f = fs[i];
        bool r = lysutil::comutils::fileUtils::isFileExist(f);
        std::cout << "file[" << f << "] isExist:" << r << std::endl;
        std::string bname;
        lysutil::comutils::fileUtils::fileBaseName(f, bname);
        std::cout << "file[" << f << "] basename:" << bname << std::endl;
    }
    std::cout << std::endl;

    std::cout << "\n---------fileSize & getContent---------" << std::endl;
    std::string tmpFilePath = "./info.json";
    if (lysutil::comutils::fileUtils::isFileExist(tmpFilePath)) {
        size_t fileSize = lysutil::comutils::fileUtils::fileSize(tmpFilePath);
        std::cout << "fileSize=" << fileSize << std::endl;
        std::string tmpFileBuf;
        lysutil::comutils::fileUtils::getTxtFileContent(tmpFilePath, tmpFileBuf);
        std::cout << "getTxtFileContent=" << tmpFileBuf << std::endl;
    }

    std::cout << std::endl;
    //cout << tmpFileBuf << endl;

    std::cout << "\n---------isAllNumber---------" << std::endl;
    std::cout << lysutil::comutils::strUtils::isAllNumber("444444444") << std::endl;
    std::cout << lysutil::comutils::strUtils::isAllNumber("444444f444") << std::endl;
    std::cout << lysutil::comutils::strUtils::isAllNumber("0") << std::endl;
    std::cout << lysutil::comutils::strUtils::isAllNumber("") << std::endl;


    std::cout << "\n---------rune---------" << std::endl;
    std::string runStr = "擘画强军蓝图，指引奋进征程。2013年到2018年全国两会期间，中共中央总书记、国家主席、中央军委主席习近平连续出席解放军和武警部队代表团全体会议并发表重要讲话，提出一系列新思想、新论断、新要求。6年来，全军部队认真贯彻习主席重要讲话精神，牢固确立习近平强军思想的指导地位，重振政治纲纪，重塑组织形态，重整斗争格局，重构建设布局，重树作风形象，在中国特色强军之路上迈出坚定步伐。";
    std::string runeOutput;
    size_t lineNum;
    lysutil::comutils::runeUtils::runeWrap(runStr, 10, runeOutput, lineNum);
    std::cout << "runeWrap..... lineNum = " << lineNum << std::endl;
    std::cout << runeOutput << std::endl;

    std::cout << "\n---------table---------" << std::endl;
    lysutil::comutils::terminalTable table;
    table.addHeadData(3, "shop_id", "shop_name", "desc");

    lysutil::comutils::pcreUtils rg("\\|");

    size_t tlen = sizeof(testForTable) / sizeof(char *);
    for (i = 0; i < tlen; i++) {
        std::string tmpStr(testForTable[i]);
        std::vector<std::string> subList;
        rg.reg_split(tmpStr, subList);
        table.addRowData(subList);
    }

    //设置各种颜色
    table.setBorderColorType(lysutil::comutils::COLOR_RED);
    table.setRowFontColorType(lysutil::comutils::COLOR_GREEN);
    table.setHeaderFontColorType(lysutil::comutils::COLOR_BROWN);

    std::string output;
    table.renderToString(output);
    std::cout << output << std::endl;
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com/");
        res = curl_easy_perform(curl);
        if (CURLE_OK == res) {
            char *ct;
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            if ((CURLE_OK == res) && ct)
                printf("We received Content-Type: %s\n", ct);
        }
        curl_easy_cleanup(curl);
    }


    testunwind();
    testJsonCPP();
    testSQLite3();
    testLua(argc, argv);
//    struct sockaddr_in addr;
//    struct event_base *base = NULL;
//    struct bufferevent *event = NULL;
//    int sock = -1;
//    //初始化服务端地址信息
//    memset(&addr, 0x00, sizeof(addr));
//    addr.sin_family = AF_INET;
//    inet_aton(SERVER_IP, &addr.sin_addr);
//    addr.sin_port = htons(SERVER_PORT);
//    //1.创建基础事件实例
//    base = event_base_new();
//    if (NULL == base) {
//        printf("Create instance of event_base failed.\n");
//        return -1;
//    }
//    //2.创建基于此基本事件的缓存事件
//    //设置非阻塞socket
//    sock = socket(AF_INET, SOCK_STREAM, 0);
//    if (-1 == sock) {
//        printf("Create socket file descriptor failed.\n");
//        return -1;
//    }
//    evutil_make_socket_nonblocking(sock);
//    event = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
//    //开启读写
//    bufferevent_enable(event, EV_READ | EV_WRITE);
//    //设置回调
//    bufferevent_setcb(event, read_cb, write_cb, event_cb, NULL);
//    //3.连接服务端
//    if (0 != bufferevent_socket_connect(event, (struct sockaddr *) (&addr), sizeof(addr))) {
//        printf("Connect to server failed.\n");
//        return -1;
//    }
//    //4.为基本事件分配循环：开启客户端
//    bufferevent_write(event, call, strlen(call));
//    event_base_dispatch(base);
    return 0;
}
