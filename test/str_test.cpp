/*************************************************************************
 * File:	str_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 13:16
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
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
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <MagickWand/MagickWand.h>

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
//    int ret =  iconv (conv, &in, (size_t *) & srclen,
//                                &out,
//                                (size_t *)& deslen);
//
//    if(ret == 0)
//    {
//        printf ("iconv succ\n");
//    }
//    else
//    {
//        if(errno == 84)
//        {
//            printf("iconv  84:%d,%d\n", srclen, deslen);
//        }
//        else
//        {
//            printf("iconv  err %d:%d,%d\n", errno, srclen, deslen);
//        }
//    }
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

//读取回调
void read_cb(struct bufferevent *bev, void *ctx) {
    //读取数据
    char buf[BUF_SIZE] = {0x00};
    size_t len = 0;
    printf("Receive data:\n");
    len = bufferevent_read(bev, buf, BUF_SIZE);
    while (0 < len) {
        printf("%s", buf);
        memset(buf, 0x00, 1024);
        len = bufferevent_read(bev, buf, BUF_SIZE);
    }
    printf("\n");
}

//写回调
void write_cb(struct bufferevent *bev, void *ctx) {
    if (cnt--) {
        printf("Send data to server.\n");
        if (-1 == bufferevent_write(bev, call, strlen(call))) {
            printf("Send data to server failed.\n");
        }
        sleep(5);
    } else {
        printf("send stop");
        bufferevent_write(bev, "stop", strlen("stop"));
    }
}

//事件回调
void event_cb(struct bufferevent *bev, short events, void *ctx) {
    if (events & BEV_EVENT_ERROR) {
        perror("Error from bufferevent.");
    }
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
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


    struct sockaddr_in addr;
    struct event_base *base = NULL;
    struct bufferevent *event = NULL;
    int sock = -1;
    //初始化服务端地址信息
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton(SERVER_IP, &addr.sin_addr);
    addr.sin_port = htons(SERVER_PORT);
    //1.创建基础事件实例
    base = event_base_new();
    if (NULL == base) {
        printf("Create instance of event_base failed.\n");
        return -1;
    }
    //2.创建基于此基本事件的缓存事件
    //设置非阻塞socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock) {
        printf("Create socket file descriptor failed.\n");
        return -1;
    }
    evutil_make_socket_nonblocking(sock);
    event = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
    //开启读写
    bufferevent_enable(event, EV_READ | EV_WRITE);
    //设置回调
    bufferevent_setcb(event, read_cb, write_cb, event_cb, NULL);
    //3.连接服务端
    if (0 != bufferevent_socket_connect(event, (struct sockaddr *) (&addr), sizeof(addr))) {
        printf("Connect to server failed.\n");
        return -1;
    }
    //4.为基本事件分配循环：开启客户端
    bufferevent_write(event, call, strlen(call));
    event_base_dispatch(base);

    const char *inputimg = "./1.png";
    const char *outputimg = "./1.png";
    if (argc < 3) {
        return -1;
    }

    MagickWand *magick_wand = nullptr;
    MagickBooleanType status;

    //初始化MagickWand
    MagickWandGenesis();

    //创建一个MagickWand实例
    magick_wand = NewMagickWand();

    //读取输入图片
    status = MagickReadImage(magick_wand, inputimg);
    if (status == MagickFalse) {
        std::cout << "open image failed" << std::endl;
        magick_wand = DestroyMagickWand(magick_wand);
        MagickWandTerminus();
        return -1;
    }

    //保存图片为
    status = MagickWriteImages(magick_wand, outputimg, MagickTrue);
    if (status == MagickFalse) {
        std::cout << "write image failed" << std::endl;

        magick_wand = DestroyMagickWand(magick_wand);

        MagickWandTerminus();

        return -1;
    }

    //销毁MagickWand实例
    magick_wand = DestroyMagickWand(magick_wand);

    //结束MagickWand
    MagickWandTerminus();

    return 0;
}
