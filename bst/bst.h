#include <string>
#include <functional>

class BST
{
public:
    using KeyType = int;
    using ItemType = std::string;

    // Wrapper functions
    ItemType* lookup(KeyType); // Return pointer to item in dictionary. 
    void insert(KeyType, ItemType);
    void remove(KeyType);
    void displayEntries(); // Traversing through nodes.

    void displayTree();
    void removeIf(std::function<bool(KeyType)>); // Higher order function to delete multiple nodes.

    BST(const BST&); // Copy constructor
    BST& operator=(const BST&); // Copy assignment operator
    BST(BST &&); // Move constructor
    BST& operator=(BST&&); // Move assignment operator
    BST() = default;
    ~BST();

private:
    struct Node;
    Node* root = leaf();

    // Leaf nodes return null pointer.
    static Node* leaf();
    static bool isLeaf(Node*);

    //Worker functions
    ItemType* lookupRec(KeyType, Node*);
    void insertRec(KeyType, ItemType, Node*&);
    void displayEntriesRec(Node*);
    void removeRec(KeyType, Node*&);
    //Node* detatchMinimumNode(Node*&);
    Node* findMinimumNode(Node*&); // Alternative method to detatch node.

    void deepDelete(Node*);
    Node* deepCopy(Node*);

    void rotateRight(Node*&);
    void rotateLeft(Node*&);
};
