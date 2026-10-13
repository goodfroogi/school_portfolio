// CMSC 341 - Fall 2026 - Project 1
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){
    this->setNullPtrs();
}
CSR::~CSR(){
    deallocate();
}
CSR::CSR(const CSR & rhs){
    this->setNullPtrs();

    if (rhs.empty()) 
        return;

    this->m_nonzeros = rhs.m_nonzeros;
    this->m_m = rhs.m_m;
    this->m_n = rhs.m_n;

    this->m_values = new int[this->m_nonzeros];
    this->m_col_index = new int[this->m_nonzeros];
    this->m_row_index = new int[this->m_m + 1];

    for (int i = 0; i < this->m_nonzeros; i++) {
        this->m_values[i] = rhs.m_values[i];
        this->m_col_index[i] = rhs.m_col_index[i];
    }

    for (int i = 0; i <= this->m_m; i++) {
        this->m_row_index[i] = rhs.m_row_index[i];
    }
}
void CSR::clear(){
    deallocate();
    setNullPtrs();
}
bool CSR::empty() const{
    return (m_values == nullptr);
}
void CSR::compress(int m, int n, int array[], int arraySize){
    if (m < 1 || n < 1 || arraySize != (m * n)) {
        //cout << "Invalid matrix/data size.\n\n";
        return;
    }

    this->clear();

    this->m_m = m;
    this->m_n = n;
    this->m_row_index = new int[this->m_m + 1];

    this->m_values = new int[arraySize];
    this->m_col_index = new int[arraySize]; // These will be resized

    int rowCount = 0; // Index in m_row_index being edited
    this->m_nonzeros = 0;

    for (int i = 0; i < arraySize; i++) {
        if (i % n == 0) {
            //cout << m_nonzeros << "\n\n";
            this->m_row_index[rowCount] = m_nonzeros;
            rowCount++;
        }

        if (array[i] != 0) {
            this->m_values[this->m_nonzeros] = array[i];
            this->m_col_index[this->m_nonzeros] = (i % n);
            this->m_nonzeros++;
        }
    }

    this->m_row_index[m] = m_nonzeros;

    this->reSize();
}
int CSR::getAt(int row, int  col) const{
    if (row < 0 || row >= m_m || col < 0 || col >= m_n) 
        throw runtime_error("");
    
    int start = this->m_row_index[row];
    int end = this->m_row_index[row+1];

    for (int i = start; i < end; i++) {
        if (this->m_col_index[i] == col) {
            return this->m_values[i];
        }
    }

    return 0;
} 
bool CSR::operator==(const CSR & rhs) const{
    if (this->empty() && rhs.empty()) 
        return true; //Both are empty
    if (this->empty() && !rhs.empty()) 
        return false; //Only one is empty
    if (!this->empty() && rhs.empty()) 
        return false; //..

    if (this->m_m != rhs.m_m || this->m_n != rhs.m_n || this->m_nonzeros != rhs.m_nonzeros) 
        return false;

    for (int i = 0; i < this->m_nonzeros; i++) {
        if (this->m_values[i] != rhs.m_values[i] || this->m_col_index[i] != rhs.m_col_index[i]) 
            return false;
    }

    return true;
}
int CSR::sparseRatio(){
    return static_cast<int>((static_cast<float>(this->m_nonzeros) / (this->m_m * this->m_n)) * 100);
}
void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){
    this->m_head = nullptr;
    this->m_size = 0;
}
CSRList::CSRList(const CSRList & rhs){
    this->m_head = nullptr;
    this->m_size = 0;

    if (!rhs.empty())
        this->recreateRec(rhs.m_head);
}
CSRList::~CSRList(){
    this->deallocate();
}
bool CSRList::empty() const{
    return (this->m_size == 0);
}
void CSRList::insertAtHead(const CSR & matrix){
    this->m_size++;

    CSR* temp = new CSR(matrix);
    temp->m_next = this->m_head;
    this->m_head = temp;
}
void CSRList::clear(){
    this->deallocate();
    this->m_size = 0;
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    if (CSRIndex < 0 || CSRIndex >= this->m_size || this->empty())
        throw runtime_error("Object is not in the list!");

    CSR* current = this->m_head;

    for (int i = 0; i < CSRIndex; i++) {
        current = current->m_next;
    }

    return current->getAt(row, col);
}
bool CSRList::operator== (const CSRList & rhs) const{
    if (this->m_size != rhs.m_size)
        return false;

    CSR* lhsPtr = this->m_head;
    CSR* rhsPtr = rhs.m_head;

    for (int i = 0; i < this->m_size; i++) {
        if (!(*lhsPtr == *rhsPtr))
            return false;
        
        lhsPtr = lhsPtr->m_next;
        rhsPtr = rhsPtr->m_next;
    }

    return true;
}
const CSRList& CSRList::operator=(const CSRList & rhs){
    this->clear();

    if (!rhs.empty())
        this->recreateRec(rhs.m_head);

    return *this;
}
int CSRList::averageSparseRatio(){
    int average = 0;
    int count = 0;
    CSR* current = this->m_head;

    for (int i = 0; i < this->m_size; i++) {
        count += current->sparseRatio();
        current = current->m_next;
    }

    average = static_cast<int>(static_cast<float>(count) / this->m_size);
    return average;
}
void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}