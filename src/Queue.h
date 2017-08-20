#ifndef QUEUE_H_
#define QUEUE_H_

#include <iostream>
#include <cstddef>
#include <assert.h>
using namespace std;

template <class queuedata>
class Queue
{
    public:
        /**constructors and destructors*/

        Queue();
        //initializes an empty queue
        //postcondition: an empty queue

        Queue(const Queue &queue);
        //initializes queue to have same elements as another queue
        //postcondition: a copy of queue

        ~Queue();
        //frees memory allocated to the queue
        //postcondition: memory for queue has been freed

        /**manipulation procedures*/

        void dequeue();
        //removes an element from the front of the queue
        //precondition:size!=0
        //postcondition: an element has been removed from the front of the queue

        void enqueue(queuedata data);
        //adds an element to the end of the queue
        //postcondition: an element added to the end of the queue

        /**accessors*/

        bool operator==(const Queue &queue);
        //returns whether this queue is equal to another queue

        queuedata getFront();
        //returns the element at the front of the queue
        //precondition: size != 0

        int getSize();
        //returns the size of the queue

        bool empty();
        //returns whether the queue is empty

        /**additional queue operations*/
        void print();
        //prints the elements in the queue separated by a blank space to stdout


    private:
        struct Node
        {
            queuedata data;
            Node* link;
            Node(queuedata data): link(NULL), data(data){}
            Node(): link(NULL) {}
        };

        typedef struct Node* NodePtr;

        NodePtr front;
        NodePtr back;
        int size;
};


template <class queuedata>
Queue<queuedata>::Queue(const Queue &queue)
{
       size = queue.size;
       if(queue.front == NULL)
       {
           back = front = NULL;
       }

       else
        {
            front = new Node;
            front->data = queue.front->data;
            NodePtr temp;
            temp = queue.front;
            NodePtr qtemp = front; //create temporary iterator

            while(temp->link != NULL)
            {

                qtemp->link = new Node;
                qtemp = qtemp->link;
                temp = temp->link;
                qtemp->data = temp->data;
            }

            back = qtemp;
        }
}

template <class queuedata>
Queue<queuedata>::~Queue()
{
    NodePtr temp = front;
    NodePtr temp2 = front;
    while (temp != NULL)
    {
        temp = temp->link;
        delete temp2;
        temp2 = temp;
    }
}

template <class queuedata>
void Queue<queuedata>::print()
{
    NodePtr temp = front;
    while (temp != NULL)
    {
        cout << temp->data << " ";
        temp = temp->link;
    }
    cout << endl;
}

#endif




