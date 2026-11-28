// main.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: Nafisa Patwary and Sophia Rahman
//
// This skeleton compiles and shows the menu. Fill in each TODO (Section 8).
// You can reuse your Part I login and input-checking code.

#include "DeliveryUnit.h"
#include "Drone.h"
#include "HeavyDrone.h"
#include "Fleet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

const string LOG_FILE = "delivery_log.txt";

//helper function, recover from a non-numeric input
void clearBadInput()
{
    cin.clear();
    //ignore the rest of the line, up to 1000 characters or until a newline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


// TODO: ask for a username and password until the pair is in users.txt; greet the user
bool login(){
    string user, pass;
    while (true) {
        //ask for username
        cout << "Username: ";
        if (!(cin >> user)) {
            return false;
        }
        //ask for pw
        cout << "Password: ";
        if (!(cin >> pass)) {
            return false;
        }
        //open users.txt and check for the pair
        ifstream in("users.txt");
        if (!in) {
            cout << "Error: could not open users.txt." << endl;
            return false;
        }

        //read every line of users.txt until end of file. Each line has a username and password.
        string fileUser, filePass;
        while (in >> fileUser >> filePass) {
            //compare the input to the file values
            if (fileUser == user && filePass == pass) {
                cout << "Welcome, " << user << "!" << endl;
                return true;
            }
        }
        cout << "Invalid username or password. Please try again." << endl;
    }
}

// TODO: read every line of drones.txt until end of file. The first word is D (Drone) or H (HeavyDrone):
//       create the unit with new, add it to the fleet, and print how many were loaded.
void loadFleet(Fleet& fleet, const string& filename){
    //open the file
    ifstream in(filename);

    //file error handling
    if (!in) {
        cout << "Error: could not open " << filename << "." << endl;
        return;
    }

    //read every line of the file until end of file
    int loaded = 0;
    char typeCode;
    //drone attributes
    string name, model, status;
    int id, x, y;
    double battery, maxPayload;

    //read the first word (D or H) and the rest of the attributes
    while (in >> typeCode >> name >> id >> model >> battery >> maxPayload >> x >> y >> status) {
        DeliveryUnit* unit = nullptr;
        //create the unit based on the type code

        //drone
        if (typeCode == 'D') {
            unit = new Drone(name, id, model, battery, maxPayload, x, y, status);
        } 
        //heavy drone
        else if (typeCode == 'H') {
            unit = new HeavyDrone(name, id, model, battery, maxPayload, x, y, status);
        } 
        else {
            cout << "Unknown unit type '" << typeCode << "' for ID " << id << "; skipping." << endl;
            continue;
        }

        if (fleet.addUnit(unit)) {
            loaded++;
        }
    }
    //debug: print loaded units
    cout << "Loaded " << loaded << " units from " << filename << "." << endl;
}

void showMenu()
{
    cout << endl
         << "===== SkyDrop Dispatch Center =====" << endl
         << " 1. Display fleet (ID order)" << endl
         << " 2. Display ID tree (traversals and height)" << endl
         << " 3. Search by ID (linked list and tree)" << endl
         << " 4. Add a unit" << endl
         << " 5. Remove a unit" << endl
         << " 6. Find nearest available unit" << endl
         << " 7. Load orders into the pending queue" << endl
         << " 8. Process pending orders" << endl
         << " 9. Undo last dispatch" << endl
         << "10. Recharge all charging units" << endl
         << "11. Retry failed orders" << endl
         << "12. Fleet status summary" << endl
         << "13. Write fleet to file" << endl
         << " 0. Quit" << endl;
}

int main()
{
    if (!login())
        return 0;

    Fleet fleet;
    loadFleet(fleet, "drones.txt");

    int choice = -1;
    while (choice != 0)
    {
        showMenu();
        cout << "Choose an option: ";

        //input validation for menu choice
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a menu number." << endl;
            //calls helper function to clear bad input
            clearBadInput();
            choice = -1;
            continue;
        }
       
        //table choices
        switch (choice){
        
        case 1: //display all drones in the fleet (linked list)
            fleet.displayAll();
            break;

        case 2: //display the BST of drones in the fleet 
            fleet.displayTree();
            break;

        case 3: //search by ID running findByID and searchTree
        {
            int id;
            cout << "Enter ID to search: ";
            if (!(cin >> id)){ 
                cout << "Invalid ID." << endl; 
                clearBadInput(); 
                break; 
            }
            //findByID returns a pointer to the unit, or nullptr if not found
            DeliveryUnit* unit = fleet.findByID(id);
            
            //searchTree returns a pointer to the unit, or nullptr if not found
            fleet.searchTree(id);

            //display the unit if found
            if (unit != nullptr) unit->display();
            break;
        }

        case 4: 
        {
            //ask for unit type, then the attributes, create the unit with new, and add it to the fleet
            char typeCode;
            string name, model, status;
            int id, x, y;
            double battery, maxPayload;

            //ask for unit type
            cout << "Enter type (D or H): ";
            //validate input
            cin >> typeCode;
            if (typeCode != 'D' && typeCode != 'H') {
                cout << "Invalid unit type." << endl;
                clearBadInput();
                break;
            }

            //ask for the rest of the attributes and validate
            cout << "Enter name, ID, model, battery, maxPayload, x, y, status: ";
            if (!(cin >> name >> id >> model >> battery >> maxPayload >> x >> y >> status)) {
                cout << "Invalid input format." << endl;
                clearBadInput();
                break;
            }

            DeliveryUnit* unit = nullptr;
            if (typeCode == 'D') {
                unit = new Drone();
            } 
            else {
                unit = new HeavyDrone();
            }

            //checking each attribute for validity using the mutators, if any are invalid, delete the unit and break
            unit->setName(name);
            bool valid = unit->setID(id) && unit->setModel(model) && unit->setBattery(battery) && unit->setMaxPayload(maxPayload) && unit->setPosition(0, x) && unit->setPosition(1, y) && unit->setStatus(status);

            //if any attribute is invalid, delete the unit and break
            if (!valid) {
                cout << "Unit rejected due to invalid attribute values." << endl;
                delete unit;
                break;
            }

            //add the unit to the fleet and print a message
            if (fleet.addUnit(unit)) {
                cout << "Unit " << id << " (" << name << ") added to fleet." << endl;
            }
            break;
        }

        case 5:  //remove a unit by ID
        {
            int id;
            cout << "Enter ID to remove: ";
            //validate input
            if (!(cin >> id)) { 
                cout << "Invalid ID." << endl; 
                clearBadInput(); 
                break; 
            }
            //remove the unit from the fleet and print a message
            if (fleet.removeUnit(id)) cout << "Unit ID " << id << " removed." << endl;
            break;
        }

        case 6: //find the nearest available unit to a given x, y, and weight
        {
            int x, y;
            double weight;
            cout << "Enter target x, y, and weight: ";
            if (!(cin >> x >> y >> weight)) { 
                cout << "Invalid input." << endl; 
                clearBadInput(); 
                break; 
            }

            //findNearestAvailable returns a pointer to the unit, or nullptr if none can deliver
            DeliveryUnit* nearest = fleet.findNearestAvailable(x, y, weight);
            if (nearest != nullptr) nearest->display();
            else cout << "No available unit can deliver this order." << endl;
            break;
        }

        case 7: //load orders from orders.txt and print how many
            cout << "Loaded " << fleet.loadOrders("orders.txt", LOG_FILE) << " orders into the pending queue" << endl;
            break;

        case 8: //process the pending orders and print how many were completed
            fleet.processPending(LOG_FILE);
            break;

        case 9: //undo the last dispatch and print a message
            fleet.undoLastDispatch(LOG_FILE);
            break;

        case 10: //recharge all charging units and print how many were recharged
            fleet.rechargeAll(LOG_FILE);
            break;

        case 11: // retry the failed orders and print how many were retried
            fleet.retryFailed(LOG_FILE);
            break;

        case 12: // print a summary of the fleet status
            fleet.countByStatus();
            break;

        case 13: // write the fleet to a file
            fleet.writeFleetToFile("fleet_output.txt");
            cout << "Fleet written to fleet_output.txt" << endl;
            break;

        case 0: // quit
            cout << "Goodbye!" << endl;
            break;

        default:
            cout << "Invalid option." << endl;
        }
    }
    return 0;   // the Fleet destructor frees everything here
}
