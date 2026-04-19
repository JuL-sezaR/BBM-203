#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "Researcher.h"
#include <iostream>

enum Color
{
    RED,
    BLACK
};

struct ResearcherNode
{
    Researcher data;
    Color color;
    ResearcherNode *left;
    ResearcherNode *right;
    ResearcherNode *parent;

    ResearcherNode(const Researcher &r)
        : data(r),
          color(RED),
          left(nullptr),
          right(nullptr),
          parent(nullptr)
    {
    }
};

class RedBlackTree
{
public:
    RedBlackTree();
    ~RedBlackTree();

    RedBlackTree(const RedBlackTree &) = delete;
    RedBlackTree &operator=(const RedBlackTree &) = delete;

    // Insert researcher by fullName.
    // Returns true if inserted, false if a researcher with the same name already exists.
    bool insertResearcher(const Researcher &researcher);

    // Remove researcher by fullName.
    // Returns true if removed, false if not found.
    bool removeResearcher(const std::string &fullName);

    // Find researcher node by name. Returns nullptr if not found.
    ResearcherNode *findResearcher(const std::string &fullName) const;

    // Pre-order traversal used for PRINT_STATS (sorted by RBT structure, key = fullName).
    void traversePreOrderForStats() const;
    void transplant(ResearcherNode *u, ResearcherNode *v);

    // Aggregate stats.
    int getResearcherCount() const;
    int getTotalLoad() const;

    // Delete all nodes.
    void clear();

private:
    ResearcherNode *root;

    void clear(ResearcherNode *node);

    ResearcherNode *find(ResearcherNode *node, const std::string &fullName) const;

    // BST insertion by name (ignores colors).
    ResearcherNode *bstInsert(ResearcherNode *root, ResearcherNode *node, bool &inserted);

    void insertFixup(ResearcherNode *node);
    void deleteFixup(ResearcherNode *node, ResearcherNode *parent);

    void rotateLeft(ResearcherNode *x);
    void rotateRight(ResearcherNode *y);

    // Internal helpers.

    ResearcherNode *minimum(ResearcherNode *node) const;

    int getResearcherCount(ResearcherNode *node) const;
    int getTotalLoad(ResearcherNode *node) const;

    void traversePreOrderForStats(ResearcherNode *node) const;
};

#endif // REDBLACKTREE_H
