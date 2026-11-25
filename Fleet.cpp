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

Fleet::~Fleet(){
    UnitNode* curr = head;
        while (curr != nullptr) {
            UnitNode* nextNode = curr->next;
            delete curr->unit; // Fleet owns the DeliveryUnit objects
            delete curr;
            curr = nextNode;
        }
        head = nullptr;
        count = 0;
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

    UnitNode* newNode = new UnitNode{u, nullptr};

    //case 1, empty list or new ID belongs before curent head (ascending)
    if (head == nullptr || id < head->unit->getID()) {
        newNode->next = head;
        head = newNode;
    } 

    //case 2, walk forward until curr->next has a larger ID or reached end node
    else {
        UnitNode* curr = head;
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

bool Fleet::removeUnit(int id){
    UnitNode* curr = head;
    UnitNode* prev = nullptr;

    //walk through the list to find the target ID
    while (curr != nullptr && curr->unit->getID() < id) {
        prev = curr;
        curr = curr->next;
    }

    //if target ID not found in list, cannot be removed
    if (curr == nullptr || curr->unit->getID() != id) {
        cout << "Unit ID " << id << " not found." << endl;
        return false;
    }

    //unlink curr from linked list
    if (prev == nullptr) {
        head = curr->next;       //remove head node
    } else {
        prev->next = curr->next; //bypass curr
    }

    //remove pointer nbefore removing the object
    index.remove(id);
    delete curr->unit; //free the unit from fleet
    delete curr;       //free the linked list node
    count--;
    return true;
}

DeliveryUnit* Fleet::findQuiet(int id) const
{
    UnitNode* curr = head;
    while (curr != nullptr) {
        int currID = curr->unit->getID();
        if (currID == id) return curr->unit; //found
        if (currID > id) return nullptr;     //early exit
        curr = curr->next;
    }
    return nullptr;
}

DeliveryUnit* Fleet::findByID(int id) const{
    int comparisons = 0;
    UnitNode* curr = head;
    while (curr != nullptr) {
        comparisons++; //one comparison per examined node
        int currID = curr->unit->getID();
        if (currID == id) {
            cout << "Linked list search, found ID " << id
                 << " after " << comparisons << " comparisons." << endl;
            return curr->unit;
        }
        //stop at first larger ID
        if (currID > id) {
            break;
        }
        curr = curr->next;
    }
    cout << "Linked list search, ID " << id
         << " not found after " << comparisons << " comparisons." << endl;
    return nullptr;
}

DeliveryUnit* Fleet::searchTree(int id) const
{
    int comparisons = 0;
    DeliveryUnit* found = index.search(id, comparisons);

    if (found != nullptr) {
        cout << "BST search: found ID " << id << " after " << comparisons << " comparisons." << endl;
    } 
    else {
        cout << "BST search: ID " << id << " not found after " << comparisons << " comparisons." << endl;
    }
    return found;

}

int Fleet::getSize() const { return count; }

void Fleet::displayAll() const {
    if (head == nullptr) {
        cout << "Fleet is empty." << endl;
        return;
    }

    UnitNode* curr = head;
    while (curr != nullptr) {
        curr->unit->display();
        curr = curr->next;
    } 
}

void Fleet::displayTree() const
{
    cout << "In-order:   ";
    index.inOrder();
    cout << endl;

    cout << "Pre-order:  ";
    index.preOrder();
    cout << endl;

    cout << "Post-order: ";
    index.postOrder();
    cout << endl;

    cout << "Height: " << index.height() << " (" << count << " units)" << endl;
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
