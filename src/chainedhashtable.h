#ifndef CHAINEDHASHTABLE_H
#define CHAINEDHASHTABLE_H

#include "hashnode.h"
#include <cstddef>
#include <functional>

class ChainedHashTable {
private:
    HashNode** buckets;
    size_t capacity;
    size_t size;
    float maxLoadFactor;

public:
    explicit ChainedHashTable(size_t initialCapacity = 16);
    ~ChainedHashTable();

    // Prevent accidental shallow copies of raw pointer array
    ChainedHashTable(const ChainedHashTable&) = delete;
    ChainedHashTable& operator=(const ChainedHashTable&) = delete;

    size_t hashFunction(int id) const;
    bool insert(const Book& book);
    Book* search(int id) const;
    bool update(int id, const Book& newBook);
    bool remove(int id);
    void displayAll() const;
    void resizeAndRehash();
    void clear();
    size_t getSize() const;

    // Helper for CSV serialization without exposing raw buckets
    void forEach(const std::function<void(const Book&)>& callback) const;
};

#endif // CHAINEDHASHTABLE_H