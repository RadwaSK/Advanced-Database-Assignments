
#include "readfileChaining.h"

int hashCode(int key){
   return key % MBUCKETS;
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
	struct DataItem data;   //a variable to read in it the records from the db
	*count = 0;				//No of accessed records
	int hashIndex = hashCode(item->key);  				//calculate the Bucket index
	int startingOffset = hashIndex*sizeof(Bucket);		//calculate the starting address of the bucket
	int Offset = startingOffset;						//Offset variable which we will use to iterate on the db
    DataItem* dataItemPtr = NULL;                       // pointer to store the next data item in chaining

    int i=0;
    while (i < RECORDSPERBUCKET){
        //on the linux terminal use man pread check the function manual
        ssize_t result = pread(fd,&data,sizeof(DataItem), Offset);
        //one record accessed
        (*count)++;
        //check whether it is a valid record or not
        if(result <= 0) { //either an error happened in the pread or it hit an unallocated space
        	// perror("some error occurred in pread");
            return -1;
        }
        else if (data.valid == 1 && data.key == item->key) {
            //I found the needed record
            item->data = data.data ;
            return Offset;
        }
        else {
            Offset +=sizeof(DataItem);  //move the offset to next record
            if (Offset > (startingOffset + (RECORDSPERBUCKET*sizeof(Bucket)))){ // i entered another bucket
                dataItemPtr = NULL; //************** to be changed to the ptr inside the bucket
                break;
            }
        }
        i++;
    } // end while
    while (dataItemPtr != NULL){
        if (dataItemPtr->valid == 1 && dataItemPtr->key == item->key) {
            //I found the needed record
            item->data = data.data ;
            return Offset;  // ************* what to return here ?
        }
        else {
            dataItemPtr = dataItemPtr->ptr; // go to the next record
        }
    }
    return -1;

}