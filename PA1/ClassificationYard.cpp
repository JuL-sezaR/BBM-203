#include "ClassificationYard.h"
#include <iostream>
#include <algorithm>

#define NUM_DESTINATIONS_INT static_cast<int>(Destination::NUM_DESTINATIONS)
#define NUM_CARGOTYPES_INT static_cast<int>(CargoType::NUM_CARGOTYPES)

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
 // TODO: Collect wagons of the same destination and assemble them into a single Train.

    /**
     * - Blocks of the same cargo type must remain grouped together.
     * - These groups must be appended to the train in descending order
     *   based on their heaviest wagon.
     * - Hazardous cargo (e.g., OIL) must always be placed at the very end of the train,
     *   and only one hazardous block can be included per train.*/

{
    struct CargoBlock{
        int type;
        WagonList wagons;
        int heaviestWagon;
    };

    CargoBlock cargoBlocks[NUM_CARGOTYPES_INT];
    int blockCount = 0;
    int hazardousIndex = -1;

    for (int i = 0; i < NUM_CARGOTYPES_INT; ++i)
    {
        WagonList& wagons = blockTrains[static_cast<int>(dest)][i];
        if (!wagons.isEmpty())
        {
            cargoBlocks[blockCount].type = i;
            cargoBlocks[blockCount].wagons = std::move(wagons);
            
            int maxWeight = 0;
            Wagon* current = cargoBlocks[blockCount].wagons.getFront();
            while (current != nullptr)
            {
                if (current->getWeight() > maxWeight)
                {
                    maxWeight = current->getWeight();
                }
                current = current->getNext();
            }
            cargoBlocks[blockCount].heaviestWagon = maxWeight;

            if (static_cast<CargoType>(i) == CargoType::HAZARDOUS)
            {
                hazardousIndex = blockCount;
            }
            blockCount++;
        }
    }

    if (blockCount == 0) return nullptr;


    for (int i = 0; i < blockCount - 1; i++)
    {
        if (i == hazardousIndex) continue;
        
        for (int j = i + 1; j < blockCount; j++)
        {
            if (j == hazardousIndex) continue;
            
            if (cargoBlocks[i].heaviestWagon < cargoBlocks[j].heaviestWagon)
            {
                std::swap(cargoBlocks[i], cargoBlocks[j]);
                
                if (i == hazardousIndex) hazardousIndex = j;
                else if (j == hazardousIndex) hazardousIndex = i;
            }
        }
    }

    Train* assembledTrain = new Train(trainName, dest);

    for (int i = 0; i < blockCount; ++i)
    {
        if (i == hazardousIndex) continue;
        assembledTrain->appendWagonList(std::move(cargoBlocks[i].wagons));
    }

    if (hazardousIndex != -1)
    {
        WagonList& hazardousList = cargoBlocks[hazardousIndex].wagons;
        
        if (!hazardousList.isEmpty())
        {
            Wagon* firstWagon = hazardousList.getFront();
            Wagon* detached = hazardousList.detachById(firstWagon->getID());
            
            if (detached)
            {
                assembledTrain->addWagonToRear(detached);
            }
        }
    }

    return assembledTrain;
}

bool ClassificationYard::isEmpty() const
{
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */

    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        for (int j = 0; j < NUM_CARGOTYPES_INT; j++)
        {
            if (!blockTrains[i][j].isEmpty())
                return false;
        }
    }
    return true;
}


bool ClassificationYard::removeWagon(int wagonID)
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        for (int j = 0; j < NUM_CARGOTYPES_INT; j++)
        {
            WagonList& wagons = blockTrains[i][j];

            Wagon* removed = wagons.detachById(wagonID);
            if (removed)
            {
                delete removed;
                return true;
            }
        }
    }
    return false;
}

void ClassificationYard::clear()
{
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */
    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        for (int j = 0; j < NUM_CARGOTYPES_INT; j++)
        {
            blockTrains[i][j].clear();
        }
    }
 
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}