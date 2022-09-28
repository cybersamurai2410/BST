#include "bst.h"
#include <iostream>
#include <cassert>
#include <stack>

// Recursive data structure as Node points to other Node instances.
struct BST::Node
{
    KeyType key;
    ItemType item;
    Node* leftChild;
    Node* rightChild;

    Node(KeyType k, ItemType i) { key = k; item = i; }
};

BST::Node* BST::leaf()
{
    return nullptr;
}

bool BST::isLeaf(Node* n)
{
    return (n == nullptr);
}

// --- WEEK 2 ---

BST::ItemType* BST::lookup(KeyType soughtKey)
{
    return lookupRec(soughtKey, root);
}

BST::ItemType* BST::lookupRec(KeyType soughtKey, Node* currentNode)
{
    if (isLeaf(currentNode)) return NULL;

    if (soughtKey < currentNode->key) {
        lookupRec(soughtKey, currentNode->leftChild);
    }
    else if (soughtKey > currentNode->key) {
        lookupRec(soughtKey, currentNode->rightChild);
    }
    else {

        ItemType* item = &(currentNode->item);
        return item;
    }
}

void BST::insert(KeyType key, ItemType item)
{

    insertRec(key, item, root);
}

void BST::insertRec(KeyType k, ItemType i, Node*& current)
{
    if (current == NULL)
    {
        current = new Node(k, i);
        current->leftChild = leaf();
        current->rightChild = leaf();
    }

    else if (k > current->key)
    {
        insertRec(k, i, current->rightChild);
    }
    else if (k < current->key)
    {
        insertRec(k, i, current->leftChild);
    }
    else
    {
        // Overwrite existing item
        current->item = i;
    }
}

// --- WEEK 3 ---

void BST::displayEntries()
{
    displayEntriesRec(root);
}

void BST::displayEntriesRec(Node* currentNode)
{
    if (currentNode == NULL) return;

    // Pre-order traversal
    // std::cout<<currentNode->key<<" "<<currentNode->item<<std::endl;

    displayEntriesRec(currentNode->leftChild);

    // In-order traversal
    std::cout << currentNode->key << " " << currentNode->item << std::endl;

    displayEntriesRec(currentNode->rightChild);

    // Post-order traversal
    // std::cout<<currentNode->key<<" "<<currentNode->item<<std::endl;
}

void BST::displayTree()
{
    std::stack<Node*> stack;
    Node* currentNode = root;

    while (!stack.empty() || currentNode != nullptr)
    {
        if (currentNode != nullptr)
        {
            stack.push(currentNode);
            currentNode = currentNode->leftChild;
        }
        else {
            currentNode = stack.top();
            stack.pop();
            std::cout << currentNode->key << " " << currentNode->item << std::endl;
            currentNode = currentNode->rightChild;
        }
    }
}

void BST::remove(KeyType key)
{
    removeRec(key, root);
}

void BST::removeRec(KeyType key, Node*& node)
{
    if (node == NULL) return;
    else if (key < node->key) {
        removeRec(key, node->leftChild);
    }
    else if (key > node->key) {
        removeRec(key, node->rightChild);
    }
    else // !move to seperate function for removeIf
    {
        // Both children are leaves
        if (isLeaf(node->leftChild) && isLeaf(node->rightChild)) {
            delete node;
            node = leaf();
        }
        // One internal node and one leaf as children
        else if (isLeaf(node->leftChild)) {
            Node* temp = node->rightChild;
            delete node;
            node = temp;
        }
        else if (isLeaf(node->rightChild)) {
            Node* temp = node->leftChild;
            delete node;
            node = temp;
        }
        // Both internal nodes as children
        else {
            Node* temp = findMinimumNode(node->rightChild);
            node->key = temp->key;
            node->item = temp->item;
            removeRec(node->key, node->rightChild);
        }
    }
}

BST::Node* BST::findMinimumNode(Node*& node)
{
    // Locate minimum node in right subtree
    Node* min_node = node;
    while (min_node && !isLeaf(min_node->leftChild))
    {
        min_node = min_node->leftChild;
    }

    return min_node;
}

void BST::removeIf(std::function<bool(KeyType)> key_condition)
{
    // Taverse tree -> remove node if true

    std::stack<Node*> stack;
    Node* currentNode = root;

    std::stack<Node*> node_stack;
   
    while (!stack.empty() || currentNode != nullptr)
    {
        if (currentNode != nullptr)
        {
            node_stack.push(currentNode);

            stack.push(currentNode);
            currentNode = currentNode->leftChild;
        }
        else {
            currentNode = stack.top();
            stack.pop();

            currentNode = currentNode->rightChild;
        }
    }

    while (!node_stack.empty())
    {
        currentNode = node_stack.top();
        node_stack.pop();

        if (key_condition(currentNode->key)) {
            remove(currentNode->key);
        }
    }
    
    // !split removeRec

    // removeIf([](KeyType k) {return false; });
}

// --- WEEK 4 ---

BST::~BST()
{
    deepDelete(root);

    // Warning! Do not mix deep destructor with shallow copying
}

void BST::deepDelete(Node* current)
{
    if (current == NULL) return;

    // Delete subtrees
    deepDelete(current->leftChild);
    deepDelete(current->rightChild);

    // Delete node
    delete current;
}

BST::BST(const BST& bstToCopy)
{
    this->root = deepCopy(bstToCopy.root);
}


BST::Node* BST::deepCopy(Node* nodeToCopy)
{
    if (nodeToCopy == NULL) return NULL;
    Node* node = new Node(nodeToCopy->key, nodeToCopy->item);
    node->leftChild = deepCopy(nodeToCopy->leftChild);
    node->rightChild = deepCopy(nodeToCopy->rightChild);

    return node;
}

// --- WEEK 5 ---

BST& BST::operator=(const BST& bstToCopy)
{    
    /*
        t1 = t2
        t1 = *this
        t2 = &bstToCopy
    */

    if (this != &bstToCopy) {
        deepDelete(this->root); // deep delete from t1
        this->root = deepCopy(bstToCopy.root); // deep copy from t2 and set root of t1 to copy
    }
    
    return *this;
}

BST::BST(BST&& bstToMove) // && rvalue reference
{
    // Steal nodes from 'bstToMove'
    
    this->root = bstToMove.root;
    bstToMove.root = leaf();
    
}

BST& BST::operator=(BST&& bstToMove)
{
    if (this != &bstToMove) {
        deepDelete(this->root); 
        this->root = bstToMove.root; 
        bstToMove.root = leaf();
    }
    
    return *this;
}

// --- WEEK 6 ---

void BST::rotateRight(Node*& localRoot)
{
    Node* b = localRoot;
    assert(!isLeaf(b));
    Node* a = b->leftChild;
    assert(!isLeaf(a));
    Node* alpha = a->leftChild;
    Node* beta = a->rightChild;
    Node* gamma = b->rightChild;

    localRoot = a;
    a->rightChild = b;
    b->leftChild = beta;
}

void BST::rotateLeft(Node*& localRoot)
{
    Node* a = localRoot;
    assert(!isLeaf(a));
    Node* b = a->rightChild;
    assert(!isLeaf(b));
    Node* alpha = a->leftChild;
    Node* beta = b->leftChild;
    Node* gamma = b->rightChild;

    localRoot = b;
    b->leftChild = a;
    a->rightChild = beta;
}

// Parametrically Polymorphic Binary Search Tree in different branch

// --- WEEK 7 ---

// AVL tree in different branch
