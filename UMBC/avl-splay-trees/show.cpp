// UMBC - CMSC 341 - Fall 2026 - Proj2
#include "show.h"

Group::Group(){ 
    this->setNullPtrs();
}
Group::Group(int group, Drone drones[], int population){
    this->setNullPtrs();

    for (int i = 0; i < population; i++) {
        this->insert(drones[i]);
    }

    this->m_groupID = group;
}
Group::~Group(){
    this->deallocateRec(this->m_root);
}
void Group::insert(const Drone& drone){
    if (drone.getID() < MINID || drone.getID() > MAXID)
        return;
    else {
        this->m_root = insertRec(this->m_root, drone.getID(), drone.getType(), drone.getState());
    }
}

void Group::clear(){
    this->deallocateRec(this->m_root);
    this->setNullPtrs();
}

void Group::remove(int id){
    this->m_root = removeRec(this->m_root, id);
}

void Group::listDrones() const {
    listDronesRec(this->m_root);
}

bool Group::setState(int id, STATE state){
    Drone *node = find(this->m_root, id);
    if (node != nullptr) {
        node->setState(state);
        return true;
    }

    return false;
}

void Group::removeDead(){
    removeDeadRec(this->m_root);
}

bool Group::findDrone(int id) const {
    return (find(this->m_root, id) != nullptr);
}

int Group::count(LIGHTCOLOR color) const{
    return countColor(this->m_root, color);
}

int Group::count(STATE state) const{
    return countState(this->m_root, state);
}

void Group::dumpTree() const {dump(m_root);}
void Group::dump(Drone* aDrone) const{
    if (aDrone != nullptr){
        cout << "(";
        dump(aDrone->m_left);//first visit the left child
        cout << aDrone->m_id << ":" << aDrone->m_height;//second visit the node itself
        dump(aDrone->m_right);//third visit the right child
        cout << ")";
    }
}

    // ***************************************************
    // ********** Group class helper functions ***********
    // ***************************************************

void Group::deallocateRec(Drone *node) {
    if (node == nullptr)
        return;

    deallocateRec(node->m_left);
    deallocateRec(node->m_right);
    delete node;
}

void Group::setNullPtrs() {
    this->m_root = nullptr;
    this->m_left = nullptr;
    this->m_right = nullptr;
}

Drone* Group::insertRec(Drone* &current, int id, LIGHTCOLOR color, STATE state) {
    if (current == nullptr) {
        return new Drone(id, color, state);
    } else if (current->getID() == id)
        return current;
        
    if (current->getID() > id) 
        current->m_left = insertRec(current->m_left, id, color, state);
    else if (current->getID() < id)
        current->m_right = insertRec(current->m_right, id, color, state);

    resetHeight(current);
    int balance = findBalance(current);

    if (balance > 1 && id < current->m_left->getID()) { // left+left
        return rightRotate(current);
    } else if (balance > 1 && id > current->m_left->getID()) { // left+right
        current->m_left = leftRotate(current->m_left);
        return rightRotate(current);
    } else if (balance < -1 && id > current->m_right->getID()) { // right+right
        return leftRotate(current);
    } else if (balance < -1 && id < current->m_right->getID()) { // right+left
        current->m_right = rightRotate(current->m_right);
        return leftRotate(current);
    }

    return current;
}

int Group::findHeight(Drone* node) {
    if (node == nullptr)
        return -1;
    else
        return node->getHeight();
}

void Group::resetHeight(Drone* node) {
    node->setHeight(1 + max(findHeight(node->m_left), findHeight(node->m_right)));
}

int Group::findBalance(Drone* node) {
    return (findHeight(node->getLeft()) - findHeight(node->getRight()));
}

Drone* Group::leftRotate(Drone* &node) {
    Drone* child = node->m_right;
    Drone* temp = child->m_left;

    child->m_left = node;
    node->m_right = temp;

    resetHeight(node);
    resetHeight(child);

    return child;
}

Drone* Group::rightRotate(Drone* &node) {
    Drone* child = node->m_left;
    Drone* temp = child->m_right;

    child->m_right = node;
    node->m_left = temp;

    resetHeight(node);
    resetHeight(child);

    return child;
}

Drone* Group::removeRec(Drone* &node, int id) {
        if (node == nullptr)
            return node;
        
        if (node->getID() < id) {
            node->m_right = removeRec(node->m_right, id);
        } else if (node->getID() > id) {
            node->m_left = removeRec(node->m_left, id);
        } else {
            if (node->m_left == nullptr || node->m_right == nullptr) { // at least one null child
                Drone *temp;
                if (node->m_left == nullptr)
                    temp = node->m_right;
                else
                    temp = node->m_left;

                if (temp == nullptr) { // no child
                    temp = node;
                    node = nullptr;
                    delete temp;
                } else { // one child
                    temp = node->m_left ? node->m_left : node->m_right;
                    delete node;
                    return temp;
                }
            } else { // two children
                Drone *min = findMin(node->m_right);

                node->m_id = min->m_id;
                node->m_state = min->m_state;
                node->m_type = min->m_type;
                node->m_right = removeRec(node->m_right, min->m_id);
            }
        }

        if (node == nullptr)
            return node;
        
        resetHeight(node);
        
        int balance = findBalance(node);

        if (balance > 1 && findBalance(node->m_left) > 0) { // left+left
            //cout << "\nleft-left imbalance found at " << node->m_id << ".\n";
            return rightRotate(node);
        } else if (balance > 1 && findBalance(node->m_left) <= 0) { // left+right
            node->m_left = leftRotate(node->m_left);
            return rightRotate(node);
        } else if (balance < -1 && findBalance(node->m_right) <= 0) { // right+right
            return leftRotate(node);
        } else if (balance < -1 && findBalance(node->m_right) > 0) { // right+left
            //cout << "\nleft-left imbalance found at " << node->m_id << ".\n";
            node->m_right = rightRotate(node->m_right);
            return leftRotate(node);
        }
        
       return node;
} 

Drone* Group::findMin(Drone* node) {
        if (node == nullptr)
            return node;
        
        Drone* next = findMin(node->m_left);
        if (next == nullptr)
            return node;

        return next;
}

void Group::listDronesRec(Drone *node) const {
        if (node == nullptr)
            return;

        listDronesRec(node->m_left);
        cout << node->m_id << ':' << node->getTypeStr() << ':' << node->getStateStr() << endl;
        listDronesRec(node->m_right);
}

Drone* Group::find(Drone *node, int id) const {
        if (node == nullptr || node->m_id == id)
            return node;
        
        if (node->m_id < id)
            return find(node->m_right, id);
        else
            return find(node->m_left, id);
}

void Group::removeDeadRec(Drone* &node) {
        if (node == nullptr)
            return;
        
        removeDeadRec(node->m_left);
        removeDeadRec(node->m_right);

        if (node->m_state == DEAD) 
            node = removeRec(node, node->m_id);
        
        if (node != nullptr)
            resetHeight(node);
}

int Group::countColor(Drone *node, LIGHTCOLOR color) const {
        if (node == nullptr)
            return 0;
        
        int count = countColor(node->m_left, color);
        count += countColor(node->m_right, color);

        if (node->m_type == color)
            return count + 1;
        else
            return count;
}

int Group::countState(Drone *node, STATE state) const {
        if (node == nullptr)
            return 0;
        
        int count = countState(node->m_left, state);
        count += countState(node->m_right, state);

        if (node->m_state == state)
            return count + 1;
        else
            return count;
}

    // ***************************************************
    // ******************* Show class ********************
    // ***************************************************

Show::Show() {
    this->m_root = nullptr;
}

Show::~Show(){
    deallocateRec(this->m_root);
}

bool Show::insert(int group, Drone drones[], int population){
    Group *node = new Group(group, drones, population);

    if (node->m_root == nullptr && population >= 1)
        return false;
    else {
        insertRec(this->m_root, node);
        this->m_root = splay(this->m_root, group);
    
        if (this->m_root == nullptr || this->m_root->m_groupID != group) {
            return false;
        } else {
            return true;
        }
    }
}

void Show::dump(bool verbose) const{
	dumpHelper(m_root, verbose);
    cout << endl;
}

void Show::dumpHelper(Group* root, bool verbose) const{
    if (root != nullptr){
        {
            cout << "(";
            dumpHelper( root->m_left, verbose );
            if (verbose)
                cout << root->m_groupID << ":" << root->m_root->m_id;
            else
                cout << root->m_groupID;
            dumpHelper( root->m_right, verbose );
            cout << ")";
        }
    }
}
int Show::count(int group, STATE state){
    this->m_root = splay(this->m_root, group);

    if (this->m_root == nullptr || this->m_root->m_groupID != group)
        return 0;
    else
        return this->m_root->count(state);
}

bool Show::removeDrone(int group, int drone, bool all){
    this->m_root = splay(this->m_root, group);

    if (this->m_root == nullptr || this->m_root->m_groupID != group)
        return false;
    else {
        if (all) {
            this->m_root->removeDead();
            return (this->m_root->count(DEAD) == 0);
        } else {
            this->m_root->remove(drone);
            return (!this->m_root->findDrone(drone));
        }
    }
}

int Show::getGroupHeight(int group){
    this->m_root = splay(this->m_root, group);

    cout << "\nnew root: " << this->m_root->m_groupID << ".\n";

    if (this->m_root == nullptr || this->m_root->m_groupID != group) {
        cout << "\nincorrect.\n";
        return -1;
    } else {
        return this->m_root->findHeight(this->m_root->m_root);
    }
}

bool Show::setState(int group, int drone, STATE state){
    this->m_root = splay(this->m_root, group);

    if (this->m_root == nullptr)
        return false;
    else
        return this->m_root->setState(drone, state);
}

    // ***************************************************
    // ********** Show class helper functions ************
    // ***************************************************

void Show::deallocateRec(Group *node) {
    if (node == nullptr)
        return;
        
    deallocateRec(node->m_left);
    deallocateRec(node->m_right);
    delete node;
}

bool Show::insertRec(Group *&node, Group *newNode) {
    if (node == nullptr) {
        node = newNode;
        return true;
    } else if (node->m_groupID == newNode->m_groupID)
        return false;
        
    if (node->m_groupID > newNode->m_groupID) {
        return insertRec(node->m_left, newNode);
    } else {
        return insertRec(node->m_right, newNode);
    }
}

Group* Show::splay(Group *node, int id) {
    if (node == nullptr || node->m_groupID == id) 
        return node;

    if (node->m_groupID > id) {
        if (node->m_left == nullptr)
            return node;

        if (node->m_left->m_groupID > id) { // left+left
            node->m_left->m_left = splay(node->m_left->m_left, id);
            node = rightRotate(node);
        } else if (node->m_left->m_groupID < id) { // left+right
            node->m_left->m_right = splay(node->m_left->m_right, id);
            if (node->m_left->m_right != nullptr)
                node->m_left = leftRotate(node->m_left);
        }

        return (node->m_left == nullptr) ? node : rightRotate(node);
    } else {
        if (node->m_right == nullptr)
            return node;
        
        if (node->m_right->m_groupID > id) { // right+left
            node->m_right->m_left = splay(node->m_right->m_left, id);
            if (node->m_right->m_left != nullptr)
                node->m_right = rightRotate(node->m_right);
        } else if (node->m_right->m_groupID < id) { // right+right
            node->m_right->m_right = splay(node->m_right->m_right, id);
            node = leftRotate(node);
        }

        return (node->m_right == nullptr) ? node : leftRotate(node);
    }
}

Group* Show::leftRotate(Group *&node) {
    Group *child = node->m_right;
    Group *temp = child->m_left;

    child->m_left = node;
    node->m_right = temp;

    return child;
}

Group* Show::rightRotate(Group *&node) {
    Group *child = node->m_left;
    Group *temp = child->m_right;

    child->m_right = node;
    node->m_left = temp;

    return child;
}