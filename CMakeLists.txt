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
SET(CMAKE_CXX_FLAGS "-gdwarf-2 -pipe -std=c++0x -fno-omit-frame-pointer -fPIC -D_GLIBCXX_USE_CXX11_ABI=0  -DSPDLOG_FMT_PRINTF")
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
MESSAGE("src_home=${src_home}")

#thirdparty
SET(PROTOBUF_DIR ${work_home}/thirdparty/protobuf-3.21.12)
SET(BOOST_DIR ${work_home}/thirdparty/boost-1.76.0)
SET(WEBSOCKETPP_DIR ${work_home}/thirdparty/websocketpp-0.8.2)
SET(OPENSSL_DIR ${work_home}/thirdparty/openssl-1.1.1v)
SET(SPDLOG_DIR ${work_home}/thirdparty/spdlog)
SET(RAPIDJSON_DIR ${work_home}/thirdparty/rapidjson)

# websocketpp-0.8.2 file list
SET(include_list "")
LIST(APPEND include_list ${src_home})
list(APPEND include_list ${PROTOBUF_DIR}/include/)
LIST(APPEND include_list ${BOOST_DIR}/include)
LIST(APPEND include_list ${WEBSOCKETPP_DIR}/include)
list(APPEND include_list ${OPENSSL_DIR}/include/)
list(APPEND include_list ${RAPIDJSON_DIR}/include/)
list(APPEND include_list ${SPDLOG_DIR}/include/)
INCLUDE_DIRECTORIES(${include_list})


# library file list
SET(library_list "-L${work_home}/lib")
list(APPEND library_list "${PROTOBUF_DIR}/lib/libprotobuf.a")
list(APPEND library_list "${BOOST_DIR}/lib/libboost_iostreams.a")
list(APPEND library_list "${BOOST_DIR}/lib/libboost_regex.a")
list(APPEND library_list "${BOOST_DIR}/lib/libboost_program_options.a")
list(APPEND library_list "${BOOST_DIR}/lib/libboost_system.a")
list(APPEND library_list "${BOOST_DIR}/lib/libboost_thread.a")
list(APPEND library_list "${BOOST_DIR}/lib/libboost_chrono.a")
list(APPEND library_list "${BOOST_DIR}/lib/libboost_locale.a")
list(APPEND library_list "${OPENSSL_DIR}/lib/libssl.a")
list(APPEND library_list "${OPENSSL_DIR}/lib/libcrypto.a")
LIST(APPEND library_list -ldl -lpthread -lpcre)


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

        #thriftconcurrency
        ${src_home}/thriftconcurrency/Monitor.cpp
        ${src_home}/thriftconcurrency/Mutex.cpp
        ${src_home}/thriftconcurrency/Thread.cpp
        ${src_home}/thriftconcurrency/ThreadFactory.cpp
        ${src_home}/thriftconcurrency/ThreadManager.cpp
        ${src_home}/thriftconcurrency/TimerManager.cpp
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
