#ifndef TEAM_H
#define TEAM_H

#include "MissionStack.h"
#include "RequestQueue.h"

class Team {
public:
    Team();
    Team(int id, int maxLoad);

    int getId() const;
    int getMaxLoadCapacity() const;
    int getCurrentWorkload() const;

    void setId(int id);
    void setMaxLoadCapacity(int maxLoad);

    bool hasActiveMission() const;

    // Called when assigning a request to this team.
    // Returns false if adding this request would exceed capacity.
    // In that case, the caller should trigger rollback.
    // TODO: Implement in Team.cpp.
    bool tryAssignRequest(const Request& req);

    // Rollback: move all requests from missionStack back to
    // their original queues (SUPPLY or RESCUE).
    // The relative order for each type should be preserved.
    // TODO: Implement in Team.cpp.
    void rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue);

    // Called when a mission is successfully completed and dispatched.
    // Clears mission stack and resets workload.
    void clearMission();

    // For printing:
    const MissionStack& getMissionStack() const;
    void printMissionStack() const;

private:
    int teamID;
    int maxLoadCapacity;
    int currentWorkload;
    MissionStack missionStack;
};

#endif // TEAM_H
