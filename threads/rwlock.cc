#include<stdio.h>
#include <iostream> //


#include "rwlock.h"

//Implement all methods of RWLock defined in rwlock.h
//Your solution for  Task 3 
//TODO


RWLock::RWLock() {
    AR = 0;
    WR = 0;
    AW = 0;
    WW = 0;
    // okToRead = NULL;
    // okToWrite = NULL;
    okToRead = new Condition((char*)"okToRead");
    okToWrite = new Condition((char*)"okToWrite");
    lock = new Lock((char*)"lock");
    //pthread_cond_init(&okToWrite, NULL);
    
 }
RWLock::~RWLock() {
  delete lock;
  delete okToRead;
  delete okToWrite;
  }
void RWLock::startRead() {
    lock->Acquire();
	//pthread_mutex_lock(&lock);
    
    while((AW + WW) > 0){
        WR++;
        //pthread_cond_wait(&okToRead,&lock);
        okToRead->Wait(lock);
        WR--;
    }
    AR++;
    //pthread_mutex_unlock(&lock);
    lock->Release();

 }
void RWLock::doneRead() {
    //pthread_mutex_lock(&lock);
    lock->Acquire();
    AR--;
    if(AR==0 && WW > 0){
        //pthread_cond_signal(&okToWrite);
        okToWrite->Signal(lock);
    }
    //pthread_mutex_unlock(&lock);
    lock->Release();

    }
void RWLock::startWrite() {
   //pthread_mutex_lock(&lock);
   lock->Acquire();
   while((AW + AR) > 0){
       WW++;
       //pthread_cond_wait(&okToWrite,&lock);
       okToWrite->Wait(lock);
       WW--;
   }
   AW++;
   //pthread_mutex_unlock(&lock);
   lock->Release();

 }
void RWLock::doneWrite() {
    //pthread_mutex_lock(&lock);
    lock->Acquire();
    AW--;
    if(WW > 0){
        //pthread_cond_signal(&okToWrite);
        okToWrite->Signal(lock);
    }else if(WR>0){
        //pthread_cond_broadcast(&okToRead);
        okToRead->Broadcast(lock);
    }
    //pthread_mutex_unlock(&lock);
    lock->Release();

    }
