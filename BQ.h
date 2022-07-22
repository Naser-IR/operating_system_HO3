//
// Created by naser on 6/4/22.
//

#ifndef UNTITLED_BQ_H
#define UNTITLED_BQ_H


#include <deque>
#include <iostream>
#include <semaphore.h>
#include <queue>
using namespace std;
class BQ : public queue<string>, public error_code {
    queue<string> news;
    sem_t full;
    sem_t empty;
    sem_t mutex;
public:
    BQ(){
    };
    BQ(int i){
        sem_init(&full, 0, 0);
        sem_init(&empty, 0, i);
        sem_init(&mutex, 0, 1);
        //full = sem_open("try1", O_CREAT, 0644, 0);
       // empty = sem_open("trry1", O_CREAT, 0644, i);
    };
    void enqueue(string s){
        sem_wait(&empty);
        ////
        sem_wait(&mutex);
        news.push(s);
        sem_post(&mutex);
       // cout << s + " pushed to q\n";
        sem_post(&full);
    };
    string dequeue(){
        sem_wait(&full);
        ////
        sem_wait(&mutex);
        string bnews =news.front();
        //news.f
        news.pop();
        sem_post(&mutex);
        sem_post(&empty);
        return bnews;
    };

};


#endif //UNTITLED_BQ_H
