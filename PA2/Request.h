#ifndef REQUEST_H
#define REQUEST_H

#include <string>

class Request {
public:
    Request();
    // Constructor for SUPPLY requests
    Request(const std::string& id,
            const std::string& city,
            const std::string& supplyType,
            int amount,
            int emergencyLevel);
    // Constructor for RESCUE requests
    Request(const std::string& id,
            const std::string& city,
            int numPeople,
            const std::string& risk,
            int emergencyLevel);

    // "SUPPLY" or "RESCUE"
    const std::string& getType() const;
    const std::string& getId() const;
    const std::string& getCity() const;
    int getEmergencyLevel() const;
    std::string toString() const;

    // Supply-specific getters (valid only if type == "SUPPLY")
    const std::string& getSupplyType() const;
    int getAmount() const;

    // Rescue-specific getters (valid only if type == "RESCUE")
    int getNumPeople() const;
    const std::string& getRisk() const;

    // Computes emergency score according to the assignment description.
    // (Students implement this based on the PDF.)
    int computeEmergencyScore() const;

    // Computes workload contribution for this request according to the rules.
    // (Students implement this based on the PDF.)
    int computeWorkloadContribution() const;

private:
    std::string id;
    std::string type;       // "SUPPLY" or "RESCUE"
    std::string city;

    // Supply fields
    std::string supplyType; // "WATER", "FOOD", ...
    int amount;

    // Rescue fields
    int numPeople;
    std::string risk;       // "LOW", "MEDIUM", "HIGH"
    int emergencyLevel;

    // Helper: LOW=1, MEDIUM=2, HIGH=3 (students can use this idea if they want)
    int getRiskMultiplier() const;
};

#endif // REQUEST_H
