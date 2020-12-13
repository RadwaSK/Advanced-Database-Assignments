#ifndef DIRECTORY
#define DIRECTORY

#include "Bucket.cpp"

class Directory {
        int globalDepth, bucketsCount;
        vector<Bucket*> buckets;
        int hash(int n);
        void grow();
        void shrink();
        void split(int bucket_no);
        void merge(int bucket_no);
        string getBucketID(int n);
    public:
        Directory();
        void insert(int key, int data,bool reinserted);
        void remove(int key);
        void search(int key);
        void display(bool duplicates);
};


Directory::Directory()
{
    this.bucketsCount = 1; // 1 bucket at stage 1
    this.globalDepth = 1; // shoud be 1
    for(int i = 0 ; i < 1<<global_depth ; i++ ) // iterate 2^global_depth
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
    return n >> (MAX_BITS - globalDepth);    
}


void Directory::grow()
{
    vector<Bucket*> temp(buckets);
    globalDepth++;
    buckets.resize(1<<globalDepth);
    
    for(int i = 0, j = 0; i < 1<<globalDepth ; i+=2, j++)
        buckets[i] = buckets[i+1] = temp[j];
}

void Directory::shrink()
{
    vector<Bucket*> temp(buckets);
    globalDepth--;
    buckets.resize(1<<globalDepth);
    
    for(int i = 0; i < 1<<globalDepth ; i++)
        buckets[i] = temp[i*2];
}

void Directory::split(int bucket_no)
{
    int localDepth, pairIndex, indexDiff, dirSize, i;
    
    localDepth = buckets[bucket_no]->increaseDepth();
    if(localDepth > globalDepth)
    {
        grow(); // double the directory
        bucket_no = bucket_no<<1 ; // if it was 10 -> 100
    }
    pairIndex = bucket_no + 1;
    // duplicate the bucket at the new index
    buckets[pairIndex] = new Bucket(localDepth);
    
    pair<DataItem, DataItem> temp;
    temp = buckets[bucket_no]->copy();
    buckets[bucket_no]->clear();

    // h1 or h2 would be either bucket_no or pairIndex
    int h1 = hash(temp.first.key);
    buckets[h1]->insert(temp.first.key, temp.first.data);

    int h2 = hash(temp.second.key);
    buckets[h2]->insert(temp.second.key, temp.second.data);

    bucketsCount++;
}

void Directory::merge(int bucket_no)
{
    delete buckets[bucket_no];
    if (bucket_no % 2 == 0){
        // merge with bucket below
        buckets[bucket_no] = buckets[bucket_no + 1];   
    }
    else{
        // merge with bucket above
        buckets[bucket_no] = buckets[bucket_no - 1];
    }
    buckets[bucket_no]->decreaseDepth();
    bucketsCount--;
}

// maps the index of the bucket to its binary
string Directory::getBucketID(int n)
{
    int d;
    string s;
    d = buckets[n]->getDepth();
    s = "";
    while(n>0 && d>0)
    {
        s = (n%2==0?"0":"1")+s;
        n/=2;
        d--;
    }
    while(d>0)
    {
        s = "0"+s;
        d--;
    }
    return s;
}

void Directory::insert(int key, int data, bool reinserted)
{
    int bucket_no = hash(key);
    int status = buckets[bucket_no]->insert(key, data);

    if(status == 1)
    {
        if(!reinserted)
            cout << "Inserted key " << key << ", data " << data << " in bucket " << getBucketID(bucket_no) << endl;
        else
            cout << "Moved key " << key << " to bucket " << getBucketID(bucket_no) << endl;
    }
    else if(status == 0) // no place to add to bucket
    {
        split(bucket_no);
        insert(key, data, reinserted);
    }
    else // status = -1 i.e. already exists
    {
        cout << "Key " << key << " already exists in bucket " << getBucketID(bucket_no) << endl;
    }
}

void Directory::remove(int key)
{
    int bucket_no = hash(key);
    int data = buckets[bucket_no]->search(key);

    if(data != -1 && buckets[bucket_no]->remove(key))
        cout << "Deleted key " << key << " from bucket " << getBucketID(bucket_no) << endl;
    else if (data == -1){
        cout << "Data not found";
        return;
    }

    if(buckets[bucket_no]->isEmpty() && buckets[bucket_no]->getDepth() > 1)
        merge(bucket_no);

    if (bucketsCount <= (1<<globalDepth)/2)
        shrink();
}

void Directory::search(int key)
{
    int bucket_no = hash(key);
    int data = buckets[bucket_no]->search(key);
    if (data == -1){
        cout << "data not in Directory" << endl;
    }
    else {
        cout << "Searching key " << key << " in bucket "<< getBucketID(bucket_no) << endl;
        cout << "Record data is " << data << endl;
    }
}

void Directory::display()
{
    cout<<"Global depth : "<<globalDepth<<endl;
    for(int i = 0; i < buckets.size(); i++)
    {
        cout << "Bucket " << getBucketID(i) << " has ";
        buckets[i]->display();
    }
}

#endif