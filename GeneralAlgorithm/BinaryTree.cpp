#include "BinaryTree.h"
#include <exception>

namespace my
{
	template<typename Key, typename Value>
	BinaryTree<Key, Value>::Node::Node(Key key, Value value)
	{
		data.key = key;
		data.value = value;
	}

	template<typename Key, typename Value>
	bool BinaryTree<Key, Value>::search(const Node* node, Key key, Node* node_out) const
	{
		if (node)
		{
			if (key == node->data.key)
			{
				node_out = node;
				return true;
			}

			if (key < node->data.key)
				return search(node->left, key, node_out);
			else
				return search(node->right, key, node_out);
		}

		return false;
	}

	template<typename Key, typename Value>
	Value BinaryTree<Key, Value>::Find(Key key) const
	{
		Node* node;
		if (search(root, key, node))
			return node->data.value;
		else
			throw std::exception("Not found");
	}

	template<typename Key, typename Value>
	void BinaryTree<Key, Value>::Insert(Key key, Value value)
	{
		Node* node;
		if (search(root, key, node))
			node->data.value = value;
		else
		{
			if (node)
			{
				if (key < node->data.key)
					node->left = new Node(key, value);
				else
					node->right = new Node(key, value);
			}
			else
				node = new Node(key, value);
		}
	}

	template<typename Key, typename Value>
	void BinaryTree<Key, Value>::Remove(Key key)
	{
		Node* node;
		if (search(root, key, node))
		{
			if(node->left)
				node = node->left;
			if (node->right)
				node->parent = node->right;
		}
	}

	template<typename Key, typename Value>
	BinaryTree<Key, Value>::~BinaryTree()
	{
		delete root;
	}
} // namespace my