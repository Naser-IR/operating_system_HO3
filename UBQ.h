//
// Created by naser on 6/4/22.
//

#ifndef UNTITLED_UBQ_H
#define UNTITLED_UBQ_H

#include <deque>
#include <iostream>
#include <semaphore.h>
#include <queue>
using namespace std;
class UBQ : public queue<string> {
    queue<string> news;
    sem_t full;
    sem_t mutex;
public:
    UBQ(){
        sem_init(&full, 0, 0);
        sem_init(&mutex, 0, 1);
    };
    void enqueue(string s){
        sem_wait(&mutex);
        news.push(s);
        sem_post(&mutex);
        sem_post(&full);
    };
    string dequeue(){

        sem_wait(&full);
        sem_wait(&mutex);
        string bnews =news.front();
        //std::cout << bnews;
        news.pop();
        sem_post(&mutex);
        return bnews;
    };

};

#endif //UNTITLED_UBQ_H
