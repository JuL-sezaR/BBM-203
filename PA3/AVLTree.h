#ifndef AVLTREE_H
#define AVLTREE_H

#include "Artifact.h"
#include <iostream>

struct ArtifactNode
{
    Artifact data;
    ArtifactNode *left;
    ArtifactNode *right;
    int height;

    ArtifactNode(const Artifact &a)
        : data(a), left(nullptr), right(nullptr), height(0)
    {
    }
};

class AVLTree
{
public:
    AVLTree();
    ~AVLTree();

    AVLTree(const AVLTree &) = delete;
    AVLTree &operator=(const AVLTree &) = delete;

    // Insert a new artifact by ID.
    // Returns true if inserted, false if an artifact with the same ID already exists.
    bool insertArtifact(const Artifact &artifact);

    // Remove artifact by ID.
    // Returns true if removed, false if not found.
    bool removeArtifact(int artifactID);

    // Find artifact node by ID. Returns nullptr if not found.
    ArtifactNode *findArtifact(int artifactID) const;

    // Print ONLY unassigned artifacts (assignedToName empty),
    // in ascending artifactID order.
    void printUnassigned() const;
    void matchRarity(int minRarity) const;

    // Set assignedToName for a given artifact (if exists).
    void setAssignedTo(int artifactID, const std::string &researcherName);

    // Clear assignedToName for a given artifact (if exists).
    void clearAssignedTo(int artifactID);

    // Return number of artifacts in the tree.
    int getArtifactCount() const;

    // Return total sum of rarityLevel for all artifacts.
    int getTotalRarity() const;

    // Post-order traversal used for PRINT_STATS (students decide exact print format).
    void traversePostOrderForStats() const;

    // Delete all nodes.
    void clear();

private:
    ArtifactNode *root;

    void clear(ArtifactNode *node);

    ArtifactNode *insert(ArtifactNode *node, const Artifact &artifact, bool &inserted);
    ArtifactNode *remove(ArtifactNode *node, int artifactID, bool &removed);

    ArtifactNode *find(ArtifactNode *node, int artifactID) const;

    int height(ArtifactNode *node) const;
    int getBalance(ArtifactNode *node) const;

    ArtifactNode *rotateLeft(ArtifactNode *x);
    ArtifactNode *rotateRight(ArtifactNode *y);

    ArtifactNode *findMinNode(ArtifactNode *node) const;

    void printUnassigned(ArtifactNode *node) const;
    void matchRarity(ArtifactNode *node, int minRarity) const;

    int getArtifactCount(ArtifactNode *node) const;
    int getTotalRarity(ArtifactNode *node) const;

    void traversePostOrderForStats(ArtifactNode *node) const;
};

#endif // AVLTREE_H
