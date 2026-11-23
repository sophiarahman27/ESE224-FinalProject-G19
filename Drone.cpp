// Drone.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: Sophia Rahman and Nafisa Patwary

#include "Drone.h"
#include <iostream>
using namespace std;

Drone::Drone() : DeliveryUnit() {}

Drone::Drone(const string& n, int id, const string& m, double b, double p, int x, int y, const string& s)
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

bool Drone::setModel(const string& m)
{
    // TODO: valid when m is Kestrel, Falcon, or Condor
    if(m == "Kestrel" || m == "Falcon" || m == "Condor"){ 
        model = m;
        return true;
    }
    cout << "Error: model must be Kestrel or Falcon or Condor" << endl;
    return false;
}

double Drone::batteryNeeded(int x, int y, double weight) const
{
    // TODO: distance x BATTERY_PER_UNIT x (1 + PAYLOAD_FACTOR x weight)
    return distanceTo(x, y) * BATTERY_PER_UNIT * (1 + PAYLOAD_FACTOR * weight); 
}

string Drone::type() const { return "Drone"; }
