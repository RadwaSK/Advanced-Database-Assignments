//============================================================================
// Name        : hashskeleton.cpp
// Author      : 
// Version     :
// Copyright   : Code adapted From https://www.tutorialspoint.com/
// Description : Hashing using open addressing
//============================================================================

#include "readfile.h"

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
  
  //test case 1
   insert(1, 20);
   insert(2, 70);
   insert(42, 80);
   insert(4, 25);
   insert(12, 44);
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



/*
   // Testcase 2
   //3. Add some data in the table
    insert(1, 20);
    insert(2, 70);
    insert(42, 80);
    insert(4, 25);
    insert(12, 44);
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
    insert(97, 11);
    insert(10, 28);

    */

   // ================== START OF Multiple Hashing Test Case =============== // 
   /*
   insert(1, 20);  // hash1(1) = 1
   insert(2, 70);  // hash1(2) = 2
   insert(42, 80); // hash1(42) = 2, will be put in 2nd record withing Bucket#2
   insert(4, 25);  // hash1(4) = 4
   insert(12, 44);  //hash1(12) = 2, busy, (hash1(12) + hash2(12)) % 10 = (2 + 2) % 10 = 4, will be put in 2nd record within Bucket#2
   insert(14, 32);  // hash1(14) = 4, busy, (hash1(14) + hash2(14)) % 10 = (4 + 7) % 10 = 1, will be put in 2nd record withing Bucket#1
   insert(24, 11); // hash1(24) = 4, busy, (hash1(24) + hash2(24)) % 10 = (4 + 4) % 10 = 8
   insert(10, 78); // hash1(10) = 0
   insert(20, 97); // hash1(10) = 0, will be put in 2nd record withing Bucket#2
   insert(70, 34); // hash1(70) = 0, busy, (hash1(70) + hash2(70)) % 10 = (0 + 7) % 10 = 7
   insert(140, 730); // hash1(140) = 0, busy, (hash1(140) + hash2(140)) % 10 = (0 + 7) % 10 = 7, put in 2nd rec. within Bucket#7
   insert(280, 133); // hash1(280) = 0, busy, (hash1(280) + hash2(280)) % 10 = (0 + 7) % 10 = 7, busy, 
                     // start open Addressing from next record, so put in Bucket#8 rec2
   insert(48, 840); // hash1(48) = 8, busy, (hash1(48) + hash2(48)) % 10 = (8 + 1) % 10 = 9
   insert(8, 83);  // hash1(8) = 8, busy, (hash1(8) + hash2(8)) % 10 = (8 + 6) % 10 = 4, busy,
                     // start open Addressing from next record, so put in Bucket#5 rec1


   //4. Display the database file again
   DisplayFile(filehandle);

   //5. Search the database
   search(1); // should be 1
   search(42); // should be 2
   search(24); // should be 3
   search(12); // should be 4
   search(8); // should be 5
   search(280); // should be 6
   search(1000); // not found

   //6. delete an item from the database
   deleteItem(1); // 1
   deleteItem(70); // 3
   deleteItem( 18); // not found
   */
   // ================== END OF Multiple Hashing Test Case =============== // 


   //4. Display the database file again
   DisplayFile(filehandle);

   //5. Search the database
   search(13);

   //6. delete an item from the database
   deleteItem(31);

   //7. Display the final data base
   DisplayFile(filehandle);
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
     struct DataItem item ;
     item.data = data;
     item.key = key;
     item.valid = 1;
     int result= insertItem(filehandle,item);  //TODO: implement this function in openAddressing.cpp
     printf("Insert: No. of searched records:%d\n",abs(result));
}

/* Functionality: search for a data in the table using the key

   Input:  key
   Output: the return data Item
*/
struct DataItem * search(int key)
{
  struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
     item->key = key;
     int diff = 0;
     int Offset= searchItem(filehandle,item,&diff); //this function is implemented for you in openAddressing.cpp
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
   if(Offset >=0 )
   {
    return deleteOffset(filehandle,Offset);
   }
   return -1;
}
