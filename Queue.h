// Queue.h
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: Sophia Rahman and Nafisa Patwary
//
// A first-in first-out queue built on your own singly linked list (Section 4.1).
// Templates must be written entirely in the header file.

#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
class Queue
{
private:
    struct Node
    {
        T data;
        Node* next;
    };
    Node* frontNode;
    Node* backNode;
    int count;

public:
    Queue() : frontNode(nullptr), backNode(nullptr), count(0) {}

    ~Queue()
    {
        // TODO: free every node (hint: clear())
        clear();
    }

    // A queue owns its nodes, so copying is not allowed
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& item)
    {
        // TODO: add a new node at the back
        Node* n = new Node;
        n->data = item;
        n->next = nullptr;

        if(backNode == nullptr){
            frontNode = n;
            backNode = n;
        }else{
            backNode->next = n;
            backNode = n;
        }
        count++;

    }

    bool dequeue(T& item)
    {
        // TODO: remove the front node, copy its data into item, return true; false if empty
        if(isEmpty()){
            return false;
        }

        Node* temp = frontNode;
        item = temp->data;
        frontNode = frontNode->next;

        if(isEmpty()){
            backNode = nullptr;
        }

        delete temp;
        count--;
        return true;
    }

    bool front(T& item) const
    {
        // TODO: copy the front data into item without removing it
        if(isEmpty()){
            return false;
        }
        item = frontNode->data;
        return true;
    }

    bool isEmpty() const { return frontNode == nullptr; }
    int size() const { return count; }

    void clear()
    {
        // TODO: remove every node
        while(!isEmpty()){
            Node* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
        backNode = nullptr;
        count = 0;
    }
};

#endif
