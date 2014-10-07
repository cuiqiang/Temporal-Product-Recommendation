//
//  SortHeap.h
//  SortHeap
//  This class is used for ranking topk items from a large item set
//  If the type of the item is struct or class, the operator ">" must be override
//  Created by Eagle on 13-7-11.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#ifndef SortHeap_SortHeap_h
#define SortHeap_SortHeap_h

#include <stdlib.h>
#include <iostream>
#include "SortHeap.h"
#include <algorithm>
#include<functional>

using namespace std;

template<class T>
class SortHeap {
private:
    int CurrentSize, MaxSize;
    T *heap;
public:
    SortHeap(int sortHeapSize = 5);
    ~SortHeap();
    int Size() const{return CurrentSize;}//return current size of heap
    T Min(){return heap[1];}//return the minimum point in the heap
    void Insert(const T& x);//insert a point into the sortheap
    void ChangeMin(T x);//delete the minimum point in the heap
    //initialize the sortheap with a exist sortheap(with current size and max size)
    //void Initialize(T a[], int size, int ArraySize);
    void Deactivate();//clear the heap
    T* Output() const;//print the heap
    //bool compare(T a,T b){return a<b;}
};

template <class T>
SortHeap<T>::SortHeap(int sortHeapSize) {
    MaxSize = sortHeapSize;
    heap = new T[MaxSize+1];
    //heap[0] = 0;
    CurrentSize = 0;
}

template <class T>
SortHeap<T>::~SortHeap() {
    if (heap) {
        delete [] heap;
    }
}


template <class T>
void SortHeap<T>::Insert(const T& x){
    //the sortheap is under maxsize
    if (CurrentSize < MaxSize){
        //find a place to insert x
        int i = ++CurrentSize;
        while (i != 1 && x < heap[i/2]){
            heap[i] = heap[i/2];//move x down
            i /= 2;//move to the father
        }
        heap[i] = x;
    }
    //the sortheap is full
    else{
        if (x <= heap[1])//if x is not bigger than the smallest element
            return;
        else{//if x is bigger than the smallest elment
            if (x<heap[2] && x<heap[3])//but is smaller than heap[2] and heap[3]
                heap[1] = x;
            else{
                ChangeMin(x);//Change the min element and the struct of sortheap
            }
        }
    }
}

template <class T>
void SortHeap<T>::ChangeMin(T x){
    //from the root,find a place for x
    int i = 1;//the root
    int ci =2;//the son of i
    
    while (ci <= CurrentSize) {
        //make heap[ci] be the smaller son of i
        if (ci < CurrentSize && heap[ci] > heap[ci+1])
            ci++;
        
        //Can put x into heap[i]?
        if (x <= heap[ci])
            break;//Can
        
        //Can't
        heap[i] = heap[ci];//son up
        i = ci;//i down
        ci *= 2;//ci down
    }
    
    heap[i] = x;
    
}

template <class T>
void SortHeap<T>::Deactivate(){
    heap = 0;
}

template <class T>
T* SortHeap<T>::Output() const{
    sort(heap+1, heap+MaxSize+1);
    reverse(heap+1, heap+MaxSize+1);
    return heap;
}

#endif
