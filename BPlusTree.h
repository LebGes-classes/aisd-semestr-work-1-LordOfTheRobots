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
#endif
