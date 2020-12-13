#ifndef BUCKET
#define BUCKET

#include <bits/stdc++.h>
using namespace std;

#include "definitions.h"

class Bucket {
        int localDepth;
        struct DataItem record1;
        struct DataItem record2;
    public:
        Bucket(int depth);
        int insert(int key, int data);
        int remove(int key);
        void search(int key);
        pair<DataItem, DataItem> copy();
        int isFull();
        int isEmpty();
        int getDepth();
        int increaseDepth();
        int decreaseDepth();
        void clear();
        void display();
};

Bucket::Bucket(int depth)
{
    this.localDepth = depth;
    this.record1.valid = 0;
    this.record2.valid = 0;
}

int Bucket::insert(int key, int data)
{
    int val = search(key);
    if (val != -1)
        return -1;

    if (this.record1.valid == 0){ //not valid
        this.record1.valid = 1;
        this.record1.key = key;
        this.record1.data = data;
        return 1;
    }
    else if (this.record2.valid == 0){
        this.record2.valid = 1;
        this.record2.key = key;
        this.record2.data = data;
        return 1;
    }
    else {
        return 0;
    }
}

int Bucket::remove(int key)
{
    if (this.record1.key == key && this.record1.valid == 1){
        this.record1.valid = 0;
        return 1;
    }
    else if (this.record2.key == key && this.record2.valid == 1){
        this.record2.valid = 0;
        return 1;
    }
    else
        return 0;
}


int Bucket::search(int key)
{
    if (record1.key == key && record1.valid == 1){
        return record1.data;
    }
    else if (record2.key == key && record2.valid == 1) {
        return record2.data;
    }
    else {
        return -1;
    }
}


pair<DataItem, DataItem> Bucket::copy(){
    return make_pair(record1, record2);
}

int Bucket::isFull()
{
    if (record1.valid == 1 && record2.valid == 1){
        return true;
    }
    return false;
}

int Bucket::isEmpty()
{
    if (record1.valid == 0 && record2.valid == 0){
        return true;
    }
    return false;
}

int Bucket::getDepth()
{
    return localDepth;
}

int Bucket::increaseDepth()
{
    localDepth++;
    return localDepth;
}

int Bucket::decreaseDepth()
{
    localDepth--;
    return localDepth;
}

void Bucket::clear()
{
    this.record1.valid = this.record2.valid = 0;
}

void Bucket::display()
{
    if (record1.valid == 1){
        cout << "Record 1: " << endl;
        cout << "key = "  << this.record1.key << endl;
        cout << "data = "  << this.record1.data << endl;
    }
    if (record2.valid == 1){
        cout << "Record 2: " << endl;
        cout << "key = "  << this.record2.key << endl;
        cout << "data = "  << this.record2.data << endl;
    }
}

#endif