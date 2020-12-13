#ifndef DEFINE_FILE
#define DEFINE_FILE

#DEFINE BUCKET_SIZE  2
#DEFINE MAX_BITS 7

struct DataItem {
   int valid;    //0 means invalid record, 1 = valid record
   int data;     
   int key;
};

#endif