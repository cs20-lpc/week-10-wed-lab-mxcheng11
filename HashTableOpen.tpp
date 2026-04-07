#include "HashTableOpen.hpp"

// Constructor
template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(int i) : M(i), ht(nullptr) {
    cout << "Constructor: M = " << M << endl;
    
    ht = new LinkedList<Record>*[M];
    cout << "Allocated ht array of size " << M << endl;
    
    for (int j = 0; j < M; j++) {
        ht[j] = new LinkedList<Record>();
        if (ht[j] == nullptr) {
            cout << "ERROR: Failed to create linked list at index " << j << endl;
        }
    }
    cout << "Constructor complete, all lists created" << endl;
}

// Copy constructor
template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(const HashTableOpen<Key, Val>& copyObj)
: M(0), ht(nullptr) {
    copy(copyObj);
}

// Assignment operator
template <typename Key, typename Val>
HashTableOpen<Key, Val>& HashTableOpen<Key, Val>::operator=
(const HashTableOpen<Key, Val>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

// Destructor
template <typename Key, typename Val>
HashTableOpen<Key, Val>::~HashTableOpen() {
    if (ht != nullptr) {
        for (int i = 0; i < M; i++) {
            if (ht[i] != nullptr) {
                delete ht[i];
            }
        }
        delete[] ht;
    }
}

// Clear
template <typename Key, typename Val>
void HashTableOpen<Key, Val>::clear() {
    if (ht != nullptr) {
        for (int i = 0; i < M; i++) {
            if (ht[i] != nullptr) {
                ht[i]->clear();
            }
        }
    }
}

// Copy helper
template <typename Key, typename Val>
void HashTableOpen<Key, Val>::copy(const HashTableOpen<Key, Val>& copyObj) {
    if (M != copyObj.M) {
        LinkedList<Record>** tmp = new LinkedList<Record>*[copyObj.M];
        if (M < copyObj.M) {
            for (int i = 0; i < M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = M; i < copyObj.M; i++) {
                tmp[i] = new LinkedList<Record>;
            }
        }
        else if (M > copyObj.M) {
            for (int i = 0; i < copyObj.M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = copyObj.M; i < M; i++) {
                delete ht[i];
            }
        }

        M = copyObj.M;
        delete[] ht;
        ht = tmp;
    }

    LinkedList<Record>* myList = nullptr;
    LinkedList<Record>* copyList = nullptr;
    int myListLen = 0;
    int copyListLen = 0;

    for (int i = 0; i < M; i++) {
        myList = ht[i];
        myListLen = myList->getLength();
        copyList = copyObj.ht[i];
        copyListLen = copyList->getLength();

        if (myListLen < copyListLen) {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = myListLen; j < copyListLen; j++) {
                if (myList->isEmpty()) {
                    myList->append(copyList->getElement(j));
                }
                else {
                    myList->insert(0, copyList->getElement(j));
                }
            }
        }
        else if (myListLen > copyListLen) {
            for (int j = 0; j < copyListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = copyListLen; j < myListLen; j++) {
                myList->remove(j);
            }
        }
        else {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
        }
    }
}

// Find
template <typename Key, typename Val>
Val HashTableOpen<Key, Val>::find(const Key& k) const {
    int slot = hash(k);
    
    if (slot < 0 || slot >= M || ht == nullptr || ht[slot] == nullptr) {
        throw string("Hash table not properly initialized");
    }
    
    LinkedList<Record>* list = ht[slot];
    
    for (int i = 0; i < list->getLength(); i++) {
        Record record = list->getElement(i);
        if (record.k == k) {
            return record.v;
        }
    }
    
    throw string("key wasn't found in hash table");
}

// Hash function - ONLY ONE VERSION!
template <typename Key, typename Val>
int HashTableOpen<Key, Val>::hash(const Key& k) const {
    const int FOLD_LEN = 4;
    
    // Safety check - if M is 0, we have a problem
    if (M == 0) {
        cout << "Warning: M is 0 in hash function!" << endl;
        return 0;
    }
    
    unsigned* ikey = (unsigned*) k.c_str();
    int ilen = k.length() / FOLD_LEN;
    unsigned sum = 0;
    
    for (int i = 0; i < ilen; i++) {
        sum += ikey[i];
    }
    
    int extra = k.length() - ilen * FOLD_LEN;
    char temp[FOLD_LEN];
    ikey = (unsigned*) temp;
    ikey[0] = 0;
    
    for (int i = 0; i < extra; i++) {
        temp[i] = k[ilen * FOLD_LEN + i];
    }
    
    sum += ikey[0];
    int slot = sum % M;
    
    cout << k << "\thashes to slot " << slot << endl;
    
    return slot;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::insert(const Key& k, const Val& v) {
    Record newRec(k, v);
    int slot = hash(k);
    ht[slot]->insert(0, newRec);  // This will now work!
}

// Remove
template <typename Key, typename Val>
void HashTableOpen<Key, Val>::remove(const Key& k) {
    if (ht == nullptr) {
        throw string("Hash table not initialized");
    }
    
    int slot = hash(k);
    
    if (slot < 0 || slot >= M || ht[slot] == nullptr) {
        throw string("Invalid slot or uninitialized list");
    }
    
    LinkedList<Record>* list = ht[slot];
    
    for (int i = 0; i < list->getLength(); i++) {
        Record record = list->getElement(i);
        if (record.k == k) {
            list->remove(i);
            return;
        }
    }
    
    throw string("key wasn't found in hash table");
}

// Size
template <typename Key, typename Val>
int HashTableOpen<Key, Val>::size() const {
    if (ht == nullptr) {
        return 0;
    }
    
    int total = 0;
    for (int i = 0; i < M; i++) {
        if (ht[i] != nullptr) {
            total += ht[i]->getLength();
        }
    }
    return total;
}