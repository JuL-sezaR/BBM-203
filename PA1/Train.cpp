#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

// This function is given to you ready
void Train::appendWagonList(WagonList &&wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight = wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{ 
    //TODO: Do the cleaning as necesssary
    if (nextLocomotive) {
        nextLocomotive -> clear();
        delete nextLocomotive;
        nextLocomotive = nullptr;
    }
    
    wagons.clear();
    totalWeight = 0;
}

// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{

    // TODO: Verify whether any wagon’s coupler is overloaded.
    // You must traverse from the rear (backmost wagon) toward the front.
    //
    // Splitting rule:
    // Split the train AFTER the overloaded wagon (the overloaded one stays).
    // Use WagonList::splitAtById to detach the overloaded segment into a new WagonList.
    //
    // If no overload occurs, return nullptr (no split needed).
    //
    // If a valid split occurs:
    // new Train should be named "<oldName>_split_<splitCounter>".

    // print message
    // std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
    // std::cout << newTrain->wagons << std::endl;

    Wagon *current = wagons.getFront();
    Wagon *lastOverloaded = nullptr;
    while (current != nullptr)
    {
        int loadOnCoupler = 0;
        Wagon *temp = current -> getNext();
        
        while (temp != nullptr)
        {
            loadOnCoupler += temp -> getWeight();
            temp = temp -> getNext();
        }

        if (loadOnCoupler > current -> getMaxCouplerLoad())
        {
            lastOverloaded = current;
        }
        current = current -> getNext();
    }

    if (lastOverloaded != nullptr)
    {
        Wagon* nextWagon = lastOverloaded->getNext();

        if (!nextWagon) return nullptr; // No split possible, last wagon is overloaded

        int splitId = nextWagon->getID();
        WagonList newWagonList = wagons.splitAtById(splitId);

        if (newWagonList.isEmpty()) return nullptr; // Split failed, should not happen

        Train* newTrain = new Train(name + "_split_" + std::to_string(splitCounter), destination);
        newTrain->wagons = std::move(newWagonList);

        std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
        std::cout << newTrain->wagons << std::endl;

        this->totalWeight = this->wagons.getTotalWeight();
        newTrain->totalWeight = newTrain->wagons.getTotalWeight();

        return newTrain;
    }

    return nullptr;
}