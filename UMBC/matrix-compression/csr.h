// CMSC 341 - Fall 2026 - Project 1
#ifndef CSR_H
#define CSR_H
#include <iostream>
#include <string>
#include <stdexcept>
using std::runtime_error;
using namespace std;
class Grader;
class Tester;
// CSR class represents a matrix
class CSR{
    public:
    friend class Grader;
    friend class Tester;
    friend class CSRList;
    CSR();
    ~CSR();
    CSR(const CSR & rhs);
    void compress(int m, int n, int array[], int arraySize);
    int getAt(int row, int  col) const;
    int sparseRatio();
    bool operator==(const CSR & rhs) const;
    void clear();
    bool empty() const;
    void dump();        //prints data for the node
    private:
    int* m_values;      //array to store non-zero values
    int* m_col_index;   //array to store column indices
    int* m_row_index;   //array to store row indices 
    int m_nonzeros;     //number of non-zero values
    int m_m;            //number of rows
    int m_n;            //number of columns
    CSR* m_next;        //pointer to the next CSR object in linked list

    /******************************************
    * Private function declarations go here! *
    ******************************************/
    void deallocate() {
        //cout << "clearing V\n";
        delete [] m_values;
        //cout << "clearing Cols\n";
        delete [] m_col_index;
        //cout << "clearing Rows\n";
        delete [] m_row_index;
    }

    void setNullPtrs() {
        m_values = nullptr;
        m_col_index = nullptr;
        m_row_index = nullptr;
        m_next = nullptr;
        m_m = -1;
        m_n = -1;
        m_nonzeros = -1;
    }

    void reSize() {
        int * newValueArr = new int[this->m_nonzeros];
        int * newColArr = new int[this->m_nonzeros];

        for (int i = 0; i < this->m_nonzeros; i++) {
            newValueArr[i] = this->m_values[i];
            newColArr[i] = this->m_col_index[i];
        }

        delete [] this->m_values;
        this->m_values = newValueArr;

        delete [] this->m_col_index;
        this->m_col_index = newColArr;
    }
};

// CSRList class represents a list of matrices (CSR objects)
class CSRList{
    public:
    friend class Grader;
    friend class Tester;
    CSRList();
    ~CSRList();
    CSRList(const CSRList & rhs);
    void insertAtHead(const CSR & matrix);
    const CSRList& operator=(const CSRList & rhs);
    bool operator==(const CSRList & rhs) const;
    int getAt(int CSRIndex, int row, int col) const;
    int averageSparseRatio();
    void clear();
    bool empty() const;
    void dump();    //prints data for all nodes
    private:
    CSR* m_head;    //the pointer to the head of the list
    int m_size;     //the current number of nodes in the list

    /******************************************
    * Private function declarations go here! *
    ******************************************/

    void deallocate() {
        CSR* temp = this->m_head;

        while (temp != nullptr) {
            CSR* ptrNext = temp->m_next;
            m_head = m_head->m_next;
            delete temp;
            temp = ptrNext;
        }

        m_head = nullptr;
    }

    void recreateRec(CSR* node) { //Start from the tail of the list and insert going backwards towards the front of the list
        if (node->m_next != nullptr) 
            recreateRec(node->m_next);

        this->insertAtHead(*node);
    }
};
#endif