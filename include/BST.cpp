#include "BST.h"
#include <iostream>

void BST::insertNode(BSTNode*& node, const Doctor& d) {
    if (!node) {
        node = new BSTNode(d);
        return;
    }
    if (d.id < node->data.id)
        insertNode(node->left, d);
    else
        insertNode(node->right, d);
}

void BST::insert(const Doctor& d) {
    insertNode(root, d);
}

void BST::inorder(BSTNode* node, std::vector<Doctor>& doctors) const {
    if (!node) return;
    inorder(node->left, doctors);
    doctors.push_back(node->data);
    inorder(node->right, doctors);
}

std::vector<Doctor> BST::getAllDoctors() const {
    std::vector<Doctor> doctors;
    inorder(root, doctors);
    return doctors;
}

void BST::deleteTree(BSTNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}
