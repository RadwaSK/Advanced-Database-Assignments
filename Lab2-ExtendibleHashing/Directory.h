#ifndef DIRECTORY
#define DIRECTORY

#include "Bucket.h"

class Directory {
        int globalDepth, bucketsCount;
        vector<Bucket*> buckets;
        int hash(int n);
        void grow();
        void shrink();
        void split(int bucket_no);
        void merge(int bucket_no);
        string getBucketID(int n);
		bool isShrinkable();
    public:
        Directory();
        void insert(int key, int data);
        void remove(int key);
        void search(int key);
        void display();
};




#endif