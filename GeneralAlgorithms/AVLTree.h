#pragma once

#include "BinaryTree.h"

template<typename Key, typename Value>
class AVLTree : public BinaryTree<Key, Value>
{
protected:
    using Node = BinaryTree<Key, Value>::Node;

    unsigned char GetBalanceFactor(Node* node)
    {
        return BinaryTree<Key, Value>::GetHeight(node->right) - BinaryTree<Key, Value>::GetHeight(node->left);
    }

    void FixHeight(Node* node)
    {
        unsigned char leftHeight = BinaryTree<Key, Value>::GetHeight(node->left);
        unsigned char rightHeight = BinaryTree<Key, Value>::GetHeight(node->right);
        node->height = std::max(leftHeight, rightHeight) + 1;
    }

    Node* RotateRight(Node* node)
    {
        Node* leftNode = node->left;
        node->left = leftNode->right;
        leftNode->right = node;
        FixHeight(node);
        FixHeight(leftNode);
        return leftNode;
    }

    Node* RotateLeft(Node* node)
    {
        Node* rightNode = node->right;
        node->right = rightNode->left;
        rightNode->left = node;
        FixHeight(node);
        FixHeight(rightNode);
        return rightNode;
    }

    virtual Node* Balance(Node* node) override
    {
        Node* result = node;
        FixHeight(node);
        if(GetBalanceFactor(node) == 2)
        {
            if(GetBalanceFactor(node->right) < 0)
                node->right = RotateRight(node->right);
            result = RotateLeft(node);
        }
        else if(GetBalanceFactor(node) == -2)
        {
            if(GetBalanceFactor(node->left) > 0)
                node->left = RotateLeft(node->left);
            result = RotateRight(node);
        }

        return result;
    }
};