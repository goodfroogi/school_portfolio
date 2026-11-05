// UMBC - CMSC 341 - Fall 2026 - Proj2
#ifndef SHOW_H
#define SHOW_H
#include <iostream>
using namespace std;
class Grader; // this class is for grading purposes, no need to do anything
class Tester; // this is your tester class, you add your test functions in this class
class Group;  // this is the class that you implement
class Show;   // this is the class that you implement
enum STATE {LIGHTON, LIGHTOFF, DEAD};
enum LIGHTCOLOR {RED,GREEN,BLUE};
const int MINID = 10000;
const int MAXID = 99999;
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0 // this is for empty object
#define DEFAULT_LIGHT RED
#define DEFAULT_STATE LIGHTOFF

class Drone{
    public:
    friend class Grader;
    friend class Tester;
    friend class Group;
    friend class Show;
    Drone(int id, LIGHTCOLOR type = DEFAULT_LIGHT, STATE state = DEFAULT_STATE){
        m_id = id; m_type = type; m_state = state;
        m_left = nullptr; m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    Drone(){
        m_id = DEFAULT_ID; m_type = DEFAULT_LIGHT; m_state = DEFAULT_STATE;
        m_left = nullptr; m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
        case LIGHTOFF:text = "LIGHTOFF";break;
        case LIGHTON:text = "LIGHTON";break;
        case DEAD:text = "DEAD";break;
        default:text = "UNKNOWN";break;
        }
        return text;
    }
    LIGHTCOLOR getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
        case RED:text = "RED";break;
        case GREEN:text = "GREEN";break;
        case BLUE:text = "BLUE";break;
        default:text = "UNKNOWN";break;
        }
        return text
        ;
    }
    int getHeight() const {return m_height;}
    Drone* getLeft() const {return m_left;}
    Drone* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(LIGHTCOLOR type){m_type=type;}
    void setHeight(int height){m_height=height;}
    void setLeft(Drone* left){m_left=left;}
    void setRight(Drone* right){m_right=right;}
    
    private:
    int m_id;
    LIGHTCOLOR m_type;
    STATE m_state;
    Drone* m_left;//the pointer to the left child
    Drone* m_right;//the pointer to the right child
    int m_height;//the height of node
};

class Group{ // this is an AVL tree, the nodes are of type Drone
    public:
    friend class Grader;
    friend class Tester;
    friend class Show;
    Group();
    Group(int group, Drone drones[], int population);
    ~Group();
    void insert(const Drone& drone);
    void clear();
    void remove(int id);
    void dumpTree() const;
    void listDrones() const;
    bool setState(int id, STATE state);
    void removeDead();//remove all dead drones from the tree
    bool findDrone(int id) const;//returns true if the drone is in tree
    int count(STATE state) const;
    int count(LIGHTCOLOR aColor) const;
    
    private:
    Drone* m_root;      //the root of the AVL tree
    int m_groupID;      // unique ID of the group
    Group* m_left;
    Group* m_right;

    void dump(Drone* aDrone) const;//helper for recursive traversal

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************

    void deallocateRec(Drone* node);
    void setNullPtrs();
    Drone* insertRec(Drone* &current, int, LIGHTCOLOR, STATE);
    int findHeight(Drone* node);
    void resetHeight(Drone* node);
    int findBalance(Drone* node);
    Drone* leftRotate(Drone* &node);
    Drone* rightRotate(Drone* &node);
    Drone* removeRec(Drone* &node, int id);
    Drone* findMin(Drone* node);
    void listDronesRec(Drone *node) const;
    Drone* find(Drone *node, int id) const;
    void removeDeadRec(Drone* &node);
    int countColor(Drone *node, LIGHTCOLOR color) const;
    int countState(Drone *node, STATE state) const;
};

class Show{ // this is a splay tree, the nodes are of type Group
    public:
    friend class Grader;
    friend class Tester;
    Show();
    ~Show();
    void dump(bool verbose=false) const;
    bool insert(int group, Drone drones[], int population);
    int count(int group, STATE state);
    bool removeDrone(int group, int drone, bool all);
    bool setState(int group, int drone, STATE state);
    int getGroupHeight(int group);

    private:
    Group* m_root;//the root of the Splay tree

    void dumpHelper(Group* root, bool verbose=false) const;

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************
    
    void deallocateRec(Group *node);
    bool insertRec(Group *&node, Group *newNode);
    Group* splay(Group *node, int id);
    Group* leftRotate(Group *&node);
    Group* rightRotate(Group *&node);
};
#endif