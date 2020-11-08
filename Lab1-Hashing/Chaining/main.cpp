//============================================================================
// Name        : hashskeleton.cpp
// Author      : 
// Version     :
// Copyright   : Code adapted From https://www.tutorialspoint.com/
// Description : Hashing using open addressing
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
   insert(1, 20);
   insert(2, 70);
   insert(42, 80);
   insert(4, 25);
   insert(12, 44);
   /*
   insert(14, 32);
   insert(17, 11);
   insert(13, 78);
   insert(37, 97);
   insert(11, 34);
   insert(22, 730);
   insert(46, 840);
   insert(9, 83);
   insert(21, 424);
   insert(41, 115);
   insert(71, 47);
   insert(31, 92);
   insert(73, 45);
   */

   //4. Display the database file again
   DisplayFile(filehandle);
   
   //5. Search the database
   search(42);
   search(12);

/*
   //6. delete an item from the database
   deleteItem(31);

   //7. Display the final data base
   DisplayFile(filehandle);

*/
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
   printf("Delete: No of records searched is %d\n",diff);


   // read the corresponding bucket
   int bucketoffset = Offset/sizeof(BUCKETSIZE); 
   struct Bucket bucket;
   pread(filehandle,&bucket,sizeof(Bucket), bucketoffset);

   struct DataItem data;
   // read the first record from this family that is in the overflow area
   pread(filehandle,&data,sizeof(DataItem), bucket.dataItemptr);

   // case 1 : offset is in main file  
   if(Offset >=0 &&  Offset < MAINFILESIZE){
      //no overflow exist in this bucket
      
      if (bucket.dataItemptr == -1){ // no overflow
         return deleteOffset(filehandle,Offset);
      }
      else {//there is an overflow in this bucket
         // delete this record from main bucket
         int deleteResult = deleteOffset(filehandle,Offset);
         struct DataItem data;
         // insert that record normally .. and it should be inserted into the place of the deleted record
         insertItem(filehandle, data);
         // update the bucket pointer to the pointer inside the read record
         bucket.dataItemptr = data.dataItemptr;
         pwrite(filehandle,&bucket,sizeof(Bucket), bucketoffset);
         // set the valid bit of the overflow record to 0 and rewrite it again
         data.valid = 0;
         pwrite(filehandle,&data,sizeof(DataItem), bucket.dataItemptr);
         return deleteResult;
      }  
   }

   // here "data" has the value of the first record in overflow area for that bucket
   
   // case 2 : offset is the first record in overflow area
   if (Offset == bucket.dataItemptr){
      bucket.dataItemptr = data.dataItemptr;
      pwrite(filehandle,&bucket,sizeof(Bucket), bucketoffset);
      return deleteOffset(filehandle, Offset);
   }

   // case 3: offset is one of the records in overflow area
   int currPtr = -1;
   int prevPtr = -1;
   
   // here "data" has the value of the first record in overflow area for that bucket also 

   prevPtr = bucket.dataItemptr; // the offset of 1st element in the oveflow
   currPtr = data.dataItemptr; // the offset of the 2nd element in overflow
   while (currPtr != -1) {
      ssize_t result = pread(filehandle,&data,sizeof(DataItem), currPtr);
      if (data.key = key){
         struct DataItem prevData;
         pread(filehandle,&prevData,sizeof(DataItem), prevPtr);
         prevData.dataItemptr = data.dataItemptr;
         pwrite(filehandle,&prevData,sizeof(DataItem), prevPtr);

         return deleteOffset(filehandle, currPtr);
      }
      prevPtr = currPtr;
      currPtr = data.dataItemptr;
   }
   return -1;
}
