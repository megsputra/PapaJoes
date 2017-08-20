#ifndef LIST_H_
#define LIST_H_

#include <iostream>
#include <string>
#include <cstddef> //for NULL
#include <assert.h>
using namespace std;

template <class listdata> //list stores generic listdata, not any specific C++ type
class List {
private:
    struct Node {
        listdata data;
        Node* nextnode;
        Node* previousnode;

        Node(listdata data) : data(data), nextnode(NULL), previousnode(NULL) {}
    };

    typedef struct Node* NodePtr;

    NodePtr begin;
    NodePtr end;
    NodePtr iterator;
    int length;

    void reverse(NodePtr node) const;
    //Helper function for the public printReverse() function.
    //Recursively prints the data in a List in reverse.

    bool isSorted(NodePtr node) const;
    //Helper function for the public isSorted() function.
    //Recursively determines whether a list is sorted in ascending order.

    int binarySearch(int low, int high, listdata data);
    //Recursively search the list by dividing the search space in half
    //Returns the index of the element, if it is found in the List
    //Returns -1 if the element is not in the List

public:

    /**Constructors and Destructors*/

    List();
    //Default constructor; initializes and empty list
    //Postcondition: initialize length to 0, initialize begin, end and iterator to NULL

    List(const List&);
    //Copy constructor: makes a copy of the list object
    //Postcondition: copy length, begin, end and iterator from the original list

    ~List();
    //Destructor. Frees memory allocated to the list
    //Postcondition: list much be empty

    /**Accessors*/

    listdata getBegin() const;
    //Returns the first element in the list
    //Precondition: length != 0

    listdata getEnd() const;
    //Returns the last element in the list
    //Precondition: length != 0

    bool empty() const;
    //Determines whether a list is empty.

    listdata* getIterator() const;
    //returns the element currently pointed at by the iterator
    //Precondition: iterator != NULL

    int getLength() const;
    //Returns the size of the list

    bool offEnd() const;
    //returns whether the iterator is off the end of the list

    bool isSorted() const;
    //Wrapper function that calls the isSorted helper function to determine whether
    //a list is sorted in ascending order.
    //We will consider that a list is trivially sorted if it is empty.
    //Therefore, no precondition is needed for this function

    int getIndex() const;
    //Indicates the index of the Node where the iterator is currently pointing
    //Nodes are numbered from 1 to size of the list
    //Pre: length != 0
    //Pre: !offEnd()

    int linearSearch(listdata data);
    //Searchs the list, element by element, from the start of the List to the end of the List
    //Pre: length != 0

    int binarySearch(listdata data);
    //Returns the index where data is located in the List
    //Calls the private helper function binarySearch to perform the search
    //Pre: length != 0
    //Pre: List is sorted (must test on a sorted list)

    /**Manipulation Procedures*/

    void deleteEnd();
    //Removes the value of the last element in the list
    //Precondition: length != 0
    //Postcondition: one node before the deleted node points to NULL

    void deleteBegin();
    //Removes the value of the first element in the list
    //Precondition: length != 0
    //Postcondition: begin pointer points to the new address

    void insertEnd(listdata data);
    //Inserts a new element at the end of the list
    //If the list is empty, the new element becomes both first and last
    //Postcondition: new node points to NULL, end pointer points to the new address

    void insertBegin(listdata data);
    //Inserts a new element at the start of the list
    //If the list is empty, the new element becomes both first and last
    //Postcondition: new node points to the nextnode, begin pointer points to the new address

    void beginIterator();
    // moves iterator to the start of the line

    void advanceIterator();
    // moves the iterator up by one node
    // Precondition: iterator != NULL

    void reverseIterator();
    //moves the iterator down by one node
    //Precondition: iterator != NULL

    void deleteIterator();
    // removes the element currently pointed to by the iterator.
    // Precondition: iterator != NULL
    // Postcondition: iterator = NULL

    void insertIterator(listdata d);
    //inserts an element after the node currently pointed to by the iterator
    //Precondition: !offEnd
    //Postcondition: !offEnd
    
    void advanceToIndex(int index);
    //Moves the iterator to the node whose index number is specified in the parameter
    //Pre: length != 0
    //Pre: index <= length

    /**Additional List Operations*/

    void print() const;
    //prints the contents of the linked list to the screen separated by a blank space
    //Prints nothing if the list is empty

    void printNumberedList() const;
    //prints the contents of the linked list to the screen in the format #. <element>
    //followed by a newline ==: compares two lists to see if they are equal.
    //Prints nothing if the list is empty

    void print_reverse() const;
    //Calls the reverse helper function to print a list in reverse
    //prints nothing if the List is empty

    bool operator==(const List& obj) const;
    // overload == operator
};

template <class listdata>
List<listdata>::List()
{
    begin = NULL;
    end = NULL;
    iterator = NULL;
    length = 0;
}

template <class listdata>
List<listdata>::List(const List& obj)
{
    if (obj.begin == NULL)
    {
        begin = end = iterator= NULL;
    }
    else {
        begin = new Node(obj.begin->data);
        NodePtr temp = obj.begin;
        iterator = begin;
        while (temp->nextnode != NULL)
        {
            temp = temp->nextnode;
            iterator->nextnode = new Node(temp->data);
            iterator = iterator->nextnode;
        }
        end = iterator;
        iterator = obj.iterator;
    }
    length = obj.length;
}

template <class listdata>
List<listdata>::~List()
{
    NodePtr after = begin;
    NodePtr before;
    while (after != NULL) {
        before = after->nextnode;
        delete after;
        after = before;
    }
}

template <class listdata>
void List<listdata>::print() const
{
    NodePtr temp = begin;
    while (temp != NULL) {
        cout << temp->data << endl;
        temp = temp->nextnode;
    }
    cout << endl;
}

//  Used for Poem.cpp
template <class listdata>
void List<listdata>::printNumberedList() const
{
    NodePtr temp = begin;
    int count = 1;
    while (temp != NULL)
    {
        cout << count << ". " << temp->data << endl;
        temp = temp->nextnode;
        count++;
    }
    cout << endl;
}

template <class listdata>
void List<listdata>::print_reverse() const
{
    reverse(end);
}

template <class listdata>
void List<listdata>::reverse(NodePtr node) const
{
    if (node == NULL)
    {
        cout << endl;
        return;
    }
    cout << node->data << " ";;
    reverse(node->previousnode);
}

template <class listdata>
void List<listdata>::insertBegin(listdata data){

    NodePtr N = new Node(data);
    if (length == 0)
    {
        begin = end = N;
    }
    else
    {
        N->nextnode = begin;
        begin->previousnode = N;
        begin = N;
    }
    length++;
}

template <class listdata>
void List<listdata>::insertEnd(listdata data) {

    NodePtr N = new Node(data);
    if (length == 0)
    {
        begin = end = N;
    }
    else
    {
        end->nextnode = N;
        N->previousnode = end;
        end = N;
    }
    length++;
}

template <class listdata>
void List<listdata>::deleteBegin() {

    assert (length != 0);
    if (length == 1)
    {
        delete begin;
        begin = end = iterator = NULL;
        length = 0;
    }
    else
    {
        NodePtr temp = begin;
        begin = begin->nextnode;
        begin->previousnode = NULL;
        delete temp;
        length--;
    }
}

template <class listdata>
void List<listdata>::deleteEnd() {

    assert (length != 0);
    if (length == 1)
    {
        delete begin;
        begin = end = iterator = NULL;
        length = 0;
    }
    else
    {
        NodePtr temp = begin;
        while (temp->nextnode != end)
        {
            temp = temp->nextnode;
        }
        delete end;
        end = temp;
        end->nextnode = NULL;
        length--;
    }
}

template <class listdata>
void List<listdata>::beginIterator()
{
    iterator = begin;
}

template <class listdata>
void List<listdata>::advanceIterator()
{
    assert(iterator != NULL);
    iterator = iterator->nextnode;
}

template <class listdata>
void List<listdata>::reverseIterator()
{
    assert(iterator != NULL);
    iterator = iterator->previousnode;
}

template <class listdata>
void List<listdata>::advanceToIndex(int index)
{
    assert(length != 0);
    assert(index <= length);
    assert(index > 0);
    iterator = begin;
    for (int i = 0; i < index-1; i++)
        iterator = iterator->nextnode;
}

template <class listdata>
void List<listdata>::deleteIterator()
{
    assert(iterator != NULL);
    if (iterator == begin)
        deleteBegin();
    else if (iterator == end)
        deleteEnd();
    else
    {
        iterator->previousnode->nextnode = iterator->nextnode;
        iterator->nextnode->previousnode = iterator->previousnode;
        delete iterator;
        length--;
    }
    iterator = NULL;
}

template <class listdata>
void List<listdata>::insertIterator(listdata d)
{
    assert(!offEnd());
    if (iterator == end)
    {
        insertEnd(d);
    }
    else
    {
        NodePtr N = new Node(d);
        N->nextnode = iterator->nextnode;
        N->previousnode = iterator;
        iterator->nextnode->previousnode = N;
        iterator->nextnode = N;
        length++;
    }
}

template <class listdata>
bool List<listdata>::empty() const
{
    return length == 0;
}

template <class listdata>
listdata* List<listdata>::getIterator() const
{
    assert(iterator != NULL);
    return &iterator->data;
}

template <class listdata>
int List<listdata>::getLength() const
{
    return length;
}

template <class listdata>
listdata List<listdata>::getBegin() const
{
    assert(!empty());
    return begin -> data;
}

template <class listdata>
listdata List<listdata>::getEnd() const
{
    assert(!empty());
    return end -> data;
}

template <class listdata>
bool List<listdata>::offEnd() const
{
    return iterator == NULL;
}

template <class listdata>
bool List<listdata>::isSorted() const
{
    if (length <= 1)
        return true;
    return isSorted(begin);
}

template <class listdata>
bool List<listdata>::isSorted(NodePtr node) const
{
    if (node->nextnode == NULL)
        return true;
    if (node->data <= node->nextnode->data)
        return isSorted(node->nextnode);
    return false;
}

template <class listdata>
int List<listdata>::getIndex() const
{
    assert(length != 0);
    assert(!offEnd());
    int count = 1;
    NodePtr temp = begin;
    for (int i = 0; i < length; i++)
    {
        if (iterator == temp)
            break;
        temp = temp->nextnode;
        count++;
    }
    return count;
}

template <class listdata>
int List<listdata>::linearSearch(listdata data)
{
    assert(length != 0);
    bool found = false;
    int index = 0;
    NodePtr temp = iterator;

    for (int i = 1; i < length+1 && !found; i++)
    {
        advanceToIndex(i);
        if (*getIterator() == data)
        {
            found = true;
            index = getIndex();
            iterator = temp;
            return index;
        }
    }
    iterator = temp;
    return -1;
}

template <class listdata>
int List<listdata>::binarySearch(listdata data)
{
    assert(length != 0);
    assert(isSorted());
    return binarySearch(1, length, data);
}

template <class listdata>
int List<listdata>::binarySearch(int low, int high, listdata data)
{
    NodePtr temp = iterator;

    if (high < low)
        return -1;

    int mid = low + (high-low) / 2;
    advanceToIndex(mid);
    if (getIterator() == data)
    {
        iterator = temp;
        return mid;
    }
    else if (getIterator() > data)
        return binarySearch(low, mid -1, data);
    else
        return binarySearch(mid +1, high, data);
}

template <class listdata>
bool List<listdata>::operator==(const List &obj) const
{
    if (length != obj.length)
        return false;
    NodePtr temp1 = begin;
    NodePtr temp2 = obj.begin;
    while(temp1 != NULL)
    {
        if (temp1->data != temp2->data)
            return false;
        temp1 = temp1->nextnode;
        temp2 = temp2->nextnode;
    }
    return true;
}

#endif /* LIST_H_ */
