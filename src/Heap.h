#ifndef HEAP_H_
#define HEAP_H_

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "Order.h"

using namespace std;


class PriorityQueue {

    vector<Order*> heap; //heap is a vector of orders

    /**private helper functions*/
    void HeapIncreaseKey(int i,  Order* key);
    //A helper function for the heapInsert algorithm

    void maxHeapify(int i, int size);
    //helper function for build max heap
    //Precondition: left(i) and right(i) are both valid max heaps

    void Print_ (ostream& out);
    //helper function for regular print

    void printSorted_ (ostream& out);
    //helper function for sorted print

public:
    /**constructor*/
    PriorityQueue() {heap.push_back(NULL);}
    //instantiates new priority queue

    /**access functions*/
    bool empty() const;
    //determines whether the heap is empty
    //returns true or false

    Order* getRoot() const;
    //returns the root of the heap

    int getSize() const;
    //returns size of vector

    int getParentIndex(int child) const;
    //returns the index of parent of any child

    int leftChild(int i) const;
    //returns left child of a node
    //pre: not a leaf node

    int rightChild(int i) const;
    //returns right child of a node
    //pre: not a leaf node

    /**manipulation procedures*/
    void buildMaxHeap();
    //turns vector into a valid MaxHeap

    void heapInsert( Order*);
    // inserts based on shipping status

    void shipProduct();
    //Gets called when order is shipped by admin.

    void heapSort();
    //heap sort algorithm using max heap property

    /**additional functions*/
    void showPriorityQueue(ostream& out);
    //prints the contents of the priority Queue
    //calls the helper function

    void printSorted(ostream &out);
    // prints the contents of the priority queue sorted ascending.
};


void PriorityQueue::HeapIncreaseKey(int i,  Order* key)
{
	int p = getParentIndex(i);

	if(key > heap[i])
		heap[i] = key;

	while (i > 1 && heap[p] < heap[i])
	{
		Order* temp = heap[i];
		heap[i] = heap[p];
		heap[p] = temp;
		i = p;
	}
}

int PriorityQueue::getSize() const
{
    return heap.size();
}

void PriorityQueue::maxHeapify(int i, int size)
{

    int l = 2 * i;  // left child
    int r = 2 * i + 1;  // right child
    int largest = 0;

    if (l <= size && (int)heap[l]->getShippingType() > (int)heap[i]->getShippingType())
    {
        largest = l;
    }
    else
    {
        largest = i;
    }
    if (r < size && (int)heap[r]->getShippingType() > (int)heap[largest]->getShippingType())
    {
        largest = r;
    }
    if (largest != i)
    {
        swap(heap[i], heap[largest]);
        maxHeapify(largest, size);
    }
}

bool PriorityQueue::empty() const
{
	return heap.size() == 1;
}

Order* PriorityQueue::getRoot() const
{
	return heap[1];
}

int PriorityQueue::getParentIndex(int child)const
{
	int p = child / 2;
	if (child == 0)
		return -1; //not found
	else
		return p;
}

int PriorityQueue::leftChild( int i) const
{
	return i * 2;
}

int PriorityQueue::rightChild(int i) const
{
	return (i * 2) + 1;
}

void PriorityQueue::heapInsert(Order* order)
{
    heap.push_back(order);
    buildMaxHeap();
}

void PriorityQueue::buildMaxHeap()
{
    for (int i = floor((heap.size() - 1) / 2); i > 0; i--) // first elem, dont go into loop!
    {
        maxHeapify(i, getSize());
    }
}

void PriorityQueue::shipProduct()
{
	if (empty())
	{
		cout << "Sorry, no order to ship!" << endl;
		return;
	}
	else
	{
		Order*temp = getRoot();
		getRoot()->setToShipped();//updated status of shipping type, updated customer history.
		heap.erase(heap.begin());
		buildMaxHeap();
		cout << temp->getProductName() << " is shipped!" << endl;
	}

}

void PriorityQueue::showPriorityQueue(ostream& out)
{
	if(empty())
	{
		out << "There are no orders to display (empty)!" << endl;
	}
	else
	{
		out << "Contents of Priority Queue: " << endl;
		printSorted(out);
	}
}

void PriorityQueue::heapSort()
{
    int sz = getSize() - 2;
    for(int i = getSize() - 1; i > 2; i--)
    {
        //swap
        swap(heap[i], heap[1]);

        //disconnect, consider 1 less than heap size
        sz--;
        maxHeapify(1, sz);
    }
}

void PriorityQueue::printSorted(ostream& out)
{
    heapSort();
    printSorted_(out);
}

void PriorityQueue::printSorted_ (ostream& out)
{
    int j = 1;
    for (int i = getSize() - 1; i >= 1; i--)
    {
        out <<  "~~~" << "Product " << j << "~~~" << endl;
        out << "Product Name: "<< heap[i]->getProductName() << endl;
        out << "Purchased on: " << heap[i]->getDateOfPurchaseString() << endl;
        out << "Shipping Priority: ";
        switch(heap[i]->getShippingType())
        {
            case STANDARD : out << "STANDARD" << endl;
                break;
            case OVERNIGHT : out << "OVERNIGHT" << endl;
                break;
            case RUSH : out << "RUSH" << endl;
                break;
        }
        if (heap[i]->isShipped())
        {
            out << "Status: "<< "Shipped" << endl;
        }
        else
        {
            out << "Status: "<< "Not Shipped" << endl;
        }
        out << endl;
        j++;
    }
    out << endl;
}

#endif /* HEAP_H_ */
