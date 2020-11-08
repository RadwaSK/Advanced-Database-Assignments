#include "readfileChaining.h"

int hashCode(int key){
   //printf("hash index = %d\n", key % MBUCKETS);
   return key % MBUCKETS;
   
}


int insertItem(int fd,DataItem item){

    int hashIndex = hashCode(item.key);  				//calculate the Bucket index
    int startingOffset = hashIndex*sizeof(Bucket);		//calculate the starting address of the bucket
    int Offset = startingOffset;
    struct Bucket bucket;
    int count = 0;
    ssize_t result = pread(fd,&bucket,sizeof(Bucket), Offset);
    
    // insertion in main buckets
   
    int i=0;
    while (i < RECORDSPERBUCKET){ // iterate on records inside each bucket
        count ++;
        if (bucket.dataItem[i].valid != 1 ){
            bucket.dataItem[i].data = item.data;
            bucket.dataItem[i].key = item.key;
            bucket.dataItem[i].valid = item.valid;
            bucket.dataItem[i].dataItemptr = item.dataItemptr;
            int result = pwrite(fd,&bucket,sizeof(Bucket), Offset);
            printf("successfuly inserted in main buckets\n");
            printf("Bucket: %d, Offset %d:~\n",Offset/BUCKETSIZE, Offset+ i*sizeof(DataItem));
            
            //return Offset+ i*sizeof(DataItem);
            return count;
        } 
        
        i++; 
    }
    // no place inside the main buckets
    // search in the overflow area
    // printf("no space in main file \n");

    int startingOffsetOverflow = MBUCKETS*sizeof(Bucket);
    Offset = startingOffsetOverflow;
    while(Offset < FILESIZE){
        count ++;
        struct DataItem data;
        ssize_t result = pread(fd,&data,sizeof(DataItem), Offset);
        if(result < 0) { 	  
            perror("some error occurred in pread");
            return -1;
        }
        else if (data.valid != 1) { // no record inside this offset
            data.data = item.data;
            data.key = item.key;
            data.valid = item.valid;
            data.dataItemptr = bucket.dataItemptr;
            int result = pwrite(fd,&data,sizeof(DataItem), Offset);
            printf("successfuly inserted in overflow area at offest = %d \n", Offset);
            bucket.dataItemptr = Offset;
            pwrite(fd,&bucket,sizeof(Bucket), startingOffset);
            //return Offset;
            return count;
        }
        Offset += sizeof(DataItem);
    }
    

   return -1;
}

/* Functionality: using a key, it searches for the data item
 *          1. use the hash function to determine which bucket to search into
 *          2. search for the element starting from this bucket and till it find it.
 *          3. if the bucket does not have the item, check the overflow area
 *          4. return the number of records accessed (searched)
 *
 * Input:  fd: filehandler which contains the db
 *         item: the dataitem which contains the key you will search for
 *               the dataitem is modified with the data when found
 *         count: No. of record searched
 *
 * Output: the in the file where we found the item
 */

int searchItem(int fd,struct DataItem* item,int *count) {

    //Definitions
	struct Bucket bucket;   //a variable to read in it the records from the db
	*count = 0;				//No of accessed records
	int hashIndex = hashCode(item->key);  				//calculate the Bucket index
	int startingOffset = hashIndex*sizeof(Bucket);		//calculate the starting address of the bucket
	int Offset = startingOffset;						//Offset variable which we will use to iterate on the db
    
    int dataItemPointer = -1;                       //pointer to store the next data item in chaining
    ssize_t result = pread(fd,&bucket,sizeof(Bucket), Offset); //bucket variable has the data of the intented bucket
            
    //check whether it is a valid record or not
    if(result <= 0) { //either an error happened in the pread or it hit an unallocated space
        // perror("some error occurred in pread");
        return -1;
    }

    int i=0;
    while (i < RECORDSPERBUCKET){

        struct DataItem data = bucket.dataItem[i];
        //one record accessed
        (*count)++;
        
        if (data.valid == 1 && data.key == item->key) {
            //I found the needed record
            item->data = data.data ;
            return Offset;
        }
        else {
            Offset +=sizeof(DataItem);  //move the offset to next record
            //printf(" -------- offset = %d \n", Offset);
            if (Offset >= (startingOffset + (RECORDSPERBUCKET*sizeof(DataItem)))){ // i entered another bucket
                dataItemPointer = bucket.dataItemptr; //point to the next record in overflow area
                break;
            }
        }
        i++;
    } // end while
    // start searching in overflow area
    //printf("start searching in oveflow area\n");
    
    if (dataItemPointer == 0 || dataItemPointer == -1){
        return -1;
    }
    
    while (dataItemPointer != -1){
        (*count)++;
        struct DataItem data;
        int Offset = dataItemPointer;
        ssize_t result = pread(fd,&data,sizeof(DataItem), Offset); 
        
        if (data.valid == 1 && data.key == item->key) {
            //I found the needed record
            item->data = data.data ;
            return Offset;  
        }
        else {
            dataItemPointer = data.dataItemptr; // go to the next record
        }
    }
    return -1;

}

/* Functionality: Display all the file contents
 *
 * Input:  fd: filehandler which contains the db
 *
 * Output: no. of non-empty records
 */
int DisplayFile(int fd){
    struct Bucket bucket;
	int count = 0;
	int Offset = 0;
	for(Offset =0; Offset< MAINFILESIZE;Offset += sizeof(Bucket)) { //iterate on main buckets
        
        ssize_t result = pread(fd,&bucket,sizeof(Bucket), Offset);
        if(result < 0) { 	  
            perror("some error occurred in pread");
			return -1;
		}
        int i=0;
        while (i < RECORDSPERBUCKET){ // iterate on records inside each bucket
            if (bucket.dataItem[i].valid == 0 ){
                printf("Bucket: %d, Offset: %d ~\n",Offset/BUCKETSIZE, Offset+ i*sizeof(DataItem));
            } 
            else{   
			    printf("Bucket: %d, Offset: %d, Data: %d, key: %d\n",Offset/BUCKETSIZE,Offset+ i*sizeof(DataItem),bucket.dataItem[i].data, bucket.dataItem[i].key);
                count++;
            }
            i++; 
        }
	} // here the offset variable points to the first place in the overflow area

    while (Offset < FILESIZE) {
        struct DataItem data;
        ssize_t result = pread(fd,&data,sizeof(DataItem), Offset);
        if(result < 0) { 	  
            perror("some error occurred in pread");
			return -1;
		}
        else if (data.valid != 1 ){
            printf("Offset %d:~\n", Offset);
        } 
        else{   
            printf("Offset: %d, Data: %d, key: %d\n",Offset,data.data, data.key);
            count++;
        }
        Offset += sizeof(DataItem);

    }
	return count;
}
 

int deleteOffset(int fd, int Offset)
{
	struct DataItem dummyItem;
	dummyItem.valid = 0;
	dummyItem.key = -1;
	dummyItem.data = 0;
	int result = pwrite(fd,&dummyItem,sizeof(DataItem), Offset);
	return result;
}