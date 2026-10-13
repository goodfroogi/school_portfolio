#include <iostream>
#include "csr.cpp"

using namespace std;

class Tester {
    public:
    /******************************************
    ********* Testing the CSR class ***********
    ******************************************/

    bool compressNormal(CSR & matrix) {
        int arr[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};

        matrix.compress(4, 6, arr, 24);
        //matrix.dump();    -   Used to get these 3 arrays
        int test1[] = {10, 20, 30, 40, 50, 60, 70, 80}; //m_values
        int test2[] = {0, 1, 1, 3, 2, 3, 4, 5}; //m_col_index
        int test3[] = {0, 2, 4, 7, 8}; //m_row_index

        for (int i = 0; i < 8; i++) {
            if (i < 5) {
                if (matrix.m_row_index[i] != test3[i]) 
                    return false;
            }
            
            if (matrix.m_col_index[i] != test2[i] || matrix.m_values[i] != test1[i]) 
                return false;
        }

        return true;
    }

    bool compressSmall(CSR & matrix) {
        int arr[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};

        matrix.compress(5, 6, arr, 24);
        return (matrix.empty());
    }

    bool compressLarge(CSR & matrix) {
        int arr[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};

        matrix.compress(4, 4, arr, 24);
        return (matrix.empty());
    }

    bool identicalMatricesCheck() {
        CSR matrix1, matrix2;
        int arr[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};

        matrix1.compress(4, 6, arr, 24);
        matrix2.compress(4, 6, arr, 24);

        return (matrix1 == matrix2);
    }

    bool blankMatricesCheck() {
        CSR matrix1, matrix2;

        return (matrix1 == matrix2);
    }

    bool CSRGetAtCheck() {
        CSR matrix;
        int arr[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
        matrix.compress(4, 6, arr, 24);

        try {
            matrix.getAt(0,6);
            return false;
        } catch (runtime_error) {
            return true;
        }
    }

    /******************************************
    ******** Testing the CSRList class ********
    ******************************************/

    bool CSRListAssignmentNormal(CSRList & list1, CSRList & list2) {
        int arr1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
        int arr2[] = {0,0,0,0,100,200,0,0,300};
        int arr3[] = {0,0,1,19,0,3};

        CSR matrix1, matrix2, matrix3;
        matrix1.compress(4, 6, arr1, 24);
        matrix2.compress(3, 3, arr2, 9);
        matrix3.compress(2, 3, arr3, 6);

        list1.insertAtHead(matrix1);
        list1.insertAtHead(matrix2);
        list1.insertAtHead(matrix3);

        list2.insertAtHead(matrix3);
        list2.insertAtHead(matrix2);
        list2.insertAtHead(matrix1);

        list1 = list2;

        return (list1 == list2);
    }

    bool CSRListAssignmentEdge(CSRList & list1, CSRList & list2) {
        int arr1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
        int arr2[] = {0,0,0,0,100,200,0,0,300};
        int arr3[] = {0,0,1,19,0,3};

        CSR matrix1, matrix2, matrix3;
        matrix1.compress(4, 6, arr1, 24);
        matrix2.compress(3, 3, arr2, 9);
        matrix3.compress(2, 3, arr3, 6);

        list1.insertAtHead(matrix1);
        list1.insertAtHead(matrix2);
        list1.insertAtHead(matrix3);

        list1 = list2;

        return (list1 == list2);
    }
    
    bool CSRListGetAtError(CSRList & list) { //List is empty
        try {
            list.getAt(0,0,0);
            return false;
        } catch (runtime_error) {
            return true;
        }
    }

    bool CSRListGetAtNormal(CSRList & list) {
        int arr1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
        int arr2[] = {0,0,0,0,100,200,0,0,300};
        int arr3[] = {0,0,1,19,0,3};

        CSR matrix1, matrix2, matrix3;
        matrix1.compress(4, 6, arr1, 24);
        matrix2.compress(3, 3, arr2, 9);
        matrix3.compress(2, 3, arr3, 6);

        list.insertAtHead(matrix1);
        list.insertAtHead(matrix2);
        list.insertAtHead(matrix3);

        try {
            if (list.getAt(2,0,0) == 10)
                return true;
            else 
                return false;
            
        } catch (runtime_error) {
            return false;
        }
    }
};

int main() {
    Tester tester;

    {
        CSR matrix;

        cout << endl << "Testing compression function with valid input:\t\t\t" << ((tester.compressNormal(matrix)) ? "pass.\n" : "fail.\n");
    }
    {
        CSR matrix;

        cout << "Testing compression function with too little data:\t\t" << ((tester.compressSmall(matrix)) ? "pass.\n" : "fail.\n");
    }
    {
        CSR matrix;

        cout << "Testing compression function with too much data:\t\t" << ((tester.compressLarge(matrix)) ? "pass.\n" : "fail.\n");
    }
    {
        cout << "Testing equality overloader with 2 identical, normal matrices:\t" << ((tester.identicalMatricesCheck()) ? "pass.\n" : "fail.\n");
    }
    {        
        cout << "Testing equality overloader with 2 identical, empty matrices:\t" << ((tester.blankMatricesCheck()) ? "pass.\n" : "fail.\n");
    }
    {
        cout << "Testing getAt() for an invalid row/col:\t\t\t\t" << ((tester.CSRGetAtCheck()) ? "pass.\n" : "fail.\n");
    }

    //CSRList Test Functions

    {
        CSRList list1, list2;

        cout << "Testing assignment opperator for 2 different CSR lists:\t\t" << ((tester.CSRListAssignmentNormal(list1, list2)) ? "pass.\n" : "fail.\n");
    }
    {
        CSRList list1, list2;

        cout << "Testing assigning a blank list to a normal one:\t\t\t" << ((tester.CSRListAssignmentEdge(list1, list2)) ? "pass.\n" : "fail.\n");
    }
    {
        CSRList list;

        cout << "Testing for CSRList getAt() error case:\t\t\t\t" << ((tester.CSRListGetAtError(list)) ? "pass.\n" : "fail.\n");
    }
    {
        CSRList list;

        cout << "Testing for CSRList getAt() normal case:\t\t\t" << ((tester.CSRListGetAtNormal(list)) ? "pass.\n" : "fail.\n");
    }

    return 0;
}