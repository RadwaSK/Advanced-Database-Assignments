 #include "readfile.h"

 int hash1(int key){
     return key % MBUCKETS;
 }

 int hash2(int key){
     return PRIME - (key % PRIME);
 }

int insertItem(int fd, DataItem item){
    return 0;
}

int searchItem(int fd, struct DataItem* item, int *count){
    return 0;
}

int deleteOffset(int fd, int offset){
    
}