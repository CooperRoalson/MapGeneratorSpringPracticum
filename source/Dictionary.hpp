//
//  Dictionary.hpp
//  Map Generator
//

#ifndef Dictionary_hpp
#define Dictionary_hpp

#include "LinkedList.hpp"

template <class K,class V> class Dictionary {
    
    public:
        LinkedList<K> getKeys();
        LinkedList<V> getValues();
    
        V get(K key);
        void set(K key, V value);
        void add(K key, V value);
        V del(K key);
            
        int length();
    
        void clear();
        bool isEmpty();
    
    private:
        LinkedList<K> keys;
        LinkedList<V> values;
};

template <class K, class V> LinkedList<K> Dictionary<K,V>::getKeys() {
    return keys;
}

template <class K, class V> LinkedList<V> Dictionary<K,V>::getValues() {
    return values;
}

template <class K, class V> V Dictionary<K,V>::get(K key) {
    return values.get(keys.findFirst(key));
}

template <class K, class V> void Dictionary<K,V>::set(K key, V value) {
    values.set(keys.findFirst(key), value);
}

template <class K, class V> void Dictionary<K,V>::add(K key, V value) {
    if (keys.findFirst(key) != -1) {return;}
    keys.add(key);
    values.add(value);
}

template <class K, class V> V Dictionary<K,V>::del(K key) {
    int index = keys.findFirst(key);
    keys.del(index);
    return values.del(index);
}

template <class K, class V> int Dictionary<K,V>::length() {
    return keys.length();
}

template <class K, class V> void Dictionary<K,V>::clear() {
    keys.clear();
    values.clear();
}

template <class K, class V> bool Dictionary<K,V>::isEmpty() {
    return keys.isEmpty();
}

#endif /* Dictionary_hpp */
