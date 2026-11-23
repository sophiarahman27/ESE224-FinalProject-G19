// HeavyDrone.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: Sophia Rahman and Nafisa Patwary

#include "HeavyDrone.h"
#include <iostream>
using namespace std;

HeavyDrone::HeavyDrone() : DeliveryUnit() {}

HeavyDrone::HeavyDrone(const string& n, int id, const string& m, double b, double p, int x, int y, const string& s)
    : DeliveryUnit()
{
    // TODO: set every member through its mutator (Section 3.3)
    setName(n); 
    setID(id);
    setModel(m);
    setBattery(b);
    setMaxPayload(p);
    setPosition(0, x);
    setPosition(1, y);
    setStatus(s);
}

bool HeavyDrone::setModel(const string& m)
{
    // TODO: valid only when m is Hauler
    if(m == "Hauler"){ 
        model = m;
        return true;
    }
    cout << "Error: model must be Hauler" << endl;
    return false;

}

double HeavyDrone::batteryNeeded(int x, int y, double weight) const
{
    // TODO: distance x HEAVY_BATTERY_PER_UNIT x (1 + HEAVY_PAYLOAD_FACTOR x weight)
    return distanceTo(x, y) * HEAVY_BATTERY_PER_UNIT * (1 + HEAVY_PAYLOAD_FACTOR * weight);
}

string HeavyDrone::type() const { return "Heavy"; }
