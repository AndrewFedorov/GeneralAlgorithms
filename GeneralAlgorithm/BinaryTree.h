#ifndef MY_BINARY_TREE
#define MY_BINARY_TREE

namespace my
{
	template<typename Key, typename Value>
	class BinaryTree
	{
	private:

		struct Node
		{
		private:
			struct Data
			{
			private:
				Key key;
				Value value;
			};
			Data data;
			Node* left;
			Node* right;

			Node(Key key, Value value);
		};
		Node* root;
		bool search(const Node* node, Key key, Node* node_out) const;
	public:
		Value Find(Key key) const;
		void Insert(Key key, Value value);
		void Remove(Key key);
		~BinaryTree();
	};
} // namespace my

#endif // MY_BINARY_TREE