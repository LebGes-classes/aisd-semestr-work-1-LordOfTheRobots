#include <iostream>
#include <vector>
#include <algorithm>
#include "BPlusTree.h"
using namespace std;

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

	void splitChild(Node* parent, int index) {
		Node* child = parent->children[index];
		Node* new_child = new Node();
		new_child->leaf = child->leaf;
		parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);
		parent->children.insert(parent->children.begin() + index + 1, new_child);
		new_child->keys.assign(child->keys.begin() + t, child->keys.end());
		child->keys.resize(t - 1);
		if (!child->leaf) {
			new_child->children.assign(child->children.begin() + t, child->children.end());
			child->children.resize(t);
		}
	}
	void insertNonFull(Node* node, int key) {
		if (node->leaf) {
			node->keys.push_back(key);
			sort(node->keys.begin(), node->keys.end());
		}
		else {
			int idx = upper_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();
			Node* child = node->children[idx];
			if (child->keys.size() == 2 * t - 1) {
				splitChild(node, idx);
				if (key > node->keys[idx]) ++idx;
			}
			insertNonFull(node->children[idx], key);
		}
	}
	void deleteInternalNode(Node* node, int key, int idx) {
		Node* leftChild = node->children[idx];
		Node* rightChild = node->children[idx + 1];

		if (leftChild->keys.size() >= t) {
			int predecessorKey = leftChild->keys.back();
			Delete(leftChild, predecessorKey);
			node->keys[idx] = predecessorKey;
		}
		else if (rightChild->keys.size() >= t) {
			int successorKey = rightChild->keys.front();
			Delete(rightChild, successorKey);
			node->keys[idx] = successorKey;
		}
		else {
			mergeNodes(node, idx);
		}
	}
	void mergeNodes(Node* parent, int idx) {
		Node* leftChild = parent->children[idx];
		Node* rightChild = parent->children[idx + 1];

		leftChild->keys.push_back(parent->keys[idx]);
		leftChild->keys.insert(end(leftChild->keys), begin(rightChild->keys), end(rightChild->keys));
		if (!rightChild->leaf) {
			leftChild->children.insert(end(leftChild->children), begin(rightChild->children), end(rightChild->children));
		}
		parent->keys.erase(begin(parent->keys) + idx);
		parent->children.erase(begin(parent->children) + idx + 1);
		delete rightChild;
		if (parent->keys.empty()) {
			this->root = leftChild;
			delete parent;
		}
	}
	void Delete(Node* node, int key) {
		if (node->leaf) {
			auto found = find(node->keys.begin(), node->keys.end(), key);
			if (found != node->keys.end()) node->keys.erase(found);
		}
		else {
			int idx = lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();
			if (idx < node->keys.size() && node->keys[idx] == key) {
				deleteInternalNode(node, key, idx);
			}
			else {
				Delete(node->children[idx], key);
			}
		}
	}

public:
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
};