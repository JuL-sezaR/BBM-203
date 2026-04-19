#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal() : autoDispatchEnabled(false)
{
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        departureYard[i] = TrainTrack(static_cast<Destination>(i));
    }
}

RailMarshal::~RailMarshal()
{
    // TODO: Cleanup remaining trains, prevent memory leaks
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        while (!departureYard[i].isEmpty())
        {
            Train *t = departureYard[i].departTrain();
            delete t;
        }
    }

    classificationYard.clear();
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    // TODO: Parse user commands from input lines.
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd == "ADD_WAGON")
    {
        int id, weight, maxCouplerLoad;
        std::string destStr, cargoStr;
        iss >> id >> cargoStr >> destStr >> weight >> maxCouplerLoad;
        
        if (iss.fail() || destStr.empty() || cargoStr.empty()) {
        std::cout << "Error: Invalid ADD_WAGON parameters.\n";
        return;
        }

        CargoType cargo = parseCargo(cargoStr);
        Destination dest = parseDestination(destStr);

        Wagon *w = new Wagon(id, cargo, dest, weight, maxCouplerLoad);

        classificationYard.insertWagon(w);
        std::cout << "Wagon " << *w << " added to yard." << std::endl;
    }

    // if REMOVE_WAGON
    // Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;

    else if (cmd == "REMOVE_WAGON")
    {
        int id;
        if (!(iss >> id)) 
        {
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
        }

        if (classificationYard.removeWagon(id))
        {
            std::cout << "Wagon " << id << " removed." << std::endl;
        }
        else
        {
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
        }
    }


    // if ASSEMBLE_TRAIN
    //  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;

    else if (cmd == "ASSEMBLE_TRAIN")
    {
        std::string destStr;
        iss >> destStr;
        Destination dest = parseDestination(destStr);
        
        if (dest == Destination::OTHERS && toUpper(destStr) != "OTHERS")
        {
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }

        TrainTrack &track = departureYard[static_cast<int>(dest)];
        std::string trainName = track.generateTrainName();

        Train *t = classificationYard.assembleTrain(dest, trainName);
        if (!t)
        {
            std::cout << "No wagons to assemble for " << destStr << std::endl;
            return;
        }

        int splitCount = 1;
        
        // Keep splitting the MAIN train until no more overloads
        while (true)
        {
            Train *splitTrain = t->verifyCouplersAndSplit(1);  // Always pass 1, not splitCount!
            if (!splitTrain || !splitTrain->getWagons().getFront())
            {
                if (splitTrain) delete splitTrain;
                break;
            }
            
            std::string splitTrainName = trainName + "_split_" + std::to_string(splitCount);
            splitTrain->setName(splitTrainName);
            track.addTrain(splitTrain);
            
            std::cout << "Train " << splitTrain->getName() << " assembled after split with "
                    << splitTrain->getWagons() << " wagons." << std::endl;

            splitCount++;
        }
        
        track.addTrain(t);
        std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;
    }

    // if DISPATCH_TRAIN
    //  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;

    else if (cmd == "DISPATCH_TRAIN")
    {
        std::string destStr;
        iss >> destStr;
        Destination dest = parseDestination(destStr);

        if (dest == Destination::OTHERS && toUpper(destStr) != "OTHERS")
        {
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }
    
        TrainTrack &track = departureYard[static_cast<int>(dest)];
        if (track.isEmpty())
        {
            std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
            return;
        }
        Train *train = track.departTrain();
        std::cout << "Dispatching " << train->getName() << " (" << train->getTotalWeight() << " tons)." << std::endl;
        delete train;
    }   

    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";

    else if (cmd == "PRINT_YARD")
    {
        std::cout << "--- classification Yard ---\n";
        classificationYard.print();
    }

    // if PRINT_TRACK
    //  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";

    else if (cmd == "PRINT_TRACK")
    {
        std::string destStr;
        iss >> destStr;
        Destination dest = parseDestination(destStr);

        if (dest == Destination::OTHERS && toUpper(destStr) != "OTHERS")
        {
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        departureYard[static_cast<int>(dest)].printTrack();
    }

    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"

    else if (cmd == "AUTO_DISPATCH")
    {
        std::string option;
        iss >> option;
        option = toUpper(option);
        if (option == "ON")
        {
            autoDispatchEnabled = true;
            TrainTrack::setAutoDispatch(true);
            std::cout << "Auto dispatch enabled" << std::endl;
        }
        else if (option == "OFF")
        {
            autoDispatchEnabled = false;
            TrainTrack::setAutoDispatch(false);
            std::cout << "Auto dispatch disabled" << std::endl;
        }
        else
        {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }

    }

    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;

    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;

    else if (cmd == "CLEAR")
    {
        classificationYard.clear();
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
        {
            while (!departureYard[i].isEmpty())
            {
                Train *t = departureYard[i].departTrain();
                delete t;
            }
        }
        std::cout << "System cleared." << std::endl;
    }
    else
    {
        std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    }
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    // TODO: Dispatch the next train (frontmost) from the specified track.
    // std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
    /*std::cout << "Train " << t->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";
     */
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}
