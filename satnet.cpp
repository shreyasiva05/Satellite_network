
#include "satnet.h"
/*
File: satnet.cpp
Author: Shreya Sivakumar

Description:  This program is a startup company that plans to create a large network of satellites
*/
//Default constructor
// Name: Default Constructor
// Desc: Setting to default values
// Preconditions: the values weren't initialized
// Postconditions: the values were set
SatNet::SatNet(){
    m_root = nullptr;
}
//Destructor
// Name: Destructor
// Desc: deallocating the memory
// Preconditions: the values weren't deallocated
// Postconditions: the values were deallocated
SatNet::~SatNet(){
    clear();
}
//Insert(const Sat &satellite)
// Name: Insert function
// Desc: allocating the memory and inserting the nodes
// Preconditions: the nodes werent inserted
// Postconditions: the nodes were inserted
void SatNet::insert(const Sat &satellite) {

    if ((satellite.getID() >= MINID)&&(satellite.getID() <= MAXID)){
        m_root = insert1(satellite, m_root); //inserts the nodes based on the AVL tree balancing
        m_root = balancedTree(m_root);//calls the balance tree after making changes
    }

}
//Inserthelper
// Name: insert1(const Sat& element, Sat * satNode)
// Desc: allocating the memory and inserting the nodes
// Preconditions: the nodes werent inserted
// Postconditions: the nodes were inserted
Sat* SatNet::insert1(const Sat& element, Sat * satNode) {

    if (satNode == nullptr) { //checking if the node is pointing to nullptr

        satNode= new Sat(element); //creating a new node
        satNode = satNode; //setting the satNode to temp
        satNode->m_height = 0;
        satNode->m_left = nullptr; //setting the left and right to nullptr
        satNode->m_right= nullptr;
        return satNode; //returns the satNode
    }
    if (element.getID() < satNode->getID()) {//go to left
        satNode->m_left = insert1(element, satNode->m_left);
        updateHeight(satNode);

    } else if (element.getID() > satNode->getID()) {
        satNode->m_right = insert1(element, satNode->m_right);
        updateHeight(satNode);
    }
    else if(element.getID() == satNode->getID()){
        return satNode;
    }
    updateHeight(satNode);
    return balancedTree(satNode); //returns the node to a balanceed tree function
}
Sat *SatNet::balancedTree(Sat* aNode){
    int balance = getBalance(aNode);
    // Balancing the AVL tree

    if (balance > 1) {
        if (getBalance(aNode->m_left) < 0) {
            aNode->setLeft(leftrotation(aNode->m_left));//calls the leftrotation
        }
            return rightrotation(aNode);
    }
    if(balance <-1){
        if (getBalance(aNode->m_right) > 0) {//if the balance factor of the right is greater than 0
            aNode->setRight((rightrotation(aNode->m_right)));//calls the rght roation
        }
            return leftrotation(aNode);//callse the left rotation based based on the condtion
    }
    return aNode;//returns the node at the end after making changes
}

int SatNet::max( int a, int b){
    if (a>b){
        return a;
    }else
        return b;
}
//getBalance(Sat *aNode)
// Name: GetBalancde
// Desc: Balancing the AVL tree
// Preconditions: The Avl tree hasn't been balanced
// Postconditions: returns the different of the height of anodes left and right
int SatNet::getBalance(Sat *aNode){
    if (aNode== nullptr)
        return 0;
    return height(aNode->m_left) - height(aNode->m_right);//returns the balance factor
}

//rightrotation(Sat* y)
// Name: Right Rotation
// Desc: Right rotation in AVL trees to balance the tree
// Preconditions: The Avl tree hasn't been balanced
// Postconditions: returns the parent of the AVL tree

Sat * SatNet::rightrotation(Sat* aNode){
    Sat *parent = aNode->m_left;
    Sat *rightchild = parent->m_right;
    parent->m_right = aNode; //setting anode to the parent's right
    aNode->m_left = rightchild; //setting the right child to aNode's left

    aNode->m_height = max(height(aNode->m_left),height(aNode->m_right) + 1);//updating the height
    parent->m_height = max(height(parent->m_left),height(parent->m_right) + 1);
    return parent;//returns the parent after making changes
}
//leftrotation(Sat* parent)
// Name: Left Rotation
// Desc: Left rotation in AVL trees to balance the tree
// Preconditions: The Avl tree hasn't been balanced
// Postconditions: returns the  right most child of the AVL tree
Sat * SatNet::leftrotation(Sat* parent){
    Sat *rightchild = parent->m_right;
    Sat *leftchild = rightchild->m_left;
    rightchild->m_left = parent; //rotations, setting parent to rightchild;s left
    parent->m_right = leftchild ;//rotations, setting parent to rightchild;s left
    parent->m_height = max(height(parent->m_left ),height(parent->m_right) + 1); //update the height
    rightchild->m_height = max(height(rightchild->m_left),height(rightchild->m_right) + 1);
    // Return new root
    return rightchild;//returs the right child after update the changes and the height
}
//height(Sat* node)
// Name: height
// Desc: returns the height of the node
// Preconditions: The node in the parameters
// Postconditions: returns the height of the node
int SatNet::height(Sat* node) {
    if (node == nullptr) {
        return -1;
    }
    return node->m_height;//returs the node's height
}

//clear()
// Name: clear
// Desc: clears the nodes
// Preconditions: the nodes hasvent been cleared
// Postconditions: dealllocates the nodes

void SatNet::clear(){
    clearhelper(m_root);
}
//clearhelper(Sat* clear)
// Name: clear
// Desc: clears the nodes
// Preconditions: the nodes hasvent been cleared
// Postconditions: dealllocates the nodes
void SatNet::clearhelper(Sat* clear){
    if (clear!=nullptr){
        clearhelper(clear->m_left);//calls the clear to clear the node is the m_left
        clearhelper(clear->m_right);//calls the clear to clear the node is the m_right
        delete clear;//returns after clearing

    }
}
//remove(int id)
// Name: remove
// Desc: rmeoves the speciifc node that has the id
// Preconditions: the specific node hasvent been cleared
// Postconditions: dealllocates the specific node
void SatNet::remove(int id){
    m_root=removehelper(m_root,id);

}
//removehelper(Sat* mroot, int id)
// Name: remove
// Desc: rmeoves the speciifc node that has the id
// Preconditions: the specific node hasvent been cleared
// Postconditions: dealllocates the specific node
Sat* SatNet::removehelper(Sat* mroot, int id){
    Sat* temp;
    if(mroot == nullptr)// item not found; do nothing
        return nullptr;
    else if(id < mroot->getID())//traverse to left
        mroot->m_left = removehelper(mroot->m_left, id);//traverse until we find
    else if(id > mroot->getID())//traverse to right
        mroot->m_right = removehelper(mroot->m_right, id);
    else if(mroot->m_left != nullptr && mroot->m_right != nullptr){//two children
        temp = updateMin(mroot->m_right);//lowest value
        mroot->m_id = temp->m_id;//copy the value
        // now delete the found node
        mroot->m_right = removehelper(mroot->m_right, temp->getID());
    }
    //if there are no children or one child
    else{
        // also, in the case of two children
        temp = mroot;
        if(mroot->m_left == nullptr)
            mroot = mroot->m_right;
        else if(mroot->m_right == nullptr)
            mroot = mroot->m_left;
        delete temp;
    }

    mroot = balancedTree(mroot);
    if(mroot!=nullptr){
        updateHeight(mroot);
    }
    return mroot;
}
//updateMin(Sat* leftnode)
// Name: remove
// Desc: removes the speciifc node that has the id
// Preconditions: the specific node hasvent been cleared
// Postconditions: dealllocates the specific node
void SatNet::updateHeight(Sat* aNode){
    if(aNode->m_left == nullptr && aNode->m_right == nullptr){
        aNode->m_height=0; //update the height if both is nullptr
    }
    else if(aNode->m_left != nullptr && aNode->m_right == nullptr){
        aNode->m_height=aNode->m_left->m_height +1; //update the height if m_left is not nullptr
    }
    else if(aNode->m_left == nullptr && aNode->m_right != nullptr){
        aNode->m_height=aNode->m_right->m_height +1; //update the height if m_right is not nullptr
    }
    else if(aNode->m_left != nullptr && aNode->m_right != nullptr){
        aNode->m_height = max(aNode->m_left->m_height, aNode->m_right->m_height)+1;
    }//update if both nullptr
}
Sat * SatNet::updateMin(Sat* leftnode){
    Sat* current = leftnode;

    /* loop down to find the leftmost leaf */
    while (current->m_left != nullptr)
        current = current->m_left;

    return current;
}


void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}
//listSatellites() const
// Name: listSatellites
// Desc: lists the satellites that exixts
// Preconditions: the parameters passes the node
// Postconditions: the node iterates and prints each satellite
void SatNet::listSatellites() const {
    listSatelliteshelper(m_root);
}
//listSatelliteshelper(Sat* printnode) const
// Name: listSatelliteshelper
// Desc: lists the satellites that exixts
// Preconditions: the parameters passes the node
// Postconditions: the node iterates and prints each satellite
void SatNet::listSatelliteshelper(Sat* printnode) const {
    if (printnode !=nullptr) {
        listSatelliteshelper(printnode->m_left);//calls the function recursively to list the left nodes of the tree
        // Print the information
        cout << printnode->getID()<<" "<<printnode->getStateStr()<<" "<<printnode->getInclinStr()<<" "<<printnode->getAltStr()<<endl;
        listSatelliteshelper(printnode->m_right);//calls the function recursively to list the right nodes of the tree
    }
}
//setState(int id, STATE state)
// Name: setState
// Desc: lists the satellites that exixts
// Preconditions: passes the node without being removed
// Postconditions: returns the tree after sets the state for each node
bool SatNet::setState(int id, STATE state){
    return setStateRecursive(m_root, id, state);
}

bool SatNet::setStateRecursive(Sat* aNode, int id, STATE state) {
    if (aNode == nullptr) {
        return false;//not found return false
    }
    if (id < aNode->getID()) { //if is is less than the aNode-s id
        return setStateRecursive(aNode->m_left, id, state);//if ID is less , search to the left
    } else if (id > aNode->getID()) {

        return setStateRecursive(aNode->m_right, id, state);//if ID is greater , search to the right
    } else {
        aNode->setState(state);//retursn the id
        return true;//returns true
    }
}
//removeDeorbited()
// Name: removeDeorbited
// Desc: rmeoves the speciifc node that has the id
// Preconditions: the specific node hasvent been cleared
// Postconditions: dealllocates the specific node
void SatNet::removeDeorbited(){
    m_root = removeDeorbitedRecursive(m_root);
}
//removeDeorbitedRecursive(Sat* aNode
// Name: removeDeorbitedRecursive
// Desc: rmeoves the speciifc node that has the id
// Preconditions: the specific node hasvent been cleared
// Postconditions: dealllocates the specific node
Sat *SatNet::removeDeorbitedRecursive(Sat* aNode) {
    if(aNode == nullptr){
        return nullptr;
    }//Recursively calls the remove function to remove to check each node's state
    aNode->m_left = removeDeorbitedRecursive(aNode->m_left); //calls the function to go to the left
    aNode->m_right = removeDeorbitedRecursive(aNode->m_right);//calls the function to go to the right

    if(aNode->getState()==DEORBITED){//checks if the state is Deoorbited
        int nodeid = aNode->m_id;
        aNode =  removehelper(aNode,nodeid);//calls the remove helper to remove if the sate is Deorbited
    }
    return aNode;//returns the node at the end


}
//findSatellitehelper(Sat* node, int id) const
// Name: findSatellitehelper
// Desc: finds the satellite uding the id if the id matches
// Preconditions: passesm_root and id
// Postconditions: returns boolean if it is found or not
bool SatNet::findSatellitehelper(Sat* node, int idnum) const {
    if (node == nullptr) {
        // If the node is nullptr, the satellite is not found.
        return false;
    }
    if (idnum < node->getID()) {
        return findSatellitehelper(node->m_left, idnum);// If the ID to find is less than node's ID
    } else if (idnum > node->getID()) {
        // If the ID to find is greater than the current node's ID, search in the right subtree.
        return findSatellitehelper(node->m_right, idnum);// If the ID to find is right than node's ID
    } else {
        // The ID matches the current node's ID, so the satellite is found.
        return true;
    }
}
//findSatellite(int id) const
// Name: findSatellite
// Desc: finds the satellite uding the id if the id matches
// Preconditions: passesm_root and id
// Postconditions: returns boolean if it is found or not
bool SatNet::findSatellite(int id) const {
    return findSatellitehelper(m_root,id);
}
//operator=(const SatNet & rhs)
// Name: Assignment operator
// Desc: compares the rhs and lhs objects whether they are eqaully allocated
// Preconditions: paramaters rhs to compare with lhs object
// Postconditions: returns this based on the function
const SatNet & SatNet::operator=(const SatNet & rhs){
    if(this!=&rhs){
        clearhelper(m_root); //calls the clear helper to clear the exisiting nodes
        m_root = operatorhelper(rhs.m_root); //calls the assignment operator recursively to check the m_root
    }
    return *this; //returns this after clearing

}
//operatorhelper(Sat * aNode)
// Name: Assignment operator
// Desc: compares the rhs and lhs objects whether they are eqaully allocated
// Preconditions: paramaters rhs to compare with lhs object
// Postconditions: returns this based on the function
Sat *SatNet::operatorhelper(Sat * aNode) {
    if (aNode == nullptr){
        return nullptr;//return nullptr if its empty
    }
    Sat *newNode  = new Sat(aNode->getID(),aNode->getAlt(),aNode->getInclin(),aNode->getState());
    //copy the aNode's left to newNode's left
    newNode->m_left = operatorhelper(aNode->m_left);
    newNode->m_right = operatorhelper(aNode->m_right); //copy the aNode's right to newNode's right
    return newNode; //return the newnode after copying

}
//countSatellites(INCLIN degree)
// Name: countSatellites
// Desc: Counts the number of satellites that exsists in INLCINE degree
// Preconditions: paramaters passes INCLIN degree
// Postconditions: returns the numnber of satellites
int SatNet::countSatellites(INCLIN degree) const{
   return countSatelliteshelper(m_root,degree); //calls the helper funciton to count the satellites

}
//countSatelliteshelper(Sat* aNode, INCLIN degree) const
// Name: countSatelliteshelper
// Desc: Counts the number of satellites that exsists in INLCINE degree
// Preconditions: paramaters passes INCLIN degree
// Postconditions: returns the numnber of satellites
int SatNet::countSatelliteshelper(Sat* aNode, INCLIN degree) const{
    if(aNode ==nullptr)
        return 0;
    int countdegree = 0;
    if (aNode->getInclin()==degree)
        countdegree++;
    //recursively ocunts the satellite by goingto the right and then left
    countdegree += countSatelliteshelper(aNode->m_right,degree);
    countdegree += countSatelliteshelper(aNode->m_left,degree);
    return countdegree;
}


