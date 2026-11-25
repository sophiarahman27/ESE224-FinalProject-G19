// Fleet.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: Sophia Rahman, Nafisa Patwary
//
// The fleet is a singly linked list kept in ascending ID order (Section 5) plus the dispatch center (Section 7).
// No vector or other STL containers: use your own list, Queue, Stack, and DroneTree.

#include "Fleet.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

Fleet::Fleet() : head(nullptr), count(0) {}

Fleet::~Fleet()
{
    // TODO: delete every node AND every unit (the Fleet owns the units)
}

// ---------- Linked list ----------
bool Fleet::addUnit(DeliveryUnit* u){
    if (u == nullptr) return false;

    int id = u->getID();

    //check if ID already exists in list
    if (findQuiet(id) != nullptr) {
        cout << "Duplicate ID " << id << ": unit not added." << endl;
        delete u;
        return false;
    }

    Node* newNode = new Node{u, nullptr};

    //case 1, empty list or new ID belongs before curent head (ascending)
    if (head == nullptr || id < head->unit->getID()) {
        newNode->next = head;
        head = newNode;
    } 

    //case 2, walk forward until curr->next has a larger ID or reached end node
    else {
        Node* curr = head;
        while (curr->next != nullptr && curr->next->unit->getID() < id) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }

    //keep BST index synchronized with linked list and update size
    index.insert(u);
    count++;
    return true;
}

bool Fleet::removeUnit(int id)
{
Node* curr = head;
    Node* prev = nullptr;

    // Traverse the sorted list until we reach 'id' or pass where it would be
    while (curr != nullptr && curr->unit->getID() < id) {
        prev = curr;
        curr = curr->next;
    }

    // Target ID was not found in the list
    if (curr == nullptr || curr->unit->getID() != id) {
        cout << "Unit ID " << id << " not found." << endl;
        return false;
    }

    // Unlink 'curr' from the singly linked list
    if (prev == nullptr) {
        head = curr->next;       // Removing the head node
    } else {
        prev->next = curr->next; // Bypass 'curr' in the middle or end
    }

    // Remove pointer from BST index BEFORE deleting the unit object itself
    index.remove(id);
    delete curr->unit; // Fleet owns the DeliveryUnit, so free it
    delete curr;       // Free the linked list node
    count--;
    return true;
}

DeliveryUnit* Fleet::findQuiet(int id) const
{
    // TODO: list lookup without printing (used by addUnit and undo)
    return nullptr;
}

DeliveryUnit* Fleet::findByID(int id) const
{
    // TODO: one comparison per node examined; stop at the first larger ID; print the count
    return nullptr;
}

DeliveryUnit* Fleet::searchTree(int id) const
{
    // TODO: index.search(...) and print the count
    return nullptr;
}

int Fleet::getSize() const { return count; }

void Fleet::displayAll() const { /* TODO */ }

void Fleet::displayTree() const
{
    // TODO: in-order, pre-order, post-order, and height
}

void Fleet::countByStatus() const { /* TODO */ }

void Fleet::writeFleetToFile(const string& filename) const { /* TODO */ }

// ---------- Dispatch center ----------
DeliveryUnit* Fleet::findNearestAvailable(int x, int y, double weight) const
{
    // TODO: closest unit that canDeliver; a tie goes to the smaller ID
    return nullptr;
}

void Fleet::appendLog(const string& logFile, const string& line) const
{
    // Prints a line and appends it to the log file
    cout << line << endl;
    ofstream out(logFile, ios::app);
    out << line << endl;
}

int Fleet::loadOrders(const string& orderFile, const string& logFile)
{
    // TODO: enqueue every order into 'pending' in file order; start a new, empty log file
    return 0;
}

void Fleet::processPending(const string& logFile)
{
    // TODO: dequeue until empty; dispatch (push a DispatchRecord first) or move to 'retry'; log each line and a summary
}

bool Fleet::undoLastDispatch(const string& logFile)
{
    // TODO: pop the last record, undoDelivery on that unit, move the order to 'retry'
    return false;
}

int Fleet::rechargeAll(const string& logFile)
{
    // TODO: every CHARGING unit -> battery 100, IDLE
    return 0;
}

void Fleet::retryFailed(const string& logFile)
{
    // TODO: move every order from 'retry' to 'pending' (keep the order), then processPending
}

int Fleet::pendingCount() const { return pending.size(); }
int Fleet::retryCount() const { return retry.size(); }
int Fleet::historyCount() const { return history.size(); }
