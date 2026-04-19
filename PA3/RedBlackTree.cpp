#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:
    if (node == nullptr) return;

    clear(node->left);
    clear(node->right);
    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name
    if (!node) return nullptr;
    if (fullName == node->data.fullName) return node;
    if (fullName < node->data.fullName) {
        return find(node->left, fullName);
    }
    return find(node->right, fullName);
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise
    bool inserted = false;//tracks whether operation is succesful
    ResearcherNode* newNode = new ResearcherNode(researcher);
    root = bstInsert(root, newNode, inserted);
    if (inserted) {
        insertFixup(newNode);
        return true;
    }
    delete newNode;
    return false;//temporary, you may delete this
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).
    if (!current) {
        inserted = true;
        node->parent = nullptr;
        return node;
    }

    if (node->data.fullName < current->data.fullName) {
        current->left = bstInsert(current->left, node, inserted);
        if (inserted && current->left == node) {
            node->parent = current;
        }
    } else if (node->data.fullName > current->data.fullName) {
        current->right = bstInsert(current->right, node, inserted);
        if (inserted && current->right == node) {
            node->parent = current;
        }
    } else {
        inserted = false;
        return current; //duplicate names
    }
    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.
    while (node->parent && node->parent->color == Color::RED) {
        if (node->parent == node->parent->parent->left) {
            ResearcherNode* uncle = node->parent->parent->right;
            if (uncle && uncle->color == Color::RED) {
                uncle->color = Color::BLACK;
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateRight(node->parent->parent);
            }
        } else {
            ResearcherNode* uncle = node->parent->parent->left;
            if (uncle && uncle->color == Color::RED) {
                uncle->color = Color::BLACK;
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateLeft(node->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.
    ResearcherNode* z = findResearcher(fullName);
    if (!z) return false;

    Color originalColor = z->color;
    ResearcherNode* x;

    if (!z->left) {
        x = z ->right;
        transplant(z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(z, z->left);
    } else {
        ResearcherNode* y = minimum(z->right);
        originalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (originalColor == Color::BLACK) {
        deleteFixup(x, x ? x->parent : nullptr);
    }

    return true;
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    // TODO: Standard Red-Black deletion fixup.
    while (node != root && (node == nullptr || node->color == Color::BLACK)) {
        if (!parent) break;

        if (node == parent->left) {
            ResearcherNode* sibling = parent->right;
            if (sibling == nullptr) break;
            if (sibling->color == Color::RED) {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                rotateLeft(parent);
                sibling = parent->right;
                if (sibling == nullptr) break;
            }
            if ((sibling->left == nullptr || sibling->left->color == Color::BLACK) &&
                (sibling->right == nullptr || sibling->right->color == Color::BLACK)) {
                sibling->color = Color::RED;
                node = parent;
                parent = node->parent;
            } else {
                if (sibling->right == nullptr || sibling->right->color == Color::BLACK) {
                    sibling->left->color = Color::BLACK;
                    sibling->color = Color::RED;
                    rotateRight(sibling);
                    sibling = parent->right;
                    if (!sibling) break;
                }
                sibling->color = parent->color;
                parent->color = Color::BLACK;
                sibling->right->color = Color::BLACK;
                rotateLeft(parent);
                node = root;
            }
        } else {
            // Symmetric right case
            ResearcherNode* sibling = parent->left;
            if (!sibling) break;
            if (sibling->color == Color::RED) {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                rotateRight(parent);
                sibling = parent->left;
                if (!sibling) break;
            }
            if ((sibling->right == nullptr || sibling->right->color == Color::BLACK) &&
                (sibling->left == nullptr || sibling->left->color == Color::BLACK)) {
                sibling->color = Color::RED;
                node = parent;
                parent = node->parent;
            } else {
                if (sibling->left == nullptr || sibling->left->color == Color::BLACK) {
                    sibling->right->color = Color::BLACK;
                    sibling->color = Color::RED;
                    rotateLeft(sibling);
                    sibling = parent->left;
                    if (!sibling) break;
                }
                sibling->color = parent->color;
                parent->color = Color::BLACK;
                sibling->left->color = Color::BLACK;
                rotateRight(parent);
                node = root;
            }
        }
    }
    if (node) node->color = Color::BLACK;
}



ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.
    ResearcherNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.
    ResearcherNode* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void RedBlackTree::transplant(ResearcherNode *u, ResearcherNode *v)
{
    if (!u->parent) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) v->parent = u->parent;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    if (!node) return 0;

    return 1 + getResearcherCount(node->left) + getResearcherCount(node->right);
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    if (!node) return 0;
    return node->data.numAssigned + getTotalLoad(node->left) + getTotalLoad(node->right);
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.
    if (!node) return;

    std::cout << node->data.fullName << " " << node->data.capacity << " " << node->data.numAssigned << std::endl;

    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);
}
