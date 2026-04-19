#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "ADD_SUPPLY") {
        std::string id, city, typeStr;
        int amount, emergencyLevel;
        iss >> id >> city >> typeStr >> amount >> emergencyLevel;
        return handleAddSupply(id, city, typeStr, amount, emergencyLevel);
    }
    else if (command == "ADD_RESCUE") {
        std::string id, city, riskStr;
        int numPeople, emergencyLevel;
        iss >> id >> city >> numPeople >> riskStr >> emergencyLevel;
        return handleAddRescue(id, city, numPeople, riskStr, emergencyLevel);
    }
    else if (command == "REMOVE_REQUEST") {
        std::string id;
        iss >> id;
        return handleRemoveRequest(id);
    }
    else if (command == "HANDLE_EMERGENCY") {
        int teamId, k;
        iss >> teamId >> k;
        return handleHandleEmergency(teamId, k);
    }
    else if (command == "DISPATCH_TEAM") {
        int teamId;
        iss >> teamId;
        return handleDispatchTeam(teamId);
    }
    else if (command == "INIT_TEAMS") {
        int numTeams;
        iss >> numTeams;
        return handleInitTeams(numTeams);
    }
    else if (command == "SET_TEAM_CAPACITY") {
        int teamId, capacity;
        iss >> teamId >> capacity;
        return handleSetTeamCapacity(teamId, capacity);
    }
    else if (command == "PRINT_QUEUES") {
        printQueues();
        return true;
    }
    else if (command == "PRINT_TEAM") {
        int teamId;
        iss >> teamId;
        printTeam(teamId);
        return true;
    }
    else if (command == "CLEAR") {
        clear();
        return true;
    }
    std::cout << "Error: Unknown command '" << command << "'." << std::endl << std::endl;
    return true;
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    if (numTeams <= 0) {
        std::cout << "Error: Invalid number of teams." << std::endl;
        return false;
    }
    supplyQueue.clear();
    rescueQueue.clear();

    delete[] teams;
    teams = new Team[numTeams];
    teamCount = numTeams;
    
    for (int i = 0; i < numTeams; i++) {
        teams[i].setId(i);
    }
    std::cout << "Initialized " << numTeams << " teams." << std::endl;
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.

    for (int i = 0; i < teamCount; i++) {
        if (teams[i].getId() == teamId) {
            return i;
        }
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    int idx = findTeamIndexById(teamId);

    if (idx == -1) {
        return true;
    }

    teams[idx].setMaxLoadCapacity(capacity);
    std::cout << "Team " << teamId << " capacity set to " << capacity << "." << std::endl;

    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    Request newRequest(id, cityStr, supplyTypeStr, amount, emergencyLevel);
    supplyQueue.enqueue(newRequest);
    std::cout << "Request " << id << " added to SUPPLY queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request newRequest(id, cityStr, numPeople, riskStr, emergencyLevel);
    rescueQueue.enqueue(newRequest);
    std::cout << "Request " << id << " added to RESCUE queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue. 
    if (supplyQueue.removeById(id)) {
        std::cout << "Request " << id << " removed from queues." << std::endl;
        return true;
    } else if (rescueQueue.removeById(id)) {
        std::cout << "Request " << id << " removed from queues." << std::endl;
        return true;
    } 

    std::cout << "Error: Request " << id << " not found." << std::endl;
    return true;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.
    int teamIdx = findTeamIndexById(teamId);
    if (teamIdx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }

    if (teams[teamIdx].getMaxLoadCapacity() <= 0) {
        std::cout << "Error: Team " << teamId << " has no capacity set." << std::endl;
        return true;
    }

    Team& t = teams[teamIdx];

    for (int step = 0; step < k; step++) {
        Request supplyReq, rescueReq;
        bool hasSupply = supplyQueue.peek(supplyReq);
        bool hasRescue = rescueQueue.peek(rescueReq);

        // If no requests left in both queues → stop
        if (!hasSupply && !hasRescue)
            break;

        double supplyScore = hasSupply ? supplyReq.computeEmergencyScore() : -1e9;
        double rescueScore = hasRescue ? rescueReq.computeEmergencyScore() : -1e9;

        Request* chosenReq = nullptr;
        RequestQueue* chosenQueue = nullptr;

        // Correct emergency decision engine
        if (hasSupply && !hasRescue) {
            chosenReq = &supplyReq;
            chosenQueue = &supplyQueue;
        }
        else if (!hasSupply && hasRescue) {
            chosenReq = &rescueReq;
            chosenQueue = &rescueQueue;
        }
        else {
            // Both available → compare scores
            if (rescueScore > supplyScore) {
                chosenReq = &rescueReq;
                chosenQueue = &rescueQueue;
            }
            else if (supplyScore > rescueScore) {
                chosenReq = &supplyReq;
                chosenQueue = &supplyQueue;
            }
            else {
                // Equal scores → RESCUE takes priority
                chosenReq = &rescueReq;
                chosenQueue = &rescueQueue;
            }
        }

        // Try to assign request
        if (!t.tryAssignRequest(*chosenReq)) {
            std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
            t.rollbackMission(supplyQueue, rescueQueue);
            break;
        }

        // Successful → remove from queue
        Request dummy;
        chosenQueue->dequeue(dummy);
    }

    // After mission assignment or rollback → summary
    const MissionStack& ms = t.getMissionStack();
    int finalCount = ms.size();

    if (finalCount > 0) {
        // We must count supply/rescue types inside the mission stack
        int sup = 0, res = 0;

        // Access MissionStack internals
        Request* arr = ms.getData();
        int top = ms.getTopIndex();

        for (int i = 0; i <= top; i++) {
            if (arr[i].getType() == "SUPPLY")
                sup++;
            else
                res++;
        }

        int totalWorkload = t.getCurrentWorkload();

        std::cout << "Team " << teamId << " assigned "
                  << finalCount << " requests ("
                  << sup << " SUPPLY, "
                  << res << " RESCUE), total workload "
                  << totalWorkload << "." << std::endl;
    }

    return true;
}


bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

bool QuakeAssistController::handleInitTeams(int numTeams) {
    return initializeTeams(numTeams);
}

void QuakeAssistController::printQueues() const {
    std::cout << "SUPPLY QUEUE:" << std::endl;
    Request req;

    for (int i = 0; i < supplyQueue.size(); i++) {
        if (supplyQueue.getAt(i, req)) {
            std::cout << req.toString() << std::endl;
        }
    }

    std::cout << "RESCUE QUEUE:" << std::endl;

    for (int i = 0; i < rescueQueue.size(); i++) {
        if (rescueQueue.getAt(i, req)) {
            std::cout << req.toString() << std::endl;
        }
    }
}


void QuakeAssistController::printTeam(int teamId) const {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        return;
    }
    
    std::cout << "TEAM " << teamId << " STACK:" << std::endl;
    teams[idx].printMissionStack();
}


void QuakeAssistController::clear() {
    //Clear data.
    supplyQueue.clear();
    rescueQueue.clear();

    for (int i = 0; i < teamCount; i++) {
        teams[i].clearMission();
    }
    std::cout << "System cleared." << std::endl;
}
