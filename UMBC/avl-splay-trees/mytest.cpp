// UMBC - CMSC 341 - Fall 2026 - Proj2
#include "show.h"
using namespace std;
#include <vector>
#include <random>
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{ // Tester class to implement test functions
    public:

    bool testAVLBalance() {
        Group group = createGroup(10);
        
        return testAVLBalanceRec(group.m_root) && findDroneHeight(group.m_root) == 3;
    }

    bool testBSTProperty() {
        Group group = createGroup(10);

        return testBSTPropertyRec(group.m_root);
    }

    bool testSplayInsertion() {
        Random idGen(MINID,MAXID);
        Random typeGen(0,2);
        Random stateGen(0,2);
        Show aShow;

        for (int i=0; i<10; i++){
            Drone drones[10];
            int groupSize = 1;
            for(int i=0;i<groupSize;i++){
                Drone drone(idGen.getRandNum(), (LIGHTCOLOR)(typeGen.getRandNum()), (STATE)(stateGen.getRandNum()));
                drones[i] = drone;
            }

            int id = idGen.getRandNum();
            aShow.insert(id, drones, 10);

            if (aShow.m_root->m_groupID != id || (!testBSTPropertyRec(aShow.m_root->m_root)))
                return false;
        }

        return true;
    }

    bool testAVLBalanceRemoval() {
        Group group = createGroup(10);

        for (int i = 0; i < 8; i++) {
            group.remove(group.m_root->m_id);
        }

        return testAVLBalanceRec(group.m_root);
    }

    bool testAVLBSTRemoval() {
        Group group = createGroup(10);

        for (int i = 0; i < 5; i++) {
            group.remove(group.m_root->m_id);
        }

        return testBSTPropertyRec(group.m_root);
    }

    bool testAVLRemovals() {
        Group group = createGroup(25);
        int rootID;

        for (int i = 0; i < 18; i++) {
            rootID = group.m_root->m_id;
            group.remove(rootID);

            if (group.findDrone(rootID))
                return false;
        }

        return true;
    }

    bool testAVLSearches() {
        Group group;
        Random idGen(MINID,MAXID);
        Random typeGen(0,2);
        Random stateGen(0,2);

        Drone drones[25];
        int id;

        for(int i = 0; i < 25; i++) {
            id = idGen.getRandNum();

            Drone drone(id, (LIGHTCOLOR)(typeGen.getRandNum()), (STATE)(stateGen.getRandNum()));
            drones[i] = drone;
            group.insert(drone);
        }

        for (int i = 0; i < 10; i++) {
            if (!group.findDrone(drones[i].getID()))
                return false;
        }

        return true;
    }

    bool testShowCount() {
        Show show = createShow(10);

        Group *temp = show.m_root;
        if (show.m_root->m_left == nullptr)
            show.count(show.m_root->m_right->m_groupID, DEAD);
        else
            show.count(show.m_root->m_left->m_groupID, DEAD);
        
        if (temp == show.m_root)
            return false;
        
        return (show.count(show.m_root->m_groupID, LIGHTOFF) == testShowCountHelper(show.m_root->m_root, LIGHTOFF));
    }

    bool testShowSetState() {
        Show show = createShow(10);

        Group *temp = show.m_root;
        if (show.m_root->m_left == nullptr)
            show.setState(show.m_root->m_right->m_groupID, show.m_root->m_right->m_root->m_id,  DEAD);
        else
            show.setState(show.m_root->m_left->m_groupID, show.m_root->m_left->m_root->m_id,  DEAD);
        
        if (temp == show.m_root) {
            return false;
        }
        
        return (show.m_root->m_root->m_state == DEAD);
    }

    bool testShowBST() {
        Show show = createShow(20);

        return testShowBSTRec(show.m_root);
    }

    private:
    // helper functions

    Group createGroup(int);
    Show createShow(int);
    int findDroneHeight(Drone *node);
    int findDroneValue(Drone *node);
    int testShowCountHelper(Drone *node, STATE state);
    bool testAVLBalanceRec(Drone *node);
    bool testBSTPropertyRec(Drone *node);
    bool testShowBSTRec(Group *node);
};
int main(){
    Tester tester;

    {
        cout << "\nTesting tree balance: " << (tester.testAVLBalance() ? "\t\t\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing BST property: " << (tester.testBSTProperty() ? "\t\t\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing splay insertion: " << (tester.testSplayInsertion() ? "\t\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing tree balance after removals: " << (tester.testAVLBalanceRemoval() ? "\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing BST property after removals: " << (tester.testAVLBSTRemoval() ? "\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing multiple AVL removals: " << (tester.testAVLRemovals() ? "\t\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing AVL node searches: " << (tester.testAVLSearches() ? "\t\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing show count: " << (tester.testShowCount() ? "\t\t\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing show setState: " << (tester.testShowSetState() ? "\t\t\tpass.\n" : "\tfail.\n");
    }
    {
        cout << "Testing show BST property: " << (tester.testShowBST() ? "\t\tpass.\n" : "\tfail.\n");
    }

    return 0;
}

// ********************************************************
// ********************************************************
// ********************************************************

Group Tester::createGroup(int groupSize) {
    Random idGen(MINID,MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2);

    Group group;
    int ID = 0;

    for(int i = 0; i < groupSize; i++) {
        ID = idGen.getRandNum();

        Drone drone(ID, (LIGHTCOLOR)(typeGen.getRandNum()), (STATE)(stateGen.getRandNum()));
        group.insert(drone);
    }    

    return group;
}

Show Tester::createShow(int showSize) {
    Random idGen(MINID,MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2);

    Show aShow;
        
    for (int i=0; i<showSize; i++){
        Drone drones[10];
        int groupSize = 10;
        for(int i=0;i<groupSize;i++){
            Drone drone(idGen.getRandNum(), (LIGHTCOLOR)(typeGen.getRandNum()), (STATE)(stateGen.getRandNum()));
            drones[i] = drone;
        }
        aShow.insert(idGen.getRandNum(), drones, 10);
    }

    return aShow;
}

int Tester::findDroneHeight(Drone *node) {
    if (node == nullptr)
        return -1;
    else
        return (node->getHeight());
}

int Tester::findDroneValue(Drone *node) {
    if (node == nullptr)
        return -1;
    else
        return node->m_id;
}

int Tester::testShowCountHelper(Drone *node, STATE state) {
    if (node == nullptr)
        return 0;
    
    int ret = testShowCountHelper(node->m_left, state) + testShowCountHelper(node->m_right, state);

    if (node->m_state == state)
        ret++;

    return ret;
}

bool Tester::testAVLBalanceRec(Drone *node) {
    if (node == nullptr)
        return true;
    
    int lheight = findDroneHeight(node->m_left);
    int rheight = findDroneHeight(node->m_right);

    if (lheight < rheight - 1 && lheight > rheight + 1)
        return false;
    
    return (testAVLBalanceRec(node->m_left) && testAVLBalanceRec(node->m_right));
}

bool Tester::testBSTPropertyRec(Drone *node) {
    if (node == nullptr)
        return true;
    
    if (findDroneValue(node->m_left) >= node->m_id) {
        return false;
    } else if (node->m_right != nullptr && findDroneValue(node->m_right) <= node->m_id) {
        return false;
    }

    return (testBSTPropertyRec(node->m_left) && testBSTPropertyRec(node->m_right));
}

bool Tester::testShowBSTRec(Group *node) {
    if (node == nullptr)
        return true;
    
    if (node->m_left != nullptr && node->m_left->m_groupID > node->m_groupID) {
        return false;
    } else if (node->m_right != nullptr && node->m_right->m_groupID < node->m_groupID) {
        return false;
    }

    return (testShowBSTRec(node->m_left) && testShowBSTRec(node->m_right));
}