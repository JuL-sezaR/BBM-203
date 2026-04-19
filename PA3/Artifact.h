#ifndef ARTIFACT_H
#define ARTIFACT_H

#include <string>

struct Artifact
{
    int artifactID;              // key in AVL tree
    std::string name;            // artifact name (no spaces)
    int rarityLevel;             // [1..10]
    int researchValue;           // arbitrary integer score
    std::string assignedToName;  // empty string ("") if unassigned, otherwise researcher name
    int usageCount;            // number of times assigned

    Artifact();
    Artifact(int id, const std::string &n, int rarity, int value);

    void updateValueBasedOnUsage(); 
};



#endif // ARTIFACT_H
