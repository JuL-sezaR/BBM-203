#ifndef QUAKE_ASSIST_CONTROLLER_H
#define QUAKE_ASSIST_CONTROLLER_H

#include <string>
#include "Team.h"
#include "RequestQueue.h"

class QuakeAssistController {
public:
    QuakeAssistController();
    ~QuakeAssistController();

    // Disable copying
    QuakeAssistController(const QuakeAssistController&) = delete;
    QuakeAssistController& operator=(const QuakeAssistController&) = delete;

    // Initialize teams with fixed count.
    // Capacities will be provided from input file using SET_TEAM_CAPACITY.
    bool initializeTeams(int numTeams);

    // Parse a single line from the input and execute the command.
    // Returns false when the command is CLEAR and system should stop,
    // or when an unrecoverable error occurs.
    // TODO: Implement in QuakeAssistController.cpp.
    bool parseAndExecute(const std::string& line);

    // Print current queues (for PRINT_QUEUES command).
    void printQueues() const;

    // Print a single team (for PRINT_TEAM <id> command).
    void printTeam(int teamId) const;

    // Clear everything (queues and teams' missions).
    void clear();

private:
    Team* teams;
    int teamCount;

    RequestQueue supplyQueue;
    RequestQueue rescueQueue;

    // Helper to find team index by id, returns -1 if not found.
    int findTeamIndexById(int teamId) const;

    // Command handlers (to be implemented by you).
    bool handleAddSupply(const std::string& id,
                         const std::string& cityStr,
                         const std::string& supplyTypeStr,
                         int amount,
                         int emergencyLevel);

    bool handleAddRescue(const std::string& id,
                         const std::string& cityStr,
                         int numPeople,
                         const std::string& riskStr,
                         int emergencyLevel);

    bool handleRemoveRequest(const std::string& id);

    bool handleHandleEmergency(int teamId, int k);

    bool handleDispatchTeam(int teamId);

    // New: initialize number of teams from input
    bool handleInitTeams(int numTeams);

    // New: capacity configuration from input
    bool handleSetTeamCapacity(int teamId, int capacity);
};

#endif // QUAKE_ASSIST_CONTROLLER_H
