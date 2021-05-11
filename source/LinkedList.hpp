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
    
        E get(int i);
        void set(int i, E e);
        void add(E e);
        E del(int i);
    
        int findFirst(E e);
        
        int length();
    
        void clear();
        bool isEmpty();
    
    private:
        Node* head;
        int len;
        
        void* getNode(int i);
};

// Function bodies

template <class E> LinkedList<E>::LinkedList() {
    head = nullptr;
    len = 0;
}

template <class E> LinkedList<E>::~LinkedList() {
    clear();
}

template <class E> void* LinkedList<E>::getNode(int i) {
    if (i < 0 || i >= len) {
        throw std::out_of_range("Index " + std::to_string(i) + " out of range for list of length " + std::to_string(len));
    }
        
    Node* currentNode = head;
    for (int index = 0; index < i; index++) {
        currentNode = currentNode->next;
    }
    
    return (void*) currentNode;
}

template <class E> E LinkedList<E>::get(int i) {
    return ((Node*) getNode(i))->data;
}

template <class E> void LinkedList<E>::set(int i, E e) {
    ((Node*) getNode(i))->data = e;
}

template <class E> void LinkedList<E>::add(E e) {
    Node* newNode = new Node(e, nullptr);
    
    if (len == 0) {head = newNode; len ++; return;}
    
    ((Node*) getNode(len-1))->next = newNode;
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
    Node* prevNode = ((Node*) getNode(i - 1));
    Node* curNode = prevNode->next;
    prevNode->next = curNode->next;
    E val = curNode->data;
    delete curNode;
    len--;
    return val;
}

template <class E> int LinkedList<E>::findFirst(E e) {
    Node* currentNode = head;
    for (int index = 0; index < len; index++) {
        if (currentNode->data == e) {
            return index;
        }
        currentNode = currentNode->next;
    }
    return -1;
}

template <class E> int LinkedList<E>::length() {
    return len;
}

template <class E> void LinkedList<E>::clear() {
    while (len > 0) {
        del(len - 1);
    }
}
template <class E> bool LinkedList<E>::isEmpty() {
    return len == 0;
}

#endif /* LinkedList_hpp */
