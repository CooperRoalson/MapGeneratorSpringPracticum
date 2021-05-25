//
//  LinkedList.hpp
//  Map Generator
//

#ifndef LinkedList_hpp
#define LinkedList_hpp

#include <stdexcept>
#include <string>

template <class E> class LinkedList {
    
    struct Node {
        E data;
        Node* next;
        
        Node(E d, Node* n) {
            data = d;
            next = n;
        }
    };
    
    public:
        LinkedList();
        ~LinkedList();
    
        E get(int i) const;
        void set(int i, E e);
        void add(E e);
        E del(int i);
    
        int findFirst(E e) const;
        bool removeFirst(E e);
        
        int length() const;
    
        void clear();
        bool isEmpty() const;
    
    private:
        Node* head;
        int len;
        
        Node* getNode(int i) const;
};

// Function bodies

template <class E> LinkedList<E>::LinkedList() {
    head = nullptr;
    len = 0;
}

template <class E> LinkedList<E>::~LinkedList() {
    clear();
}

template <class E> typename LinkedList<E>::Node* LinkedList<E>::getNode(int i) const {
    if (i < 0 || i >= len) {
        throw std::out_of_range("Index " + std::to_string(i) + " out of range for list of length " + std::to_string(len));
    }
        
    Node* currentNode = head;
    for (int index = 0; index < i; index++) {
        currentNode = currentNode->next;
    }
    
    return currentNode;
}

template <class E> E LinkedList<E>::get(int i) const {
    return getNode(i)->data;
}

template <class E> void LinkedList<E>::set(int i, E e) {
    getNode(i)->data = e;
}

template <class E> void LinkedList<E>::add(E e) {
    Node* newNode = new Node(e, nullptr);
    
    if (len == 0) {head = newNode; len ++; return;}
    
    getNode(len-1)->next = newNode;
    len++;
}

template <class E> E LinkedList<E>::del(int i) {
    if (i == 0) {
        Node* curNode = head;
        head = curNode->next;
        E val = curNode->data;
        delete curNode;
        len--;
        return val;
    }
    Node* prevNode = getNode(i - 1);
    Node* curNode = prevNode->next;
    prevNode->next = curNode->next;
    E val = curNode->data;
    delete curNode;
    len--;
    return val;
}

template <class E> int LinkedList<E>::findFirst(E e) const {
    Node* currentNode = head;
    for (int index = 0; index < len; index++) {
        if (currentNode->data == e) {
            return index;
        }
        currentNode = currentNode->next;
    }
    return -1;
}

template <class E> bool LinkedList<E>::removeFirst(E e) {
    int i = findFirst(e);
    if (i == -1) {return false;}
    del(i);
    return true;
}

template <class E> int LinkedList<E>::length() const {
    return len;
}

template <class E> void LinkedList<E>::clear() {
    while (len > 0) {
        del(len - 1);
    }
}
template <class E> bool LinkedList<E>::isEmpty() const {
    return len == 0;
}

#endif /* LinkedList_hpp */
