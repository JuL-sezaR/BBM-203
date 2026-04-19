#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    // TODO: Delete all Wagon objects in this list and reset pointers.

    if (front == nullptr) return;

    while (front != nullptr) {
        Wagon* temp = front;
        front = front->getNext();
        delete temp;
    }

    rear = nullptr;
    totalWeight = 0;

}


WagonList::WagonList(WagonList &&other) noexcept
{
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state.
    front = other.front;
    rear = other.rear;
    totalWeight = other.totalWeight;

    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;

}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    // Operation version of the move constructor.
    // TODO: Implement it.
    if (this != &other) {
        clear(); // free existing resources

        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
        other.front = nullptr;
        other.rear = nullptr;
        other.totalWeight = 0;
    }
    
    return *this;
}

Wagon *WagonList::findById(int id)
{
    // TODO: Find and return the Wagon with given ID.
    // Return nullptr if not found.

    Wagon *current = front;
    while (current != nullptr)
    {
        if (current->getID() == id)
            return current;
        current = current->getNext();
    }

    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight
    if (!w) return;

    w -> setNext(nullptr);
    w -> setPrev(rear);

    if (isEmpty()) {
        front = w;
        rear = w;
    } else {
        rear -> setNext(w);
        rear = w;
    }

    totalWeight += w -> getWeight();

}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    // TODO: Return true if the list has no wagons.
    if (front == nullptr)
        return true;

    return false;
}

void WagonList::insertSorted(Wagon *wagon)
{
    // TODO: Insert wagon into this list in descending order of weight.
    if (!wagon) return;

    if (isEmpty()) {
        front = wagon;
        rear = wagon;
        wagon -> setPrev(nullptr);
        wagon -> setNext(nullptr);
        totalWeight = wagon -> getWeight();
        return;
    }

    Wagon* current = front;

    while (current != nullptr && current -> getWeight() >= wagon -> getWeight()) {
        current = current -> getNext();
    }

    if (current == front) {
        wagon -> setNext(front);
        wagon -> setPrev(nullptr);
        front -> setPrev(wagon);
        front = wagon;
    } else if (current == nullptr) {
        wagon -> setNext(nullptr);
        wagon -> setPrev(rear);
        rear -> setNext(wagon);
        rear = wagon;
    }
    else {
        wagon -> setNext(current);
        wagon -> setPrev(current -> getPrev());
        current -> getPrev() -> setNext(wagon);
        current -> setPrev(wagon);
    }

    totalWeight += wagon -> getWeight();

}

void WagonList::appendList(WagonList &&other)
{
   // TODO: Append another WagonList to this one (merge them).
   // Use move semantics to avoid deep copies. (Double && at the parameter already makes it so)
   // 'other' should end up empty after this operation
   // At merge lists (blocks) will be protected 
   // But the one with heavier wagon at the front will be before the other list
    if (other.isEmpty()) return;

    if (isEmpty()) {
        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
    } else {
        rear -> setNext(other.front);
        other.front -> setPrev(rear);
        rear = other.rear;
        totalWeight += other.totalWeight;
    }

    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;

}

Wagon *WagonList::detachById(int id)
{
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.

    Wagon *toRemove = findById(id);
    
    if (!toRemove) return nullptr; // Not found

    if (toRemove == front && toRemove == rear) {
        front = nullptr;
        rear = nullptr;
    } else if (toRemove == front) {
        front = toRemove -> getNext();
        front -> setPrev(nullptr);
    } else if (toRemove == rear) {
        rear = toRemove -> getPrev();
        rear -> setNext(nullptr);
    } else {
        toRemove -> getPrev() -> setNext(toRemove -> getNext());
        toRemove -> getNext() -> setPrev(toRemove -> getPrev());
    }

    toRemove -> setNext(nullptr);
    toRemove -> setPrev(nullptr);
    totalWeight -= toRemove -> getWeight();

    std::cout << "Wagon " << toRemove->getID() << " detached from Wagon List. " << std::endl;
    return toRemove;
}


WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.
    Wagon *splitWagon = findById(id);

    if (!splitWagon) return newList; // not found, return empty list

    if (splitWagon == front) {
        newList.front = front;
        newList.rear = rear;
        newList.totalWeight = totalWeight;

        front = nullptr;
        rear = nullptr;
        totalWeight = 0;

        return newList;
    }

    newList.front = splitWagon;
    newList.rear = rear;

    int newListWeight = 0;
    Wagon *current = splitWagon;

    while (current != nullptr) {
        newListWeight += current -> getWeight();
        current = current -> getNext();
    }

    newList.totalWeight = newListWeight;

    Wagon *beforeSplit = splitWagon -> getPrev();
    beforeSplit -> setNext(nullptr);
    rear = beforeSplit;

    // Remove old rear pointer from the newList
    splitWagon -> setPrev(nullptr);

    // Update totalWeight of the original List
    totalWeight -= newListWeight;

    return newList; // moved, not copied
}

bool WagonList::removeById(int wagonID)
{
    Wagon* current = front;

    while (current != nullptr)
    {
        if (current->getID() == wagonID)
        {
            if (current == front && current == rear)
            {
                front = nullptr;
                rear = nullptr;
            }
            else if (current == front)
            {
                front = current->getNext();
                front->setPrev(nullptr);
            }
            else if (current == rear)
            {
                rear = current->getPrev();
                rear->setNext(nullptr);
            }
            else
            {
                current->getPrev()->setNext(current->getNext());
                current->getNext()->setPrev(current->getPrev());
            }

            totalWeight -= current->getWeight();

            delete current;
            return true;
        }
        
        current = current->getNext();
    }
    return false;
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
