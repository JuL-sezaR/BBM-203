#ifndef RESEARCHER_H
#define RESEARCHER_H

#include <string>

struct Researcher
{
    std::string fullName;   // key in Red-Black Tree
    int capacity;           // max number of artifacts they can supervise
    int *assignedArtifacts; // dynamic array of artifact IDs
    int numAssigned;        // how many are currently used

    Researcher();
    Researcher(const Researcher &other);
    Researcher(const std::string &name, int cap);
    Researcher& operator=(const Researcher &other);
    ~Researcher();

    // Try to add an artifact to this researcher's assignment list.
    // Returns true on success, false if capacity is full or artifact already assigned to them.
    bool addArtifact(int artifactID);

    // Remove an artifact from this researcher's assignment list.
    // Returns true if artifact was found and removed, false otherwise.
    bool removeArtifact(int artifactID);

    // Remove all artifacts from this researcher (but does NOT update the AVL tree).
    void removeAllArtifacts();

    // Returns true if this researcher supervises the given artifact ID.
    bool hasArtifact(int artifactID) const;
};

#endif // RESEARCHER_H
