 #include "readfile.h"

 int hash1(int key){
     return key % MBUCKETS;
 }

 int hash2(int key){
     return PRIME - (key % PRIME);
 }

int insertItem(int fd, DataItem item){
    struct DataItem currentData;
	int count = 0;
	int hashIndex = hash1(item.key);

	int currentOffset = hashIndex * BUCKETSIZE;
	int recordIndexWithinBucket = 1;
	int startingOffset = -1; // considered in case reached openAddressing search
	
	bool rewind = false;
	bool secondHashApplied = false;
	bool startOpenAddressing = false;

	while (true){
		ssize_t result = pread(fd, &currentData, sizeof(DataItem), currentOffset);

		count++;

		if (result <= 0)
			return -1;
		else if (currentData.valid == 0){
			// current Data is empty
			pwrite(fd, &item, sizeof(DataItem), currentOffset);
			return count;
		} else {
			// the place was busy
			// first search other records within the bucket
			if (recordIndexWithinBucket < RECORDSPERBUCKET && !startOpenAddressing){
				currentOffset += sizeof(DataItem);
				recordIndexWithinBucket++;
			}
			else if (!secondHashApplied){
				// if I finished searching within the bucket and didn't find the data
				recordIndexWithinBucket = 1;
				hashIndex = (hashIndex + hash2(item.key)) % MBUCKETS;
				currentOffset = hashIndex * BUCKETSIZE;
				secondHashApplied = true;
			}
			else {
				// if second Hash function is applied and still didn't find it, apply openAddressing
				currentOffset += sizeof(DataItem);
				if (!startOpenAddressing){
					startingOffset = currentOffset;
					startOpenAddressing = true;
				}
			}


			// if reached end
			if (currentOffset >= FILESIZE && !rewind){
				rewind = true;
				currentOffset = 0;
			}
			else if (currentOffset == startingOffset && rewind)
				return -1;
			
		}

	}
	
	return 0;
}

int searchItem(int fd, struct DataItem* item, int *count){
    struct DataItem currentData;
	*count = 0;
	
	int hashIndex = hash1(item->key);
	int currentOffset = hashIndex * BUCKETSIZE;
	int recordIndexWithinBucket = 1;
	int startingOffset = -1; // considered in case reached openAddressing search
	
	bool rewind = false;
	bool finished = false;
	bool secondHashApplied = false;
	bool startOpenAddressing = false;

	while (!finished){
		ssize_t result = pread(fd, &currentData, sizeof(DataItem), currentOffset);

		(*count)++;

		if (result <= 0)
			return -1;
		else if (currentData.valid == 1 && currentData.key == item->key){
			// current Data is valid and is the key I'm searching for
			item->data = currentData.data;
			return currentOffset;
		} else {
			// I didn't find it
			// first search other records within the bucket
			if (recordIndexWithinBucket < RECORDSPERBUCKET && !startOpenAddressing){
				currentOffset += sizeof(DataItem);
				recordIndexWithinBucket++;
			}
			else if (!secondHashApplied){
				// if I finished searching within the bucket and didn't find the data
				recordIndexWithinBucket = 1;
				hashIndex = (hashIndex + hash2(item->key)) % MBUCKETS;
				currentOffset = hashIndex * BUCKETSIZE;
				secondHashApplied = true;
			}
			else {
				// if second Hash function is applied and still didn't find it, apply openAddressing
				currentOffset += sizeof(DataItem);
				if (!startOpenAddressing){
					startingOffset = currentOffset;
					startOpenAddressing = true;
				}
			}


			// if reached end
			if (currentOffset >= FILESIZE && !rewind){
				rewind = true;
				currentOffset = 0;
			}
			else if (currentOffset == startingOffset && rewind)
				finished = true;
			
		}

	}
	
	return -1;
}

int deleteOffset(int fd, int offset){
    struct DataItem dummyItem;
	dummyItem.valid = 0;
	dummyItem.key = -1;
	dummyItem.data = 0;
	int result = pwrite(fd, &dummyItem, sizeof(DataItem), offset);
	return result;
}


int DisplayFile(int fd){
    struct DataItem data;
	int count = 0;

	for(int offset = 0; offset < FILESIZE; offset += sizeof(DataItem)) {
		ssize_t result = pread(fd, &data, sizeof(DataItem), offset);
		if(result < 0) {
            perror("some error occurred in pread");
			return -1;
		} else if (result == 0 || data.valid == 0 ) { //empty space found or end of file
			printf("Bucket: %d, Offset %d:~\n", offset/BUCKETSIZE, offset);
		} else {
			pread(fd,&data,sizeof(DataItem), offset);
			printf("Bucket: %d, Offset: %d, Data: %d, key: %d\n", offset/BUCKETSIZE, offset,data.data,data.key);
					 count++;
		}
	}
	printf("\n=======================================\n");
	return count;
}