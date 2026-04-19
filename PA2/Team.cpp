#include "Team.h"
#include <iostream>

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //Implement tryAssignRequest function as explained in the PDF.
    (void)req;
    int reqWorkLoad = req.computeWorkloadContribution();
    if (currentWorkload + reqWorkLoad > maxLoadCapacity) {
        return false;
    } else {
        missionStack.push(req);
        currentWorkload += reqWorkLoad;
        return true;
    }
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //Implement rollbackMission function as explained in the PDF.
    (void)supplyQueue;
    (void)rescueQueue;

    int count = missionStack.size();
    if (count == 0) return;

    Request* temp = new Request[count];
    int i = 0;
    while (!missionStack.isEmpty()) {
        Request req;
        missionStack.pop(req);
        temp[i++] = req;
    }

     for (int j = 0; j < count; ++j) {
        if (temp[j].getType() == "SUPPLY") {
            supplyQueue.enqueueFront(temp[j]);
        } else {
            rescueQueue.enqueueFront(temp[j]);
        }
    }


    delete[] temp;
    currentWorkload = 0;
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}

void Team::printMissionStack() const {
    if (missionStack.isEmpty()) return;
    
    Request* data = missionStack.getData();
    int top = missionStack.getTopIndex();
    
    // LIFO: print from top to bottom
    for (int i = top; i >= 0; i--) {
        std::cout << data[i].toString() << std::endl;
    }
}


