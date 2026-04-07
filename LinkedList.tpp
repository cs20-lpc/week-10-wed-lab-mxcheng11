template <typename T>
LinkedList<T>::LinkedList()
: head(nullptr) { }

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& copyObj) {
    copy(copyObj);
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
void LinkedList<T>::append(const T& elem) {
    Node* n = new Node(elem);

    if (head == nullptr) {
        head = n;
    }
    else {
        Node* curr = head;

        while (curr->next != nullptr) {
            curr = curr->next;
        }

        curr->next = n;
    }

    this->length++;
}

template <typename T>
void LinkedList<T>::clear() {
    Node* prev = nullptr;

    while (head != nullptr) {
        prev = head;
        head = head->next;
        delete prev;
    }

    this->length = 0;
}

template <typename T>
void LinkedList<T>::copy(const LinkedList<T>& copyObj) {
    this->length = copyObj.length;
    head         = nullptr;

    if (this->length > 0) {
        head           = new Node(copyObj.head->value);
        Node* myCurr   = head;
        Node* copyCurr = copyObj.head->next;

        while (copyCurr != nullptr) {
            Node* n      = new Node(copyCurr->value);
            myCurr->next = n;
            myCurr       = n;
            copyCurr     = copyCurr->next;
        }
    }
}

template <typename T>
T LinkedList<T>::getElement(int position) const {
    if (position < 0 || position >= this->length) {
        throw string("getElement: error, position out of bounds");
    }
    
    Node* curr = head;

    for (int i = 0; i < position; i++) {
        curr = curr->next;
    }

    return curr->value;
}

template <typename T>
int LinkedList<T>::getLength() const {
    return this->length;
}

// FIXED insert method
template <typename T>
void LinkedList<T>::insert(int position, const T& elem) {
    // Allow position 0 when empty, and allow position == length (append)
    if (position < 0 || position > this->length) {
        throw string("insert: error, position out of bounds");
    }

    Node* n = new Node(elem);
    
    if (position == 0) {
        // Insert at beginning
        n->next = head;
        head = n;
    } else {
        // Insert in middle or end
        Node* curr = head;
        for (int i = 0; i < position - 1; i++) {
            curr = curr->next;
        }
        n->next = curr->next;
        curr->next = n;
    }
    
    this->length++;
}

template <typename T>
bool LinkedList<T>::isEmpty() const {
    return this->length == 0;
}

template <typename T>
void LinkedList<T>::remove(int position) {
    if (position < 0 || position >= this->length) {
        throw string("remove: error, position out of bounds");
    }

    if (head->next == nullptr) {
        delete head;
        head = nullptr;
    }
    else {
        Node* curr = head;
        Node* prev = nullptr;

        for (int i = 0; i < position; i++) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == head) {
            head = head->next;
        }
        else {
            prev->next = curr->next;
        }

        delete curr;
    }

    this->length--;
}

template <typename T>
void LinkedList<T>::replace(int position, const T& elem) {
    if (position < 0 || position >= this->length) {
        throw string("replace: error, position out of bounds");
    }

    Node* curr = head;

    for (int i = 0; i < position; i++) {
        curr = curr->next;
    }

    curr->value = elem;
}

template <typename T>
ostream& operator<<(ostream& outStream, const LinkedList<T>& myObj) {
    if (myObj.isEmpty()) {
        outStream << "List is empty, no elements to display.\n";
    }
    else {
        typename LinkedList<T>::Node* curr = myObj.head;
        while (curr != nullptr) {
            outStream << curr->value;
            if (curr->next != nullptr) {
                outStream << " --> ";
            }
            curr = curr->next;
        }
        outStream << endl;
    }

    return outStream;
}