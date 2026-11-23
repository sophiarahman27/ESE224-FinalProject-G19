// DeliveryUnit.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: Sophia Rahman and Nafisa Patwary
//
// Much of this is your Part I Drone code, moved into the base class (Section 3.2).

#include "DeliveryUnit.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

DeliveryUnit::DeliveryUnit()
{
    // TODO: the Part I defaults [done]
    name = ""; 
    ID = -1;
    model = ""; 
    battery = 100.0;
    maxPayload = 0.0;
    position[0] = 0;
    position[1] = 0;
    status = "IDLE";
    deliveriesCompleted = 0;
}

DeliveryUnit::~DeliveryUnit() {}

// ---------- Accessors ----------
string DeliveryUnit::getName() const { return name; }
int DeliveryUnit::getID() const { return ID; }
string DeliveryUnit::getModel() const { return model; }
double DeliveryUnit::getBattery() const { return battery; }
double DeliveryUnit::getMaxPayload() const { return maxPayload; }
string DeliveryUnit::getStatus() const { return status; }
int DeliveryUnit::getDeliveriesCompleted() const { return deliveriesCompleted; }

int DeliveryUnit::getPosition(int index) const
{
    // TODO: as in Part I [done]
    if(index == 0){      
        return position[0];
    }else if(index == 1){
        return position[1];
    }else{
        cout << "Error: position index must be a 0 or a 1" << endl;
        return -1;
    }
}

// ---------- Mutators (validate as in Part I; setMaxPayload: 0 < p <= MAX_PAYLOAD_LIMIT) ----------
void DeliveryUnit::setName(const string& n) { 
    name = n; 
}

bool DeliveryUnit::setID(int id) { 
    if(id > 0){      
        ID = id;
        return true;
    }
    cout << "Error: ID must be greater than 0" << endl;
    return false;
}

bool DeliveryUnit::setBattery(double b) { 
    if(b >= 0 && b <= 100){ 
        battery = b;
        return true;
    }
    cout << "Error: battery value must be between 0 and 100" << endl; // Any battery level outside of the bounds gets an error message
    return false; 
}

bool DeliveryUnit::setMaxPayload(double p) {
    if(p > 0 && p <= MAX_PAYLOAD_LIMIT){ 
        maxPayload = p;
        return true;
    }
    cout << "Error: max payload value must be between 0 and " << MAX_PAYLOAD_LIMIT << endl;
    return false; 
}

bool DeliveryUnit::setPosition(int index, int value) {
    if((index == 0 || index == 1) && value >= 0){ 
        position[index] = value;
        return true;
    }
    cout << "Error: position index must be 0 or 1 and value must be greater than or equal to 0" << endl;
    return false; 
}

bool DeliveryUnit::setStatus(const string& s) {
    if(s == "IDLE" || s == "CHARGING" || s == "MAINTENANCE"){
        status = s;
        return true;
    }
    cout << "Error: status must be IDLE or CHARGING or MAINTENANCE" << endl;
    return false; 
}

// ---------- Shared behaviour ----------
double DeliveryUnit::distanceTo(int x, int y) const
{
    // TODO [done]
    double dx = x - position[0];      
    double dy = y - position[1];
    return sqrt((dx * dx) + (dy * dy));
}

bool DeliveryUnit::canDeliver(int x, int y, double weight) const
{
    // TODO: as in Part I, but call the virtual batteryNeeded so each unit type uses its own formula
    if(status == "IDLE" && weight <= maxPayload && battery - batteryNeeded(x, y, weight) >= SAFETY_RESERVE){
        return true;
    }                
    return false;
}

void DeliveryUnit::completeDelivery(int x, int y, double weight)
{
    // TODO: as in Part I [done]
    battery = battery - batteryNeeded(x, y, weight); 
    position[0] = x;
    position[1] = y;
    deliveriesCompleted++;
    if(battery < LOW_BATTERY){
        status = "CHARGING";
    }
}

void DeliveryUnit::undoDelivery(double oldBattery, int oldX, int oldY, const string& oldStatus)
{
    // TODO: restore battery, position, and status; deliveriesCompleted goes down by 1 (never below 0) [done]
    battery = oldBattery;
    position[0] = oldX;
    position[1] = oldY;
    status = oldStatus;
    if(deliveriesCompleted > 0){
        deliveriesCompleted--;
    }
}

void DeliveryUnit::display() const
{
    // TODO: print [type()] and every member with a label [done]
    cout << "[" << type() << "]" << endl;
    cout << "Name: " << name << endl; 
    cout << "ID: " << ID << endl;
    cout << "Model: " << model << endl;
    cout << "Battery: " << fixed << setprecision(1) << battery << "%" << endl;
    cout << "Max Payload: " << maxPayload << " kg" << endl;
    cout << "Position: (" << position[0] << ", " << position[1] << ")" << endl;
    cout << "Status: " << status << endl;;
    cout << "Deliveries Completed: " << deliveriesCompleted << endl;
}
