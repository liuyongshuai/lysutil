CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(lysutil)
SET(project_name "lysutil")

##============= 确定c++编译器 =============
SET(DEFAULT_CPP_COMPILER "/usr/local/gcc/bin/c++")
IF (NOT EXISTS ${DEFAULT_CPP_COMPILER})
    EXEC_PROGRAM("which c++" OUTPUT_VARIABLE DEFAULT_CPP_COMPILER)
ENDIF ()
IF (NOT EXISTS ${DEFAULT_CPP_COMPILER})
    FIND_PROGRAM(DEFAULT_CPP_COMPILER NAMES "c++")
ENDIF ()
SET(CMAKE_CXX_COMPILER ${DEFAULT_CPP_COMPILER})
SET(CXX ${DEFAULT_CPP_COMPILER})
IF (NOT EXISTS ${DEFAULT_CPP_COMPILER})
    MESSAGE(FATAL_ERROR "cannot find c++/gcc compiler....")
ELSE ()
    MESSAGE(STATUS "find c++ compiler ${DEFAULT_CPP_COMPILER}")
    MESSAGE(STATUS "find gcc compiler ${DEFAULT_C_COMPILER}")
ENDIF ()


##============= 确定编译选项 =============
OPTION(TARGET_DEBUG_MODE "Build the project with debug mode" OFF)
SET(CMAKE_CXX_FLAGS "-gdwarf-2 -pipe -std=c++0x -fno-omit-frame-pointer -fPIC -D_GLIBCXX_USE_CXX11_ABI=0 -DSPDLOG_FMT_PRINTF")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(notdir $<)\"'")
if (TARGET_DEBUG_MODE)
    SET(CMAKE_BUILD_TYPE "Debug")
    MESSAGE("debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -W -Wall -O0 -g -ftest-coverage -fprofile-arcs -D__DEBUG_MODE__ -DLOG_DEBUG_ENABLE=true")
else (TARGET_DEBUG_MODE)
    SET(CMAKE_BUILD_TYPE "Release")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG -O2 -DLOG_DEBUG_ENABLE=false")
    MESSAGE("not debug")
endif (TARGET_DEBUG_MODE)


# work home
SET(work_home ${CMAKE_CURRENT_SOURCE_DIR})
SET(src_home ${work_home}/src/)
SET(test_home ${work_home}/test/)

#include list
SET(include_list "")
LIST(APPEND include_list ${src_home})
list(APPEND include_list ${work_home}/thirdparty/protobuf/include/)
LIST(APPEND include_list ${work_home}/thirdparty/boost/include)
LIST(APPEND include_list ${work_home}/thirdparty/websocketpp/include)
list(APPEND include_list ${work_home}/thirdparty/openssl/include/)
list(APPEND include_list ${work_home}/thirdparty/openldap/include/)
list(APPEND include_list ${work_home}/thirdparty/rapidjson/include/)
list(APPEND include_list ${work_home}/thirdparty/spdlog/include/)
list(APPEND include_list ${work_home}/thirdparty/pcre/include/)
list(APPEND include_list ${work_home}/thirdparty/leveldb/include/)
list(APPEND include_list ${work_home}/thirdparty/thrift/include/)
list(APPEND include_list ${work_home}/thirdparty/gflags/include/)
list(APPEND include_list ${work_home}/thirdparty/gtest/include/)
list(APPEND include_list ${work_home}/thirdparty/gmock/include/)
list(APPEND include_list ${work_home}/thirdparty/benchmark/include/)
list(APPEND include_list ${work_home}/thirdparty/gperftools/include/)
list(APPEND include_list ${work_home}/thirdparty/snappy/include/)
list(APPEND include_list ${work_home}/thirdparty/bzip2/include/)
list(APPEND include_list ${work_home}/thirdparty/lz4/include/)
list(APPEND include_list ${work_home}/thirdparty/zlib/include/)
list(APPEND include_list ${work_home}/thirdparty/zstd/include/)
list(APPEND include_list ${work_home}/thirdparty/sasl2/include/)
list(APPEND include_list ${work_home}/thirdparty/curl/include/)
list(APPEND include_list ${work_home}/thirdparty/idn2/include/)
list(APPEND include_list ${work_home}/thirdparty/tasn1/include/)
list(APPEND include_list ${work_home}/thirdparty/iconv/include/)
list(APPEND include_list ${work_home}/thirdparty/readline/include/)
list(APPEND include_list ${work_home}/thirdparty/ncurses/include/)
list(APPEND include_list ${work_home}/thirdparty/libevent/include/)
list(APPEND include_list ${work_home}/thirdparty/tiff/include/)
list(APPEND include_list ${work_home}/thirdparty/jpeg/include/)
list(APPEND include_list ${work_home}/thirdparty/libpng/include/)
list(APPEND include_list ${work_home}/thirdparty/giflib/include/)
list(APPEND include_list ${work_home}/thirdparty/webp/include/)
list(APPEND include_list ${work_home}/thirdparty/ImageMagick/include/)
INCLUDE_DIRECTORIES(${include_list})


# library file list
SET(library_list "-L${work_home}/lib")
list(APPEND library_list "${work_home}/thirdparty/protobuf/lib/libprotobuf.a")
list(APPEND library_list "${work_home}/thirdparty/boost/lib/libboost_iostreams.a")
list(APPEND library_list "${work_home}/thirdparty/boost/lib/libboost_regex.a")
list(APPEND library_list "${work_home}/thirdparty/boost/lib/libboost_program_options.a")
list(APPEND library_list "${work_home}/thirdparty/boost/lib/libboost_system.a")
list(APPEND library_list "${work_home}/thirdparty/boost/lib/libboost_thread.a")
list(APPEND library_list "${work_home}/thirdparty/boost/lib/libboost_chrono.a")
list(APPEND library_list "${work_home}/thirdparty/boost/lib/libboost_locale.a")
list(APPEND library_list "${work_home}/thirdparty/curl/lib/libcurl.a")
list(APPEND library_list "${work_home}/thirdparty/openssl/lib/libssl.a")
list(APPEND library_list "${work_home}/thirdparty/openssl/lib/libcrypto.a")
list(APPEND library_list "${work_home}/thirdparty/pcre/lib/libpcre.a")
list(APPEND library_list "${work_home}/thirdparty/gflags/lib/libgflags.a")
list(APPEND library_list "${work_home}/thirdparty/gmock/lib/libgmock.a")
list(APPEND library_list "${work_home}/thirdparty/gtest/lib/libgtest.a")
list(APPEND library_list "${work_home}/thirdparty/benchmark/lib/libbenchmark.a")
list(APPEND library_list "${work_home}/thirdparty/snappy/lib/libsnappy.a")
list(APPEND library_list "${work_home}/thirdparty/leveldb/lib/libleveldb.a")
list(APPEND library_list "${work_home}/thirdparty/thrift/lib/libthrift.a")
list(APPEND library_list "${work_home}/thirdparty/thrift/lib/libthriftnb.a")
list(APPEND library_list "${work_home}/thirdparty/gperftools/lib/libtcmalloc_and_profiler.a")
list(APPEND library_list "${work_home}/thirdparty/bzip2/lib/libbz2.a")
list(APPEND library_list "${work_home}/thirdparty/lz4/lib/liblz4.a")
list(APPEND library_list "${work_home}/thirdparty/zlib/lib/libz.a")
list(APPEND library_list "${work_home}/thirdparty/zstd/lib/libzstd.a")
list(APPEND library_list "${work_home}/thirdparty/openldap/lib/libldap.a")
list(APPEND library_list "${work_home}/thirdparty/openldap/lib/liblber.a")
list(APPEND library_list "${work_home}/thirdparty/sasl2/lib/libsasl2.a")
list(APPEND library_list "${work_home}/thirdparty/ssh2/lib/libssh2.a")
list(APPEND library_list "${work_home}/thirdparty/idn2/lib/libidn2.a")
list(APPEND library_list "${work_home}/thirdparty/tasn1/lib/libtasn1.a")
list(APPEND library_list "${work_home}/thirdparty/iconv/lib/libiconv.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libform.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libform_g.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libmenu.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libmenu_g.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libncurses++.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libncurses.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libncurses++_g.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libncurses_g.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libpanel.a")
list(APPEND library_list "${work_home}/thirdparty/ncurses/lib/libpanel_g.a")
list(APPEND library_list "${work_home}/thirdparty/readline/lib/libreadline.a")
list(APPEND library_list "${work_home}/thirdparty/readline/lib/libhistory.a")
list(APPEND library_list "${work_home}/thirdparty/libevent/lib/libevent.a")
list(APPEND library_list "${work_home}/thirdparty/libevent/lib/libevent_core.a")
list(APPEND library_list "${work_home}/thirdparty/libevent/lib/libevent_extra.a")
list(APPEND library_list "${work_home}/thirdparty/libevent/lib/libevent_openssl.a")
list(APPEND library_list "${work_home}/thirdparty/libevent/lib/libevent_pthreads.a")
list(APPEND library_list "${work_home}/thirdparty/ImageMagick/lib/libMagick++.a")
list(APPEND library_list "${work_home}/thirdparty/ImageMagick/lib/libMagickWand.a")
list(APPEND library_list "${work_home}/thirdparty/ImageMagick/lib/libMagickCore.a")
list(APPEND library_list "${work_home}/thirdparty/tiff/lib/libtiff.a")
list(APPEND library_list "${work_home}/thirdparty/jpeg/lib/libjpeg.a")
list(APPEND library_list "${work_home}/thirdparty/libpng/lib/libpng.a")
list(APPEND library_list "${work_home}/thirdparty/giflib/lib/libgif.a")
list(APPEND library_list "${work_home}/thirdparty/webp/lib/libwebp.a")
list(APPEND library_list "${work_home}/thirdparty/webp/lib/libsharpyuv.a")
list(APPEND library_list "${work_home}/thirdparty/webp/lib/libwebpdemux.a")
list(APPEND library_list "${work_home}/thirdparty/webp/lib/libwebpmux.a")
LIST(APPEND library_list  -lpthread -ldl)


#引入其他文件
SET(SRC_SOURCE_LIST
        #protocol
        ${src_home}/protocol/httpresp.pb.cc

        #comutils
        ${src_home}/comutils/pb_utils.cpp
        ${src_home}/comutils/color_utils.cpp
        ${src_home}/comutils/file_utils.cpp
        ${src_home}/comutils/http_utils.cpp
        ${src_home}/comutils/pcre_utils.cpp
        ${src_home}/comutils/regexp_utils.cpp
        ${src_home}/comutils/rune_utils.cpp
        ${src_home}/comutils/str_utils.cpp
        ${src_home}/comutils/sys_utils.cpp
        ${src_home}/comutils/md5_utils.cpp
        ${src_home}/comutils/terminal_table.cpp
        ${src_home}/comutils/toml_parser.cpp

        #httpsvr
        ${src_home}/httpsvr/global_conf.cpp
        ${src_home}/httpsvr/http_common.cpp
        ${src_home}/httpsvr/http_request.cpp
        ${src_home}/httpsvr/http_response.cpp
        ${src_home}/httpsvr/http_router.cpp
        ${src_home}/httpsvr/http_server.cpp
        ${src_home}/httpsvr/http_task.cpp
        )


#工程的二进制文件
ADD_EXECUTABLE(${project_name} ${work_home}/src/main.cpp ${SRC_SOURCE_LIST} ${PROTOBUF_SOURCE_LIST} ${ABSEIL_SOURCE_LIST})
TARGET_LINK_LIBRARIES(${project_name} ${library_list})
INSTALL(TARGETS ${project_name} DESTINATION ${work_home}/output/bin)
INSTALL(DIRECTORY ${work_home}/conf DESTINATION ${work_home}/output)
INSTALL(DIRECTORY ${work_home}/static DESTINATION ${work_home}/output)
#INSTALL(FILES ${work_home}/inf/control.sh DESTINATION ${work_home}/output)


#遍历./test下所有的.cpp文件
if (WITH_TEST)
    AUX_SOURCE_DIRECTORY(${test_home} testSourceList)
    foreach (originFile ${testSourceList})
        MESSAGE(STATUS ">>>>> start add test ${originFile}")
        #获取basename
        GET_FILENAME_COMPONENT(b ${originFile} NAME)
        #不带后缀的名称
        STRING(REGEX REPLACE "(.*).cpp$" "\\1" target ${b})
        #每个文件生成一个同名的二进制文件
        ADD_EXECUTABLE(${target} ${originFile} ${SRC_SOURCE_LIST} ${PROTOBUF_SOURCE_LIST} ${ABSEIL_SOURCE_LIST})
        #跟上面的一堆动态、静态库链接起来
        TARGET_LINK_LIBRARIES(${target} ${library_list})
        INSTALL(TARGETS ${target} DESTINATION ${work_home}/output/test)
    endforeach ()
endif ()
