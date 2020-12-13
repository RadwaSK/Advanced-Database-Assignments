#pragma once

#include "Directory.h"

Directory::Directory()
{
	this->bucketsCount = 1; // 1 bucket at stage 1
	this->globalDepth = 1; // shoud be 1
	for (int i = 0; i < 1 << globalDepth; i++) // iterate 2^global_depth
	{
		Bucket * bucketPointer = new Bucket(1);
		buckets.push_back(bucketPointer);   // initially all buckets would have depth of 1
	}
}

// returns index of bucket pointer in buckets vector
int Directory::hash(int n)
{
	// return integer(globalDepth-bits from the left)
	// assuming any n is represented in MAX_BITS-bits
	n %= (1 << MAX_BITS);
	int h = n >> (MAX_BITS - globalDepth);
	return h;
}


void Directory::grow()
{
	vector<Bucket*> temp(buckets);
	globalDepth++;
	buckets.resize(1 << globalDepth);

	for (int i = 0, j = 0; i < 1 << globalDepth; i += 2, j++)
		buckets[i] = buckets[i + 1] = temp[j];
}

void Directory::shrink()
{
	vector<Bucket*> temp(buckets);
	globalDepth--;
	buckets.resize(1 << globalDepth);

	for (int i = 0; i < 1 << globalDepth; i++)
		buckets[i] = temp[i * 2];
}

void Directory::split(int bucket_no)
{
	int localDepth, pairIndex, start, end, count, depthDiff;
	
	localDepth = buckets[bucket_no]->increaseDepth(); 
	depthDiff = globalDepth - (localDepth - 1); 

	if (localDepth > globalDepth)
	{
		grow(); // double the directory
		bucket_no = bucket_no << 1; // if it was 10 -> 100
	}

	pair<DataItem, DataItem> temp;
	temp = buckets[bucket_no]->copy();
	buckets[bucket_no]->clear();

	count = (1 << depthDiff); // 2^depthDiff
	for (int i = max(bucket_no - depthDiff, 0); ; i++) {
		if (*buckets[i] == *buckets[bucket_no]) {
			start = i;
			break;
		}
	}

	pairIndex = start;

	// duplicate the bucket at the new index
	buckets[pairIndex] = new Bucket(localDepth);

	for (int i = start; i < start + count / 2; i++)
		buckets[i] = buckets[pairIndex];


	int h1 = hash(temp.first.key);
	buckets[h1]->insert(temp.first.key, temp.first.data);

	int h2 = hash(temp.second.key);
	buckets[h2]->insert(temp.second.key, temp.second.data);

	bucketsCount++;
}

void Directory::merge(int bucket_no)
{
	int depthDiff = globalDepth - buckets[bucket_no]->getDepth();
	int count = (1 << depthDiff);
	delete buckets[bucket_no];

	int start;
	for (int i = max(bucket_no - count, 0); ; i++)
		if (*buckets[i] == *buckets[bucket_no]) {
			start = i;
				break;
		}
	
	int pairIndex;
	if (start == 0)
		pairIndex = count;
	else
		pairIndex = start - 1;

	for (int i = start; i < start + count; i++)
		buckets[i] = buckets[pairIndex];
	
	buckets[bucket_no]->decreaseDepth();
	bucketsCount--;
}

// maps the index of the bucket to its binary
string Directory::getBucketID(int n)
{
	int i;
	string r;
	for (i = 1 << MAX_BITS - 1; i > 0; i = i / 2)
		(n & i) ? r += "1" : r += "0";
	return r;
}

bool Directory::isShrinkable()
{
	for (int i = 1; i < (1 << globalDepth) - 1; i++) {
		if (!(*buckets[i - 1] == *buckets[i]) && !(*buckets[i] == *buckets[i + 1]))
			return false;
	}
	return true;
}

void Directory::insert(int key, int data)
{
	int bucket_no = hash(key);
	int status = buckets[bucket_no]->insert(key, data);

	if (status == 0) // no place to add to bucket
	{
		split(bucket_no);
		insert(key, data);
	}
	else if (status == -1 )// status = -1 i.e. already exists
	{
		cout << "Key " << key << " already exists in bucket " << getBucketID(bucket_no) << endl;
	}
}

void Directory::remove(int key)
{
	int bucket_no = hash(key);
	int data = buckets[bucket_no]->search(key);

	if (data != -1 && buckets[bucket_no]->remove(key))
		cout << "Deleted key " << key << " from bucket " << getBucketID(bucket_no) << endl;
	else if (data == -1) {
		cout << "Data not found";
		return;
	}

	if (buckets[bucket_no]->isEmpty() && buckets[bucket_no]->getDepth() > 1)
		merge(bucket_no);

	if (isShrinkable())
		shrink();
}

void Directory::search(int key)
{
	int bucket_no = hash(key);
	int data = buckets[bucket_no]->search(key);
	if (data == -1) {
		cout << "Data not in Directory" << endl;
	}
	else {
		cout << "Searching key " << key << ", data is " << data << ", in bucket " << getBucketID(bucket_no) << endl;
	}
}

void Directory::display()
{
	cout << "Global depth : " << globalDepth << endl;
	for (int i = 0; i < buckets.size(); i++)
	{
		cout << "Bucket " << getBucketID(i) << " has ";
		buckets[i]->display();
		cout << "====================" << endl;
	}
}