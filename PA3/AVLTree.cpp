#include "AVLTree.h"

AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
//TODO
    if (node == nullptr) return;
    
    clear(node->left);
    clear(node->right);
    delete node;
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root
    ArtifactNode* y = x->right;
    x->right = y->left;
    y->left = x;

    // Height updats
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).
    ArtifactNode* x = y->left;
    y->left = x->right;
    x->right = y;

    // Height updates
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.
    while(node && node->left){
        node = node->left;
    }

    return node;
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.
    if (!node){
        inserted = true;
        return new ArtifactNode(artifact);
    }

    if (artifact.artifactID < node->data.artifactID){
        node->left = insert(node->left, artifact, inserted);

    } else if (artifact.artifactID > node->data.artifactID){
        node->right = insert(node->right, artifact, inserted);

    } else {
        inserted = false;
        return node; //duplicate IDs
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balance = getBalance(node);
    // Left Left Case
    if (balance > 1 && artifact.artifactID < node->left->data.artifactID)
        return rotateRight(node);
    // Right Right Case
    if (balance < -1 && artifact.artifactID > node->right->data.artifactID)
        return rotateLeft(node);
    // Left Right Case
    if (balance > 1 && artifact.artifactID > node->left->data.artifactID)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // Right Left Case
    if (balance < -1 && artifact.artifactID < node->right->data.artifactID)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.

    if (!node){
        removed = false;
        return node;
    }

    if (artifactID < node->data.artifactID){
        node->left = remove(node->left, artifactID, removed);
    } else if (artifactID > node->data.artifactID){
        node->right = remove(node->right, artifactID, removed);
    } else {
        // Node with only one child or no child
        removed = true; 
        if (!node->left || !node->right){
            ArtifactNode* temp = node->left ? node->left : node->right;

            // No child case
            if (!temp){
                temp = node;
                node = nullptr;
            } else { // One child case
                *node = *temp; // Copy the contents of the non-empty child
            }
            delete temp;
        } else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            ArtifactNode* temp = findMinNode(node->right);
            node->data = temp->data; // Copy the inorder successor's data to this node
            node->right = remove(node->right, temp->data.artifactID, removed); // Delete the inorder successor
        }
    }

    if (!node) return node;

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);
    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);
    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;//tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;//tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO
    if (!node) return nullptr;
    if (artifactID == node->data.artifactID) return node;
    if (artifactID < node->data.artifactID) {
        return find(node->left, artifactID);
    }
    return find(node->right, artifactID);

}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{
    //assign artifact to researcher
    ArtifactNode* node = findArtifact(artifactID);
    if (node) {
        node->data.assignedToName = researcherName;
    }

}

void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO
    ArtifactNode* node = findArtifact(artifactID);
    if (node) {
        node->data.assignedToName = "";
    }
}

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>
    if (!node) return;

    printUnassigned(node->left);
    if (node->data.assignedToName.empty()) {
        std::cout << node->data.artifactID << " "
                    << node->data.name << " "
                    << node->data.rarityLevel << " "
                    << node->data.researchValue << std::endl;
    }
    printUnassigned(node->right);

}
void AVLTree::matchRarity(int minRarity) const
{
    matchRarity(root, minRarity);
}

void AVLTree::matchRarity(ArtifactNode* node, int minRarity) const
{
    if (!node) return;

    matchRarity(node->left, minRarity);
    if (node->data.rarityLevel >= minRarity) {
        std::cout << node->data.artifactID << " "
                << node->data.name << " "
                << node->data.rarityLevel << " "
                << node->data.researchValue;
                if (!node->data.assignedToName.empty()) {
                    std::cout<< " ASSIGNED:" << node->data.assignedToName;
                } else {
                    std::cout << " UNASSIGNED";
                }
                std::cout << std::endl;
    }
    matchRarity(node->right, minRarity);
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    // TODO: return size of subtree.
    if (!node) return 0;
    return 1 + getArtifactCount(node->left) + getArtifactCount(node->right);
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    // TODO: sum of rarityLevel over subtree.
    if (!node) return 0;
    return node->data.rarityLevel + getTotalRarity(node->left) + getTotalRarity(node->right);
}

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.
    if (!node) return;

    traversePostOrderForStats(node->left);
    traversePostOrderForStats(node->right);
    std::cout << node->data.artifactID << " "
              << node->data.name << " "
              << node->data.rarityLevel << " "
              << node->data.researchValue;
    if (!node->data.assignedToName.empty()) {
        std::cout << " " << node->data.assignedToName;
    } else {
        std::cout << " UNASSIGNED";
    }
    std::cout << std::endl;
}
