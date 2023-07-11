/*************************************************************************
 * File:	thread_basic_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-11 13:31
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<iostream>
#include <memory>
#include "thriftconcurrency/ThreadManager.h"
#include "thriftconcurrency/Exception.h"
#include "thriftconcurrency/Monitor.h"

class printCounter : public apache::thrift::concurrency::Runnable{
public:
    printCounter(int cnt) : cnt_(cnt){}

    ~printCounter() override{
        std::cout << "[DESTORY]cnt=" << this->cnt_ << std::endl;
    }

    void run() override{
        for (int i = 0; i < 20; i++){
            sleep(1);
            std::cout << "[RUN]cnt=" << this->cnt_ << "\ti=" << i << std::endl;
        }
    }

private:
    int cnt_;
};

int main(int argc, char *argv[]){
    std::shared_ptr< apache::thrift::concurrency::ThreadManager > threadManager = apache::thrift::concurrency::ThreadManager::newSimpleThreadManager(30, 4);
    std::shared_ptr< apache::thrift::concurrency::ThreadFactory > threadFactory = std::make_shared< apache::thrift::concurrency::ThreadFactory >();
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    for (int i = 1; i < 10; i++){
        std::cout << threadManager->state() << std::endl;
        std::shared_ptr< apache::thrift::concurrency::Runnable > task = std::shared_ptr< apache::thrift::concurrency::Runnable >(new printCounter(i));
        threadManager->add(task);
    }
    sleep(30);
    threadManager->stop();
    return 0;
}