#include <vector> 
#ifndef BPlusTree
#define BPlusTree
class Node {
public:
    bool leaf;
    vector<int> keys;
    vector<Node*> children;

    Node(bool l = true) : leaf(l), keys(), children() {}
};
    class BPlusTree {
    private:
        int t;
        Node* root = new Node;

        void splitChild(Node* parent, int idx);
        void insertNonFull(Node* node, int key);
        void Delete(Node* node, int key);
        void deleteInternalNode(Node* node, int key, int idx);
        void mergeNodes(Node* parent, int idx);

    public:
        BPlusTree(int t) {
            this->t = t
        };
        bool search(int key);
        void insert(int key);
        void remove(int key);
    };
    BPlusTree(int t) : t(t) {}
	bool search(int key) {
		Node* current = root;
		while (!current->leaf) {
			auto it = lower_bound(current->keys.begin(), current->keys.end(), key);
			int pos = distance(current->keys.begin(), it);
			current = current->children[pos];
		}
		auto found = find(current->keys.begin(), current->keys.end(), key);
		return (found != current->keys.end()) ? true : false;
	}
	void insert(int key) {
		if (root->keys.size() == 2 * t - 1) {
			Node* new_root = new Node(false);
			new_root->children.push_back(root);
			this->root = new_root;
			splitChild(new_root, 0);
			insertNonFull(this->root, key);
		}
		else {
			insertNonFull(this->root, key);
		}
	}
	void remove(int key) {
		Delete(root, key);
	}
#endif