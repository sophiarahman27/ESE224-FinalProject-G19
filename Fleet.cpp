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
        delete curr->unit; //fleet owns delivery unit objects
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
        newNode->next = curr->next; //traversing list
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
    } 
    else {
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
        int currID = curr->unit->getID(); //inc ID
        if (currID == id) {
            cout << "Linked list search, found ID " << id << " after " << comparisons << " comparisons." << endl; //printing format
            return curr->unit;
        }
        //stop at first larger ID
        if (currID > id) {
            break;
        }
        curr = curr->next;
    }
    cout << "Linked list search, ID " << id << " not found after " << comparisons << " comparisons." << endl;
    return nullptr;
}

DeliveryUnit* Fleet::searchTree(int id) const{
    int comparisons = 0; //keep track of # of comparisons
    DeliveryUnit* found = index.search(id, comparisons); //call function
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
    //if empty, print message and return
    if (head == nullptr) {
        cout << "Fleet is empty." << endl;
        return;
    }

    //otherwise, walk through the list and call display() on each unit
    UnitNode* curr = head;
    while (curr != nullptr) {
        curr->unit->display();
        curr = curr->next;
    } 
}

void Fleet::displayTree() const{
    //print IDs in ascending sorted order
    cout << "In-order:   ";
    index.inOrder();
    cout << endl;
    //print IDs in insertion/structure order
    cout << "Pre-order:  ";
    index.preOrder();
    cout << endl;
    //print IDs bottom up
    cout << "Post-order: ";
    index.postOrder();
    cout << endl;

    cout << "Height: " << index.height() << " (" << count << " units)" << endl;
}

void Fleet::countByStatus() const {
    int droneCount = 0;
    int heavyCount = 0;
    int idleCount = 0;
    int chargingCount = 0;
    int maintCount = 0;
    double totalBattery = 0.0;

    //traverse fleetes linked list once to gather unit stats
    UnitNode* curr = head;
    while (curr != nullptr) {
        DeliveryUnit* u = curr->unit;
        //check for heavy, increment, and other for non heavy
        if (u->type() == "Heavy") {
            heavyCount++;
        } 
        else {
            droneCount++;
        }

        //check statuses and add based on status
        string status = u->getStatus();
        if (status == "IDLE") {
            idleCount++;
        } else if (status == "CHARGING") {
            chargingCount++;
        } else if (status == "MAINTENANCE") {
            maintCount++;
        }

        //accumulate battery percentage to find the average
        totalBattery += u->getBattery();
        curr = curr->next;
    }

    double avgBattery = 0.0;
    //avg calculation
    if (count > 0){
        avgBattery = totalBattery / count;
    } 

    cout << fixed << setprecision(1);
    cout << "Total units: " << count << " (Drones: " << droneCount << ", HeavyDrones: " << heavyCount << ")" << endl;
    //status
    cout << "Status counts - IDLE: " << idleCount  << ", CHARGING: " << chargingCount << ", MAINTENANCE: " << maintCount << endl;

    cout << "Average battery: " << avgBattery << "%" << endl;
    cout << "Queues & Stack - Pending: " << pending.size()  << ", Retry: " << retry.size()  << ", History: " << history.size() << endl;
}

void Fleet::writeFleetToFile(const string& filename) const {
    ofstream out(filename);
    //file error handling
    if (!out) {
        cout << "Error opening output file: " << filename << endl;
        return;
    }

    out << fixed << setprecision(1);
    UnitNode* curr = head;
    while (curr != nullptr) {
        DeliveryUnit* u = curr->unit;
        char typeChar;
        //drone type
        if (u->type() == "Heavy"){
            typeChar = 'H';
        }
        else{
            typeChar = 'D';
        }
        //formatting output to file
        out << typeChar << " "
            << u->getName() << " "
            << u->getID() << " "
            << u->getModel() << " "
            << u->getBattery() << " "
            << u->getMaxPayload() << " "
            << u->getPosition(0) << " "
            << u->getPosition(1) << " "
            << u->getStatus() << " "
            << u->getDeliveriesCompleted() << endl;
        curr = curr->next;
    }
}

// ---------- Dispatch center ----------
DeliveryUnit* Fleet::findNearestAvailable(int x, int y, double weight) const{
    DeliveryUnit* best = nullptr;
    double bestDist = 0.0;

    //traverse list in ascending ID order
    UnitNode* curr = head;
    while (curr != nullptr) {
        DeliveryUnit* u = curr->unit;
        //check if unit can deliver the order
        if (u->canDeliver(x, y, weight)) {
            double d = u->distanceTo(x, y);
            //swap based on distance if best is null or new distance is smaller
            if (best == nullptr || d < bestDist) {
                best = u;
                bestDist = d;
            }
        }
        curr = curr->next;
    }
    return best;
}

void Fleet::appendLog(const string& logFile, const string& line) const
{
   //prints and appends line to log file
    cout << line << endl;
    ofstream out(logFile, ios::app);
    out << line << endl;
}

int Fleet::loadOrders(const string& orderFile, const string& logFile)
{
    //file error handling
    ifstream in(orderFile);
    if (!in) {
        cout << "Error opening order file: " << orderFile << endl;
        return 0;
    }

    //starts new empty log file 
    ofstream clearLog(logFile, ios::trunc);
    clearLog.close();

    int loaded = 0;
    Order ord;
    //read orders from file and enqueue them into pending queue
    while (in >> ord.orderID >> ord.x >> ord.y >> ord.weight) {
        pending.enqueue(ord);
        loaded++;
    }
    return loaded;
}

void Fleet::processPending(const string& logFile)
{
    int totalProcessed = 0;
    int assignedCount = 0;
    int retryMovedCount = 0;
    double totalDistance = 0.0;

    Order ord;

    //dequeue orders one at a time and attempt to assign them to the nearest available unit
    while (pending.dequeue(ord)) {
        totalProcessed++;
        //find closest eligble unit
        DeliveryUnit* unit = findNearestAvailable(ord.x, ord.y, ord.weight);

        if (unit != nullptr) {
            //save unit's state before delivery for undo functionality
            DispatchRecord rec{
                ord,
                unit->getID(),
                unit->getBattery(),
                unit->getPosition(0),
                unit->getPosition(1),
                unit->getStatus()
            };
            //push that
            history.push(rec);

            //capture exact distance and initial battery, then execute batteryNeeded
            double dist = unit->distanceTo(ord.x, ord.y);
            double oldBatt = unit->getBattery();
            unit->completeDelivery(ord.x, ord.y, ord.weight);
            double newBatt = unit->getBattery();

            totalDistance += dist; //add exact distance before formatting
            assignedCount++;

            //log formatting
            ostringstream oss;
            oss << fixed << setprecision(1);
            oss << "Order " << ord.orderID << ": " << unit->type() << " " << unit->getID() << " (" << unit->getName() << ") distance " << dist << " battery " << oldBatt << " -> " << newBatt; 
            appendLog(logFile, oss.str());
        } 
        else {
            //if no unit can deliver, move order to retry queue and log it
            retry.enqueue(ord);
            retryMovedCount++;

            ostringstream oss;
            oss << "Order " << ord.orderID << ": no unit available, moved to retry queue";
            appendLog(logFile, oss.str());
        }
    }

    //print and append batch summary line with 1 decimal place formatting
    ostringstream summary;
    summary << fixed << setprecision(1);
    summary << "Processed " << totalProcessed << " orders: " << assignedCount << " assigned, " << retryMovedCount << " moved to retry queue. Distance flown: " << totalDistance;
    appendLog(logFile, summary.str());
}

bool Fleet::undoLastDispatch(const string& logFile){
    
    DispatchRecord rec;
    //if stack empty, print message, otherwise, pop most recent record
    if (!history.pop(rec)) {
        cout << "Nothing to undo." << endl;
        return false;
    }

    DeliveryUnit* unit = findQuiet(rec.unitID);
    //move undone order back to retry queue, regardless of whether the unit is still in the fleet
    retry.enqueue(rec.order);

    //if unit is no longer in the fleet, log that and return true
    if (unit == nullptr) {
        ostringstream oss;
        oss << "Undo order " << rec.order.orderID << ": unit " << rec.unitID << " no longer in fleet, order moved to retry queue";
        appendLog(logFile, oss.str());
        return true;
    }

    //restore battery, position, status 
    unit->undoDelivery(rec.oldBattery, rec.oldX, rec.oldY, rec.oldStatus);

    //udno order log formatting
    ostringstream oss;
    oss << fixed << setprecision(1);
    oss << "Undo order " << rec.order.orderID << ": "
        << unit->type() << " " << unit->getID()
        << " (" << unit->getName() << ") restored to battery " << unit->getBattery()
        << " at (" << unit->getPosition(0) << ", " << unit->getPosition(1) << ") "
        << unit->getStatus() << ", order moved to retry queue";
    appendLog(logFile, oss.str());
    return true;
}

int Fleet::rechargeAll(const string& logFile){
    int recharged = 0;
    UnitNode* curr = head;

    //only recharge units in charging status, set battery to 100% and status to IDLE
    while (curr != nullptr) {
        DeliveryUnit* u = curr->unit;
        if (u->getStatus() == "CHARGING") {
            u->setBattery(100.0);
            u->setStatus("IDLE");
            recharged++;
        }
        curr = curr->next;
    }

    //log format
    ostringstream oss;
    oss << "Recharged " << recharged << " units to 100%.";
    appendLog(logFile, oss.str());
    return recharged;
}

void Fleet::retryFailed(const string& logFile)
{
    int countToRetry = retry.size();
    Order ord;

    //dequeue from retry queue and enqueue back to pending queue for reprocessing
    while (retry.dequeue(ord)) {
        pending.enqueue(ord);
    }

    //log retry count before calling processPending to handle the orders again
    ostringstream oss;
    oss << "Retrying " << countToRetry << " orders.";
    appendLog(logFile, oss.str());

    processPending(logFile);
}

int Fleet::pendingCount() const { return pending.size(); }
int Fleet::retryCount() const { return retry.size(); }
int Fleet::historyCount() const { return history.size(); }
