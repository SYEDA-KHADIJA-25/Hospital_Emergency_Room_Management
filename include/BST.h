#ifndef BST_H
#define BST_H

#include <string>
#include <vector>
#include "Doctor.h"

struct BSTNode {
    Doctor data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Doctor& d) : data(d), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    void insertNode(BSTNode*& node, const Doctor& d);
    void inorder(BSTNode* node, std::vector<Doctor>& doctors) const;
    void deleteTree(BSTNode* node);

public:
    BST() : root(nullptr) {}
    ~BST() { deleteTree(root); }

    void insert(const Doctor& d);
    std::vector<Doctor> getAllDoctors() const;
};

#endif
