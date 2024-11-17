#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <stdexcept>

// Node Class
template <typename T>
class Node {
public:
    T data;
    Node* next;

    Node(T value) : data(value), next(nullptr) {}
};

// ListIterator Class
template <typename T>
class ListIterator {
private:
    Node<T>* current;

public:
    explicit ListIterator(Node<T>* start) : current(start) {}

    bool HasNext() {
        return current != nullptr;
    }

    T& Next() {
        if (!HasNext()) {
            throw std::out_of_range("No more elements in the list.");
        }
        T& value = current->data;
        current = current->next;
        return value;
    }
};

// LinkedList Class
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

    Node<T>* GetNode(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range.");
        }
        Node<T>* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    }

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void Add(T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (tail != nullptr) {
            tail->next = newNode;
        }
        else {
            head = newNode;
        }
        tail = newNode;
        ++size;
    }

    void Insert(T& value, int index) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range.");
        }

        Node<T>* newNode = new Node<T>(value);

        if (index == 0) {
            newNode->next = head;
            head = newNode;
            if (size == 0) {
                tail = newNode;
            }
        }
        else {
            Node<T>* prev = GetNode(index - 1);
            newNode->next = prev->next;
            prev->next = newNode;
            if (index == size) {
                tail = newNode;
            }
        }
        ++size;
    }

    T& Get(int index) {
        return GetNode(index)->data;
    }

    int IndexOf(T& value) {
        Node<T>* current = head;
        int index = 0;
        while (current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            ++index;
        }
        return -1;
    }

    T Remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range.");
        }

        Node<T>* removedNode = nullptr;
        T removedData; // Temporary variable to hold the removed data

        if (index == 0) {
            removedNode = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
        }
        else {
            Node<T>* prev = GetNode(index - 1);
            removedNode = prev->next;
            prev->next = removedNode->next;
            if (removedNode == tail) {
                tail = prev;
            }
        }

        removedData = removedNode->data; // Copy the data
        delete removedNode; // Safely delete the node
        --size;

        return removedData; // Return the copied value
    }


    ListIterator<T> Iterator() {
        return ListIterator<T>(head);
    }

    int Size() {
        return size;
    }
};

// DNode Class (inherits Node)
template <typename T>
class DNode : public Node<T> {
public:
    DNode* prev;

    DNode(T value) : Node<T>(value), prev(nullptr) {}
};

// DoublyLinkedList Class
template <typename T>
class DoublyLinkedList {
private:
    DNode<T>* head;
    DNode<T>* tail;
    int size;

    DNode<T>* GetNode(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range.");
        }
        DNode<T>* current;
        if (index < size / 2) { // Traverse from head
            current = head;
            for (int i = 0; i < index; ++i) {
                current = static_cast<DNode<T>*>(current->next);
            }
        } else { // Traverse from tail
            current = tail;
            for (int i = size - 1; i > index; --i) {
                current = current->prev;
            }
        }
        return current;
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList() {
        while (head != nullptr) {
            DNode<T>* temp = head;
            head = static_cast<DNode<T>*>(head->next);
            delete temp;
        }
    }

    void Add(T& value) {
        DNode<T>* newNode = new DNode<T>(value);
        if (tail != nullptr) {
            tail->next = newNode;
            newNode->prev = tail;
        } else {
            head = newNode;
        }
        tail = newNode;
        ++size;
    }

    void Insert(T& value, int index) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range.");
        }

        DNode<T>* newNode = new DNode<T>(value);

        if (index == 0) {
            newNode->next = head;
            if (head != nullptr) {
                head->prev = newNode;
            }
            head = newNode;
            if (size == 0) {
                tail = newNode;
            }
        } else if (index == size) {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        } else {
            DNode<T>* nextNode = GetNode(index);
            DNode<T>* prevNode = nextNode->prev;
            newNode->next = nextNode;
            newNode->prev = prevNode;
            prevNode->next = newNode;
            nextNode->prev = newNode;
        }
        ++size;
    }

    T& Get(int index) {
        return GetNode(index)->data;
    }

    int IndexOf(T& value) {
        DNode<T>* current = head;
        int index = 0;
        while (current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = static_cast<DNode<T>*>(current->next);
            ++index;
        }
        return -1;
    }

    T Remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range.");
        }

        DNode<T>* removedNode = nullptr;
        T removedData;

        if (index == 0) {
            removedNode = head;
            head = static_cast<DNode<T>*>(head->next);
            if (head != nullptr) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
        } else if (index == size - 1) {
            removedNode = tail;
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            removedNode = GetNode(index);
            DNode<T>* prevNode = removedNode->prev;
            DNode<T>* nextNode = static_cast<DNode<T>*>(removedNode->next);
            prevNode->next = nextNode;
            nextNode->prev = prevNode;
        }

        removedData = removedNode->data;
        delete removedNode;
        --size;

        return removedData;
    }

    ListIterator<T> Iterator() {
        return ListIterator<T>(head);
    }

    int Size() {
        return size;
    }
};


// EXERCISE 3
// Write a hash table class that maps keys to values. Define the following classes:

// The Hasher and EqualityPredicate classes should be base classes with pure virtual functions. 
// They can be overridden to provide concrete implementations of a hashing function and equality predicate function, respectively.

// Hasher Base Class
template <typename K>
class Hasher {
public:
    virtual size_t Hash(const K& key) const = 0; // Pure virtual function
    virtual ~Hasher() = default; // Virtual destructor
};

// EqualityPredicate Base Class
template <typename K>
class EqualityPredicate {
public:
    virtual bool Equals(const K& key1, const K& key2) const = 0; // Pure virtual function
    virtual ~EqualityPredicate() = default; // Virtual destructor
};

// Hashtable Class
template <typename K, typename V>
class Hashtable {
private:
    struct BucketItem {
        K key;
        V value;
    };

    std::vector<std::list<BucketItem>> buckets;
    size_t bucketCount;
    const Hasher<K>& hasher;
    const EqualityPredicate<K>& equalityPredicate;

    size_t GetBucketIndex(const K& key) const {
        return hasher.Hash(key) % bucketCount;
    }

public:
    Hashtable(size_t bucketCount, const Hasher<K>& hasher, const EqualityPredicate<K>& equalityPredicate)
        : bucketCount(bucketCount), hasher(hasher), equalityPredicate(equalityPredicate), buckets(bucketCount) {}

    void Put(const K& key, const V& value) {
        size_t bucketIndex = GetBucketIndex(key);
        auto& bucket = buckets[bucketIndex];
        for (auto& item : bucket) {
            if (equalityPredicate.Equals(item.key, key)) {
                item.value = value; // Update existing key
                return;
            }
        }
        // If this key is not in the Hashtable :
        bucket.emplace_back(BucketItem{key, value}); // Insert new key-value pair
    }

    V Get(const K& key) const {
        size_t bucketIndex = GetBucketIndex(key);
        const auto& bucket = buckets[bucketIndex];
        for (const auto& item : bucket) {
            if (equalityPredicate.Equals(item.key, key)) {
                return item.value;
            }
        }
        throw std::out_of_range("Key not found in hash table.");
    }

    bool ContainsKey(const K& key) const {
        size_t bucketIndex = GetBucketIndex(key);
        const auto& bucket = buckets[bucketIndex];
        for (const auto& item : bucket) {
            if (equalityPredicate.Equals(item.key, key)) {
                return true;
            }
        }
        return false;
    }

    void Remove(const K& key) {
        size_t bucketIndex = GetBucketIndex(key);
        auto& bucket = buckets[bucketIndex];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (equalityPredicate.Equals(it->key, key)) {
                bucket.erase(it);
                return;
            }
        }
        throw std::out_of_range("Key not found in hash table.");
    }
};

class IntHasher : public Hasher<int> {
public:
    size_t Hash(const int& key) const override {
        return static_cast<size_t>(key);
    }
};

class IntEqualityPredicate : public EqualityPredicate<int> {
public:
    bool Equals(const int& key1, const int& key2) const override {
        return key1 == key2;
    }
};

int main() {
    try {
        // Create a DoublyLinkedList instance
        DoublyLinkedList<int> dll;

        // Add elements to the list
        int a = 10, b = 20, c = 30, d = 40;
        dll.Add(a);
        dll.Add(b);
        dll.Add(c);
        std::cout << "After adding elements: ";
        for (auto it = dll.Iterator(); it.HasNext();) {
            std::cout << it.Next() << " ";
        }
        std::cout << std::endl;

        // Insert elements at specific positions
        int e = 25;
        dll.Insert(e, 2); // Insert 25 at index 2
        std::cout << "After inserting 25 at index 2: ";
        for (auto it = dll.Iterator(); it.HasNext();) {
            std::cout << it.Next() << " ";
        }
        std::cout << std::endl;

        // Get element by index
        std::cout << "Element at index 2: " << dll.Get(2) << std::endl;

        // IndexOf test
        std::cout << "Index of element 30: " << dll.IndexOf(c) << std::endl;

        // Remove an element
        int removed = dll.Remove(2);
        std::cout << "After removing element at index 2 (" << removed << "): ";
        for (auto it = dll.Iterator(); it.HasNext();) {
            std::cout << it.Next() << " ";
        }
        std::cout << std::endl;

        // Display size of the list
        std::cout << "Size of the list: " << dll.Size() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    IntHasher hasher;
    IntEqualityPredicate equalityPredicate;

    Hashtable<int, std::string> hashTable(10, hasher, equalityPredicate);

    // Insert key-value pairs
    hashTable.Put(1, "One");
    hashTable.Put(2, "Two");
    hashTable.Put(3, "Three");

    // Retrieve and print values
    std::cout << "Key 1: " << hashTable.Get(1) << std::endl;
    std::cout << "Key 2: " << hashTable.Get(2) << std::endl;

    // Check if a key exists
    std::cout << "Contains key 3: " << (hashTable.ContainsKey(3) ? "Yes" : "No") << std::endl;

    // Remove a key
    hashTable.Remove(2);
    std::cout << "Removed key 2." << std::endl;

    // Verify key removal
    std::cout << "Contains key 2: " << (hashTable.ContainsKey(2) ? "Yes" : "No") << std::endl;

    return 0;
}
