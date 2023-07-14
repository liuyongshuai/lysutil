cmake_minimum_required(VERSION 2.8)
project(lysutil)
set(project_name "lysutil")

##============= 确定c++编译器 =============
set(DEFAULT_CPP_COMPILER "/usr/local/gcc/bin/c++")
IF (NOT EXISTS ${DEFAULT_CPP_COMPILER})
    EXEC_PROGRAM("which c++" OUTPUT_VARIABLE DEFAULT_CPP_COMPILER)
ENDIF ()
IF (NOT EXISTS ${DEFAULT_CPP_COMPILER})
    FIND_PROGRAM(DEFAULT_CPP_COMPILER NAMES "c++")
ENDIF ()
set(CMAKE_CXX_COMPILER ${DEFAULT_CPP_COMPILER})
set(CXX ${DEFAULT_CPP_COMPILER})
IF (NOT EXISTS ${DEFAULT_CPP_COMPILER})
    MESSAGE(FATAL_ERROR "cannot find c++/gcc compiler....")
ELSE ()
    MESSAGE(STATUS "find c++ compiler ${DEFAULT_CPP_COMPILER}")
    MESSAGE(STATUS "find gcc compiler ${DEFAULT_C_COMPILER}")
ENDIF ()


##============= 确定编译选项 =============
option(TARGET_DEBUG_MODE "Build the project with debug mode" OFF)
set(CMAKE_CXX_FLAGS "-gdwarf-2 -pipe -std=c++14 -fno-omit-frame-pointer -fPIC  -DSPDLOG_FMT_PRINTF")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(notdir $<)\"'")
if (TARGET_DEBUG_MODE)
    set(GENERATE_TEST "OFF")
    set(CMAKE_BUILD_TYPE "Debug")
    message("debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-W -Wall -O0 -g -ftest-coverage -fprofile-arcs -D__DEBUG_MODE__ -DLOG_DEBUG_ENABLE=true")
else (TARGET_DEBUG_MODE)
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O2 -DLOG_DEBUG_ENABLE=false")
    message("not debug")
endif (TARGET_DEBUG_MODE)
##取消优化选项
IF (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
ENDIF (CMAKE_COMPILER_IS_GNUCC)


# work home
set(work_home ${CMAKE_CURRENT_SOURCE_DIR})
set(src_home ${work_home}/src/)
set(test_home ${work_home}/test/)
message("src_home=${src_home}")


# include file list
set(include_list "")
list(APPEND include_list ${src_home})
list(APPEND include_list ${work_home}/thirdparty/protobuf-3.22.5)
list(APPEND include_list ${work_home}/thirdparty/abseil)
include_directories(${include_list})


# library file list
set(library_list "-L${work_home}/lib")
list(APPEND library_list -lpthread -lpcre)


#test
INCLUDE(${work_home}/thirdparty/abseil/CMakeLists.txt)
INCLUDE(${work_home}/thirdparty/protobuf-3.22.5/CMakeLists.txt)

set(SRC_SOURCE_LIST
        #comutils
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
add_executable(${project_name} ${work_home}/src/main.cpp ${SRC_SOURCE_LIST} ${PROTOBUF_SOURCE_LIST} ${ABSEIL_SOURCE_LIST})
target_link_libraries(${project_name} ${library_list})
install(TARGETS ${project_name} DESTINATION ${work_home}/output/bin)
install(DIRECTORY ${work_home}/conf DESTINATION ${work_home}/output)
install(DIRECTORY ${work_home}/static DESTINATION ${work_home}/output)
#install(FILES ${work_home}/inf/control.sh DESTINATION ${work_home}/output)



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
        ADD_EXECUTABLE(${target} ${originFile} ${SRC_SOURCE_LIST})
        #跟上面的一堆动态、静态库链接起来
        TARGET_LINK_LIBRARIES(${target} ${library_list})
        install(TARGETS ${target} DESTINATION ${work_home}/output/test)
    endforeach ()
endif ()
