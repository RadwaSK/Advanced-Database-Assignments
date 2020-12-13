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
   //3. Add some data in the table
   insert(1, 20);//1.1
   insert(2, 70);//2.1
   insert(42, 80);//2.2 
   insert(4, 25);//4.1
   insert(12, 44);//2 colision (in 3.1)
   insert(14, 32);//4 colision (in 4.2)
   insert(17, 11);//7.1
   insert(13, 78);//3.2
   insert(37, 97);//7.2
   insert(11, 34);//1.2
   insert(22, 730);//2->5.1
   insert(46, 840);//6.1
   insert(9, 83);//9.1
   insert(21, 424);//1->5.2
   insert(41, 115);//1->6.2
   insert(71, 47);//1->8.1
   insert(31, 92);//1->8.2
   insert(73, 45);//3->9.2
   insert(97, 11);//7->0.1
   

   //4. Display the database file again
   DisplayFile(filehandle);

   //5. Search the database
   search(13);//3.1->2 collision
   search(97);//0.1->7 collision

   //6. delete an item from the database
   deleteItem(31);//8.2 free
   deleteItem(9);//9.1 free
   deleteItem(14);//4.2 free

   //7. Display the final data base
   DisplayFile(filehandle);
   
   /*
  //test case 2
   insert(1, 20);//1.1
   insert(2, 70);//2.1
   insert(42, 80);//2.2
   insert(4, 25);//4.1
   insert(12, 44);//2 collision3.1
   insert(14, 32);//2 collision3.2
   insert(17, 11);//7.1
   insert(13, 78);//4.2
   insert(37, 97);//7.2
   insert(11, 34);//1.2
   insert(22, 730);//5.1
   insert(46, 840);//6.1
   insert(9, 83);//9.1
   insert(21, 424);5.2
   insert(41, 115);//6.2
   insert(71, 47);//8.1
   insert(31, 92);//8.1
   insert(73, 45);//9.1


   //4. Display the database file again
   DisplayFile(filehandle);

   //5. Search the database
   search(13);//4.2

   //6. delete an item from the database
   deleteItem(31);//8.1 free

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
