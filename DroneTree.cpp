// DroneTree.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: Sophia Rahman and Nafisa Patwary
//
// Write every operation recursively using the private helpers (Section 6).
// The tree does NOT own the units: delete tree nodes only, never node->unit.

#include "DroneTree.h"
#include <iostream>
using namespace std;

DroneTree::DroneTree() : root(nullptr), count(0) {}

DroneTree::~DroneTree() { destroy(root); }

void DroneTree::destroy(TreeNode* node)
{
    // TODO: delete every node (post-order)
    if(node == nullptr){
        return;
    }

    destroy(node->left);
    destroy(node->right);
    delete node;
}

bool DroneTree::insert(DeliveryUnit* u)
{
    if (insertHelper(root, u)) { count++; return true; }
    return false;
}

bool DroneTree::insertHelper(TreeNode*& node, DeliveryUnit* u)
{
    // TODO: smaller IDs left, larger right; false on a duplicate
    if (node == nullptr)
    {
        node = new TreeNode;
        node->unit = u;
        node->left = nullptr;
        node->right = nullptr;
        return true;
    }

    int id = u->getID();
    int nodeID = node->unit->getID();

    if (id == nodeID)
    {
        return false;
    }
    if (id < nodeID)
    {
        return insertHelper(node->left, u);
    }
    return insertHelper(node->right, u);
}

DeliveryUnit* DroneTree::search(int id, int& comparisons) const
{
    comparisons = 0;
    return searchHelper(root, id, comparisons);
}

DeliveryUnit* DroneTree::searchHelper(TreeNode* node, int id, int& comparisons) const
{
    // TODO: one comparison per node examined
     if (node == nullptr)
    {
        return nullptr;
    }

    comparisons++;
    int nodeID = node->unit->getID();

    if (id == nodeID)
    {
        return node->unit;
    }
    if (id < nodeID)
    {
        return searchHelper(node->left, id, comparisons);
    }
    return searchHelper(node->right, id, comparisons);
}

bool DroneTree::remove(int id)
{
    if (removeHelper(root, id)) { count--; return true; }
    return false;
}

bool DroneTree::removeHelper(TreeNode*& node, int id)
{
    // TODO: three cases; two children -> in-order successor
    if (node == nullptr)
    {
        return false;
    }

    int nodeID = node->unit->getID();

    if (id < nodeID)
    {
        return removeHelper(node->left, id);
    }
    if (id > nodeID)
    {
        return removeHelper(node->right, id);
    }

    //only right child
    if (node->left == nullptr)
    {
        TreeNode* temp = node;
        node = node->right;
        delete temp;
        return true;
    }

    //only left child
    if (node->right == nullptr)
    {
        TreeNode* temp = node;
        node = node->left;
        delete temp;
        return true;
    }

    //two children -> in-order successor
    TreeNode* successor = node->right;
    while (successor->left != nullptr)
    {
        successor = successor->left;
    }
    node->unit = successor->unit;
    return removeHelper(node->right, successor->unit->getID());
}

void DroneTree::inOrder() const { inOrderHelper(root); cout << endl; }
void DroneTree::preOrder() const { preOrderHelper(root); cout << endl; }
void DroneTree::postOrder() const { postOrderHelper(root); cout << endl; }

void DroneTree::inOrderHelper(TreeNode* node) const {
    if (node == nullptr)
    {
        return;
    }

    inOrderHelper(node->left);             //left
    cout << node->unit->getID() << " ";    //node
    inOrderHelper(node->right);            //right
}
void DroneTree::preOrderHelper(TreeNode* node) const {
    if (node == nullptr)
    {
        return;
    }

    cout << node->unit->getID() << " ";    //node
    inOrderHelper(node->left);             //left
    inOrderHelper(node->right);            //right
}
void DroneTree::postOrderHelper(TreeNode* node) const {
    if (node == nullptr)
    {
        return;
    }

    inOrderHelper(node->left);             //left
    inOrderHelper(node->right);            //right
    cout << node->unit->getID() << " ";    //node
}

int DroneTree::height() const { return heightHelper(root); }

int DroneTree::heightHelper(TreeNode* node) const
{
    // TODO: empty tree 0, one node 1
    if (node == nullptr)
    {
        return 0;
    }

    int leftHeight = heightHelper(node->left);
    int rightHeight = heightHelper(node->right);

    if (leftHeight > rightHeight)
    {
        return 1 + leftHeight;
    }
    return 1 + rightHeight;

    return 0;
}

int DroneTree::size() const { return count; }
