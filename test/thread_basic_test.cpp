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
#include "thriftconcurrency/threadManager.h"

class printCounter : public apache::thrift::concurrency::Runnable{
public:
    printCounter(int cnt) : cnt_(cnt){}

    ~printCounter(){
        std::cout << "[DESTORY]threadId=" << this->thread()->getId() << "\tcnt=" << this->cnt_ << std::endl;
    }

    void run(){
        sleep(1);
        std::cout << "[RUN]threadId=" << this->thread()->getId() << "\tcnt=" << this->cnt_ << std::endl;
    }

private:
    int cnt_;
};

int main(int argc, char *argv[]){
    std::shared_ptr< apache::thrift::concurrency::ThreadManager > threadManager = apache::thrift::concurrency::ThreadManager::newSimpleThreadManager(30, 4);
    threadManager->start();
    for (int i = 1; i < 10; i++){
        std::shared_ptr< apache::thrift::concurrency::Runnable > task = std::shared_ptr< apache::thrift::concurrency::Runnable >(new printCounter(i));
        threadManager->add(task);
    }
    return 0;
}