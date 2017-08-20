#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <string>
#include <iomanip>
#include "List.h"
#include "Customer.h"

using namespace std;

template <class hashdata>
class HashTable {
public:
	static const int SIZE = 10;

    int hash(string key) const;
    int countBucket(int index) const;
    hashdata* searchData(hashdata b);
    void insertData(hashdata b);
    void removeData(hashdata b);
    void printBucket(int index) const;
    void printTable() const;
    List<hashdata>& getBucket(int index);
    bool empty() const;

    void customerLogin();
protected:
    List<hashdata> Table[SIZE];
};

template <class hashdata>
int HashTable<hashdata>::hash(string key) const
{
    int index, sum = 0;
    for (int i = 0; i < key.length(); i++)
    {
        sum += (int)key[i];
    }
    index = sum % SIZE;
    return index;
}

template <class hashdata>
void HashTable<hashdata>::insertData(hashdata data)
{
    int index = hash(data.getKey());
    Table[index].insertEnd(data);
}

template <class hashdata>
void HashTable<hashdata>::printBucket(int index) const
{
    assert(0 <= index && index < SIZE);
    cout << setprecision(2) << fixed;
    Table[index].print();
}

template <class hashdata>
void HashTable<hashdata>::printTable() const
{
    cout << setprecision(2) << fixed;
    for (int i = 0; i < SIZE; i++)
    {
        if (!Table[i].empty()) {
            printBucket(i);
        }
    }
}

template <class hashdata>
int HashTable<hashdata>::countBucket(int index) const
{
    return Table[index].getLength();
}

template <class hashdata>
bool HashTable<hashdata>::empty() const  // check if the Table is empty
{
    int length = 0;
    for (int i = 0; i < SIZE; i++)
    {
        length = Table[i].getLength();
        if (length != 0)
            return false;
    }
    return true;
}

template <class hashdata>
hashdata* HashTable<hashdata>::searchData(hashdata key)
{
    int bucket = hash(key.getKey());
    // TODO: Magic number. Define a static public constant, i.e. HashTable::NOT_FOUND.
    int index = -1;
    if (!Table[bucket].empty())
    {
        index = Table[bucket].linearSearch(key);
    }
    if (index == -1)
        return NULL;
	Table[bucket].advanceToIndex(index);
	return Table[bucket].getIterator();
}

template <class hashdata>
void HashTable<hashdata>::removeData(hashdata data)
{
    int index = hash(data.getCustomerID());
    assert(0 <= index && index < SIZE);

    int found = Table[index].linearSearch(data);
    if (found != -1)
    {
        Table[index].advanceToIndex(found);
        Table[index].deleteIterator();
    }
}

template <class hashdata>
List<hashdata>& HashTable<hashdata>::getBucket(int index) {
	return Table[index];
}

#endif /* HASHTABLE_H_ */
