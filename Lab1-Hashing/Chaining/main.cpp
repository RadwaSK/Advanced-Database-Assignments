//============================================================================
// Name        : hashskeleton.cpp
// Author      : 
// Version     :
// Copyright   : Code adapted From https://www.tutorialspoint.com/
// Description : Hashing using chaining
//============================================================================

#include "readfileChaining.h"

void insert(int key,int data);
int deleteItem(int key);
struct DataItem * search(int key);


int filehandle;   //handler for the database file

/* DBMS (DataBase Management System) needs to store its data in something non-volatile
 * so it stores its data into files (manteqy :)).

 * Some DBMS or even file-systems constraints the size of that file. 

 * for the efficiency of storing and retrieval, DBMS uses hashing 
 * as hashing make it very easy to store and retrieve, it introduces 
 * another type of problem which is handling conflicts when two items
 * have the same hash index

 * In this exercise we will store our database into a file and experience
 * how to retrieve, store, update and delete the data into this file 

 * This file has a constant capacity and uses external-hashing to store records,
 * however, it suffers from hashing conflicts.
 * 
 * You are required to help us resolve the hashing conflicts 

 * For simplification, consider the database has only one table 
 * which has two columns key and data (both are int)

 * Functions in this file are just wrapper functions, the actual functions are in openAddressing.cpp

*/

int main(){

//here we create a sample test to read and write to our database file

  //1. Create Database file or Open it if it already exists, check readfile.cpp
   filehandle = createFile(FILESIZE,"openaddressing");
   
  //2. Display the database file, check openAddressing.cpp
   DisplayFile(filehandle);

  
  //3. Add some data in the table
   insert(1, 20); // bucket 1.1
   insert(2, 70); // bucket 2.1
   insert(42, 80); // bucket 2.2
   insert(4, 25); // bucket 4.1
   insert(12, 44); // overflow offset = 360
   insert(14, 32); // bucket 4.2
   insert(17, 11); // bucket 7.1
   insert(13, 78); // bucket 3.1
   insert(37, 97); // bucket 7.2
   insert(11, 34); // bucket 1.2
   insert(22, 730); // overflow offset = 376
   insert(46, 840); // bucket 6.1
   insert(9, 83); // bucket 9.1
   insert(21, 424); // overflow offset = 392
   insert(41, 115); // overflow offset = 408
   insert(71, 47); // overflow offset = 424
   insert(31, 92); // overflow offset = 440
   insert(73, 45); // bucket 3.2
   

   //4. Display the database file again
   DisplayFile(filehandle);
   
   //5. Search the database
   printf("searching for element whose key = 42 (in main file)-- offset should be 88");
   search(42);
   printf("searching for element whose key = 12 (in overflow area)-- offset should be 360");
   search(12);
   printf("searching for element whose key = 71 (in overflow area)-- offset should be 424");
   search(71);
   printf("searching for element whose key = 70 (does not exist in DB)-- offset should be -1");
   search(70);


   //6. delete an item from the database
   deleteItem(31); // delete first item in overfow area for bucket 1 @ offset = 440
   DisplayFile(filehandle);
  
   deleteItem(21); // delete last item in overfow area for bucket 1 @ offset = 392
   DisplayFile(filehandle);
   
   deleteItem(9); // delete item inside bucket 9 having no overflow and offset = 324
   DisplayFile(filehandle);

   deleteItem(2); // delete the first element in bucket 2 // there is overflow for this bucket
                  // item in offset 376 should be transfered to this place
   DisplayFile(filehandle);
   struct DataItem data;
   pread(filehandle,&data,sizeof(DataItem), 440);
   printf("%d\n", data.valid);


   // And Finally don't forget to close the file.
   close(filehandle);
   return 0;



}

/* functionality: insert the (key,data) pair into the database table
                  and print the number of comparisons it needed to find
    Input: key, data
    Output: print statement with the no. of comparisons
*/
void insert(int key,int data){
     struct DataItem item;
     item.data = data;
     item.key = key;
     item.valid = 1;
     item.dataItemptr = -1;
     int result= insertItem(filehandle,item);  
     printf("Insert: No. of searched records:%d\n",abs(result));
}

/* Functionality: search for a data in the table using the key
cd
   Input:  key
   Output: the return data Item
*/
struct DataItem * search(int key)
{
   struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
   item->key = key;
   int diff = 0;
   int Offset= searchItem(filehandle,item,&diff); 
   printf("Search: No of records searched is %d\n",diff);
   if(Offset <0)  //If offset is negative then the key doesn't exists in the table
      printf("Item not found\n");
   else
      printf("Item found at Offset: %d,  Data: %d and key: %d\n",Offset,item->data,item->key);
  return item;
}

/* Functionality: delete a record with a certain key

   Input:  key
   Output: return 1 on success and -1 on failure
*/
int deleteItem(int key){
   struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
   item->key = key;
   int diff = 0;
   int Offset= searchItem(filehandle,item,&diff);
   printf("item offset = %d\n", Offset);
   printf("Delete: No of records searched is %d\n",diff);


   // read the corresponding bucket
   int bucketOffset = (key % MBUCKETS)*sizeof(Bucket);
   printf("Bucket offset = %d\n", bucketOffset); 
   struct Bucket bucket;
   pread(filehandle,&bucket,sizeof(Bucket), bucketOffset);

   struct DataItem data;
   // read the first record from this family that is in the overflow area
   pread(filehandle,&data,sizeof(DataItem), bucket.dataItemptr);

   // case 1 : offset is in main file  
   if(Offset >=0 &&  Offset < MAINFILESIZE){
      printf("*****item to be deleted is in main file*****\n");
      //no overflow exist in this bucket
      int deleteResult = deleteOffset(filehandle,Offset);

      if (bucket.dataItemptr == -1 || bucket.dataItemptr == 0){ // no overflow
         printf("*****no overflow*****\n");
         return deleteResult;
      }
      else {
         printf("*****there is overflow*****\n");
         struct DataItem tempData;
         pread(filehandle,&tempData,sizeof(DataItem), bucket.dataItemptr);

         deleteOffset(filehandle,bucket.dataItemptr);

         pread(filehandle,&bucket,sizeof(Bucket), bucketOffset);
         bucket.dataItemptr = tempData.dataItemptr;
         pwrite(filehandle,&bucket,sizeof(Bucket), bucketOffset);

         insertItem(filehandle, tempData);
         return deleteResult;
      }  
   }

   // here "data" has the value of the first record in overflow area for that bucket
   
   // case 2 : offset is the first record in overflow area
   if (Offset == bucket.dataItemptr){
      printf("*****item to be deleted is the first element in overflow area*****");
      bucket.dataItemptr = data.dataItemptr;
      pwrite(filehandle,&bucket,sizeof(Bucket), bucketOffset);
      return deleteOffset(filehandle, Offset);
   }

   // case 3: offset is one of the records in overflow area
   int currPtr = -1;
   int prevPtr = -1;
   printf("*****item to be deleted is in overflow area*****");
   // here "data" has the value of the first record in overflow area for that bucket also 

   prevPtr = bucket.dataItemptr; // the offset of 1st element in the oveflow
   currPtr = data.dataItemptr; // the offset of the 2nd element in overflow
   while (currPtr != -1) {
      ssize_t result = pread(filehandle,&data,sizeof(DataItem), currPtr);
      if (data.key == key){
         struct DataItem prevData;
         pread(filehandle,&prevData,sizeof(DataItem), prevPtr);
         prevData.dataItemptr = data.dataItemptr;
         pwrite(filehandle,&prevData,sizeof(DataItem), prevPtr);
         printf("currPtr = %d\n", currPtr);
         return deleteOffset(filehandle, currPtr);
      }
      prevPtr = currPtr;
      currPtr = data.dataItemptr;
   }
   return -1;
}
