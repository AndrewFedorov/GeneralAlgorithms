#pragma once

#include <optional>
#include <algorithm>
#include <functional>

enum Color { RED, BLACK };

template<typename Key, typename Value>
class BinaryTree
{
protected:
    struct Node
    {
        struct Data
        {
            Key key;
            Value value;
        };

        Data data;
        Node* left;
        Node* right;
        unsigned char height;
        bool color;

        Node(Key key, const Value& value)
        {
            data.key = key;
            data.value = value;
            left = nullptr;
            right = nullptr;
            height = 1;
            color = RED;
        }
    };

    Node* root;

    static unsigned char GetHeight(Node* node)
    {
        return node ? node->height : 0;
    }

    Node* FindNode(Node* node, Key key) const
    {
        Node* result = nullptr;
        if(!node)
            return nullptr;

        if(key == node->data.key)
            result = node;
        else if(key < node->data.key)
            result = FindNode(node->left, key);
        else
            result = FindNode(node->right, key);

        return result;
    }

    void DestroyNode(Node* node)
    {
        if(node)
        {
            DestroyNode(node->left);
            DestroyNode(node->right);
            delete node;
        }
    }

    virtual Node* Balance(Node* node)
    {
        return node;
    }

    Node* InsertNode(Node* node, Key key, const Value& value)
    {
        if(!node)
            node = new Node(key, value);
        else if(key == node->data.key)
            node->data.value = value;
        else if(key < node->data.key)
            node->left = InsertNode(node->left, key, value);
        else if(key > node->data.key)
            node->right = InsertNode(node->right, key, value);

        return Balance(node);
    }

    Node* FindMostLeft(Node* node)
    {
        return node->left ? FindMostLeft(node->left) : node;
    }

    Node* RemoveMostLeft(Node* node)
    {
        if(!node->left)
            return node->right;

        node->left = RemoveMostLeft(node->left);
        return Balance(node);
    }

    Node* RemoveNode(Node* node, Key key)
    {
        if(!node)
            return nullptr;

        if(key < node->data.key)
            node->left = RemoveNode(node->left, key);
        else if(key > node->data.key)
            node->right = RemoveNode(node->right, key);
        else
        {
            Node* leftNode = node->left;
            Node* rightNode = node->right;
            delete node;

            if(!rightNode)
                return leftNode;

            Node* mostLeft = FindMostLeft(rightNode);
            mostLeft->right = RemoveMostLeft(rightNode);
            mostLeft->left = leftNode;
            return Balance(mostLeft);
        }
 
        return Balance(node);
    }

    void InOrderTraverseNode(Node* node, std::function<void(Key, const Value&)> callback)
    {
        if(!node)
            return;

        InOrderTraverseNode(node->left, callback);
        callback(node->data.key, node->data.value);
        InOrderTraverseNode(node->right, callback);
    }

    void PreOrderTraverseNode(Node* node, std::function<void(Key, const Value&)> callback)
    {
        if(!node)
            return;

        callback(node->data.key, node->data.value);
        PreOrderTraverseNode(node->left, callback);
        PreOrderTraverseNode(node->right, callback);
    }

    void PostOrderTraverseNode(Node* node, std::function<void(Key, const Value&)> callback)
    {
        if(!node)
            return;

        PostOrderTraverseNode(node->left, callback);
        PostOrderTraverseNode(node->right, callback);
        callback(node->data.key, node->data.value);
    }

public:
    BinaryTree()
    {
        root = nullptr;
    }

    BinaryTree(Key key, Value value)
    {
        root = new Node(key, value);
    }

    ~BinaryTree()
    {
        DestroyNode(root);
    }

    std::optional<Value> Find(Key key) const
    {
        std::optional<Value> value;
        Node* node = FindNode(root, key);
        if(node)
            value = node->data.value;

        return value;
    }

    void Insert(Key key, const Value& value)
    {
        root = InsertNode(root, key, value);
    }

    void Remove(Key key)
    {
        root = RemoveNode(root, key);
    }

    void InOrderTraverse(std::function<void(Key, const Value&)> callback)
    {
        InOrderTraverseNode(root, callback);
    }

    void PreOrderTraverse(std::function<void(Key, const Value&)> callback)
    {
        PreOrderTraverseNode(root, callback);
    }

    void PostOrderTraverse(std::function<void(Key, const Value&)> callback)
    {
        PostOrderTraverseNode(root, callback);
    }
};