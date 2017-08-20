#ifndef BST_H_
#define BST_H_

#include <iostream>
#include <cstddef>
#include <string>
#include <assert.h>
#include <iomanip>
#include "Product.h"

using namespace std;

class BST {
private:
	bool primary;
    struct Node {
    	Product* data;
        Node* left;
        Node* right;

        Node(Product* newdata):data(newdata), left(NULL), right(NULL) {}
    };

    typedef struct Node* NodePtr;
    NodePtr root;

    /*Private helper functions*/
    void insertData(NodePtr root, Product* value);
    //private helper function for insert
    //recursively inserts a value into the BST

    void inOrderPrint(ostream& out, NodePtr root) const;
    //private helper function for inOrderPrint
    //recursively prints tree values in order from smallest to largest

    void preOrderPrint(ostream& out, NodePtr root) const;
    //private helper function for preOrderPrint
    //recursively prints tree values in pre order

    void postOrderPrint(ostream& out, NodePtr root) const;
    //private helper function for postOrderPrint
    //recursively prints tree values in post order

    void makeCopy(NodePtr copy);
    //recursive helper function to the copy constructor

    void freeNode(NodePtr root);
    //private helper function for the destructor
    //recursively frees the memory in the BST

    void searchString(char field, string value, NodePtr root, List<Product*>* out) const;
    //private helper function for searchS

    void searchNum(char field, double low, double high, NodePtr root, List<Product*>* out) const;
    //private helper function for searchN

    void inOrderToFile(ofstream& of, NodePtr root) const;
    //private helper function for inOrderToFile

    void inOrderReleaseData(NodePtr root);
    //private helper function for inOrderReleaseData

    Product* searchS(NodePtr root, Product* value) const;
    //recursive helper function to search data by given string

    Product* search(NodePtr root, Product* value) const;
    //recursive helper function to search
    //returns whether the value is found in the tree

    Product* minimum(NodePtr root) const;
    //recursive helper function to minimum
    //returns the minimum value in the tree

    Product* maximum(NodePtr root) const;
    //recursive helper function to maximum
    //returns the maximum value in the tree

    NodePtr deleteData(NodePtr root, Product* value);
    //recursive helper function to removeData
    //removes value from the tree

    void size(NodePtr root, int& size) const;
    //recursive helper function to the size function
    //calculates the size of the tree
    //stores the result in size

    int height(NodePtr root) const;
    //recursive helper function to the height function
    //returns the height of the tree

public:

    List<Product*> browse();

    /*constructors and destructor*/
    BST(bool type) {
    	this->primary = type;
    	root = NULL; };
    //Instantiates a new BST

    BST(const BST &bst);
    //copy constructor

    ~BST();
    //deallocates the tree memory

    /*access functions*/
    Product* minimum() const;
    //returns the minimum value in the BST
    //pre: !empty()

    Product* maximum() const;
    //returns the maximum value in the BST and returns it
    //pre: !empty()

    bool empty() const;
    //determines whether the BST is empty

    int size() const;
    //returns the size of the tree

    Product* getRoot() const;
    //returns the value stored at the root of the BST
    //pre: !empty()

    int height() const;
    //returns the height of the tree

    List<Product*> searchString(char field, string value) const;
    //search product by given string

    List<Product*> searchNum(char field, double low, double high) const;
    //search product by given number

    Product* search(Product* value) const;
    //pre: !empty()

    Product* searchS(Product* value) const;
    //pre: !empty()

    /*manipulation procedures*/
    void insertData(Product* value);
    //inserts a new value into the BST

    void removeData(Product* value);
    //removes the specified value from the tree
    //pre: value is located in the tree

    /*additional functions*/
    void inOrderPrint(ostream& out = cout) const;
    //calls the inOrderPrint function to print out the values
    //stored in the BST

    void preOrderPrint(ostream& out = cout) const;
    //calls the reOrderPrint function to print out the values
    //stored in the BST

    void postOrderPrint(ostream& out = cout) const;
    //calls the postOrderPrint function to print out the values
    //stored in the BST

    void inOrderToFile(ofstream& of) const;
    //write into output file

    void inOrderReleaseData();
    //release the data

    void addProduct();
    //add a product into BST

    int compare(Product* one, Product* two) const{
    	if(primary)
    		return (*one == *two) ? 0 : (*one > *two) ? 1 : -1;
		return (one->getName() == two->getName()) ? 0 : (one->getName() > two->getName()) ? 1 : -1;
    }
};

void BST::searchString(char field, string value, NodePtr root, List<Product*>* out) const {
    if( root ){
    	searchString(field, value, root->left, out);
		if (root->data->getFieldS(field) == value)
			out->insertEnd(root->data);
		searchString(field, value, root->right, out);
    }
}

List<Product*> BST::searchString(char field, string value) const {
	List<Product*> p;
	searchString(field, value, root, &p);
	return p;
}

void BST::searchNum(char field, double low, double high, NodePtr root, List<Product*>* out) const {
    if( root ){
    	searchNum(field, low, high, root->left, out);
		if (root->data->getFieldN(field) >= low && root->data->getFieldN(field) <= high)
			out->insertEnd(root->data);
		searchNum(field, low, high, root->right, out);
    }
}

List<Product*> BST::browse() {
	List<Product*> p;
	int low = 5;
	searchNum('r', low, 5.0, root, &p);
	while (p.getLength() < 3 && low >= 0){
		low--;
		searchNum('r', low, 5.0, root, &p);
	}
	return p;
}

List<Product*> BST::searchNum(char field, double low, double high) const {
	List<Product*> p;
	searchNum(field, low, high, root, &p);
	return p;
}

void BST::insertData(NodePtr root, Product* value) {
    if (compare(value, root->data) == -1) {
        if(root->left)
            insertData(root->left, value);
        else
            root->left = new Node(value);
    }
    else if (compare(value, root->data) == 1){
        if(root->right)
            insertData(root->right, value);
        else
            root->right = new Node(value);
    }else
        cout << "Error: Product already exists" << endl; // data already exists
}

void BST::inOrderPrint(ostream& out, NodePtr root) const{
    if (root) {
        inOrderPrint(out, root->left);
        out << *root->data << endl;
        inOrderPrint(out, root->right);
    }
}

void BST::inOrderToFile(ofstream& of, NodePtr root) const {
	if (root) {
		inOrderToFile(of, root->left);
        root->data->toFile(of);
        inOrderToFile(of, root->right);
    }
}

void BST::inOrderReleaseData(NodePtr root){
	if (root) {
		inOrderReleaseData(root->left);
        delete root->data;
        root->data = NULL;
        inOrderReleaseData(root->right);
    }
}

void BST::preOrderPrint(ostream& out, NodePtr root) const{
    if (root) {
        out << *root->data << endl;
        preOrderPrint(out, root->left);
        preOrderPrint(out, root->right);
    }
}

void BST::postOrderPrint(ostream& out, NodePtr root) const{
    if (root){
        postOrderPrint(out, root->left);
        postOrderPrint(out, root->right);
        out << *root->data <<endl;
    }
}

void BST::makeCopy(NodePtr copy) {
    if (copy == NULL)
        return;
    insertData(copy->data);
    makeCopy(copy->left);
    makeCopy(copy->right);
}

void BST::freeNode(NodePtr root) {
    if (root) {
        freeNode(root->left);
        freeNode(root->right);
        delete root;
    }
}

Product* BST::search(NodePtr root, Product* value) const{
    if(!root) {
    	return NULL;
    }
    if (compare(value, root->data) == 1){
        return search(root->right, value);
    }
    else if (compare(value, root->data) == -1) {
        return search(root->left, value);
    }
    else{
        return root->data;
    }
}

Product* BST::searchS(NodePtr root, Product* value) const{
    if(!root) {
    	return NULL;
    }
    if (value->getName() > root->data->getName()){
        return search(root->right, value);
    }
    else if (value->getName() < root->data->getName()) {
        return search(root->left, value);
    }
    else{
        return root->data;
    }
}

Product* BST::minimum(NodePtr root) const{
    if(!root->left)
        return root->data;
    return minimum(root->left);
}

Product* BST::maximum(NodePtr root) const{
    if(!root->right)
        return root->data;
    return maximum(root->right);
}

BST::NodePtr BST::deleteData(NodePtr root, Product* value) {
    if(!root) {
        return root;
    }
    else if (compare(value, root->data) == 1) root->right = deleteData(root->right, value);
    else if (compare(value, root->data) == -1) root->left = deleteData(root->left, value);
    else {
        if(!root->left && !root->right){
            delete root;
            root = NULL;
        }
        else if(!root->left) {
            NodePtr tmp= root;
            root = root->right;
            delete tmp;
        }
        else if(!root->right) {
            NodePtr tmp= root;
            root = root->left;
            delete tmp;
        }
        else {
            root->data = minimum(root->right);
            root->right = deleteData(root->right, root->data);
        }
    }
    return root;
}

void BST::size(NodePtr root, int& sz) const {
    if (root == NULL)
        return;
    size(root->left, sz);
    size(root->right, sz);
    sz++;
}

int BST::height(NodePtr root) const {
    if(!root)
        return 0;
    int left = height(root->left);
    int right = height(root->right);
    return (left > right) ? left+1 : right+1;
}

BST::BST(const BST &bst) {
    root = NULL;
    makeCopy(bst.root);
}

BST::~BST() {
    freeNode(root);
}

Product* BST::minimum() const {
    assert(root);
    return minimum(root);
}

Product* BST::maximum() const {
    assert(root);
    return maximum(root);
}

bool BST::empty() const {
    return !root;
}

int BST::size() const {
    int sz = 0;
    size(root, sz);
    return sz;
}

Product* BST::getRoot() const {
    assert(root);
    return root->data;
}

int BST::height() const {
    assert(root);
    return height(root) - 1;
}

Product* BST::search(Product* value) const {
    return search(root, value);
}

void BST::insertData(Product* value) {
    if (root == NULL)
        root = new Node(value);
    else
        insertData(root, value);
}

void BST::removeData(Product* value) {
    assert(search(value));
    root = deleteData(root, value);
}

void BST::inOrderPrint(ostream& out) const{
    inOrderPrint(out, root);
    out << "\n";
}

void BST::inOrderToFile(ofstream& of) const {
    inOrderToFile(of, root);
}

void BST::inOrderReleaseData() {
	inOrderReleaseData(root);
}

void BST::preOrderPrint(ostream& out) const{
    preOrderPrint(out, root);
    out << "\n";
}

void BST::postOrderPrint(ostream& out) const{
    postOrderPrint(out, root);
    out << "\n";
}

#endif /* BST_H_ */
