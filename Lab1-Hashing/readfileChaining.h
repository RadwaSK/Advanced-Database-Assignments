/*
 * readfileCahining.h
 *
 *  Created on: nov. 7, 2020
 *      Author: Menna
 */

#ifndef READFILECHAINING_H_
#define READFILECHAINING_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define MBUCKETS  10					//Number of BUCKETS
#define TOTALBUCKETS 20                 //total number of buckets including the overflow area
#define RECORDSPERBUCKET 2				//No. of records inside each Bucket
#define BUCKETSIZE sizeof(Bucket)		//Size of the bucket (in bytes)
#define FILESIZE BUCKETSIZE*MBUCKETS    //Size of the file 


//Data Record inside the file
struct DataItem {
   int valid;    //) means invalid record, 1 = valid record
   int data;     
   int key;
   DataItem* ptr;
};


//Each bucket contains number of records and a pointer to overflow records
struct Bucket {
	struct DataItem  dataItem[RECORDSPERBUCKET];
    DataItem* ptr;
};

//Check the create File
int createFile(int size, char *);

//check the chaining File
int deleteItem(int key);
int insertItem(int fd,DataItem item);
int searchItem(int filehandle,struct DataItem* item,int *count);




#endif /* READFILECHAINING_H_ */
