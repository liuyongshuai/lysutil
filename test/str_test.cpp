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
#include "comutils/str_utils.h"
#include "comutils/file_utils.h"
#include "comutils/sys_utils.h"
#include "comutils/color_utils.h"
#include "comutils/http_utils.h"
#include "comutils/rune_utils.h"
#include "comutils/pcre_utils.h"
#include "comutils/terminal_table.h"

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

int main(int argc, char *argv[]) {
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
}