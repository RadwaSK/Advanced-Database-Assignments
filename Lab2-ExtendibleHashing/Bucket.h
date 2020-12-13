#ifndef BUCKET
#define BUCKET

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#define MAX_BITS 5

struct DataItem {
	int valid;    //0 means invalid record, 1 = valid record
	int data;
	int key;
	bool operator==(DataItem const &item) {
		if (valid == item.valid && data == item.data && key == item.key)
			return true;
		return false;
	}
};

class Bucket {
        int localDepth;
        struct DataItem record1;
        struct DataItem record2;
    public:
        Bucket(int depth);
        int insert(int key, int data);
        int remove(int key);
        int search(int key);
        pair<DataItem, DataItem> copy();
        int isFull();
        int isEmpty();
        int getDepth();
        int increaseDepth();
        int decreaseDepth();
        void clear();
        void display();
		bool operator ==(Bucket const &b);
};


#endif