#include "ChainedHashTable.h"
#include <iomanip>
#include <iostream>
#include <string>

ChainedHashTable::ChainedHashTable(size_t initialCapacity)
    : capacity(initialCapacity > 0 ? initialCapacity : 16),
      size(0),
      maxLoadFactor(0.75f) {
    buckets = new HashNode*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }
}

ChainedHashTable::~ChainedHashTable() {
    clear();
    delete[] buckets;
    buckets = nullptr;
}

size_t ChainedHashTable::hashFunction(int id) const {
    // Polynomial rolling hash (Horner's method with prime 31)
    std::string keyStr = std::to_string(id);
    size_t hash = 0;
    const size_t prime = 31;
    for (char ch : keyStr) {
        hash = hash * prime + static_cast<size_t>(ch);
    }
    return hash % capacity;
}

bool ChainedHashTable::insert(const Book& book) {
    size_t index = hashFunction(book.getBookID());
    HashNode* current = buckets[index];

    // Enforce unique Book ID
    while (current != nullptr) {
        if (current->book.getBookID() == book.getBookID()) {
            return false;
        }
        current = current->next;
    }

    // Prepend new node at head of chain for O(1) insertion
    HashNode* newNode = new HashNode(book);
    newNode->next = buckets[index];
    buckets[index] = newNode;
    ++size;

    float loadFactor = static_cast<float>(size) / static_cast<float>(capacity);
    if (loadFactor > maxLoadFactor) {
        resizeAndRehash();
    }

    return true;
}

Book* ChainedHashTable::search(int id) const {
    size_t index = hashFunction(id);
    HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->book.getBookID() == id) {
            return &(current->book);
        }
        current = current->next;
    }
    return nullptr;
}

bool ChainedHashTable::update(int id, const Book& newBook) {
    size_t index = hashFunction(id);
    HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->book.getBookID() == id) {
            // Preserve original Book ID while updating metadata
            current->book.setTitle(newBook.getTitle());
            current->book.setAuthor(newBook.getAuthor());
            current->book.setPublicationYear(newBook.getPublicationYear());
            current->book.setAvailable(newBook.isAvailable());
            return true;
        }
        current = current->next;
    }
    return false;
}

bool ChainedHashTable::remove(int id) {
    size_t index = hashFunction(id);
    HashNode* current = buckets[index];
    HashNode* previous = nullptr;

    while (current != nullptr) {
        if (current->book.getBookID() == id) {
            if (previous == nullptr) {
                buckets[index] = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            --size;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

void ChainedHashTable::displayAll() const {
    if (size == 0) {
        std::cout << "\nNo books in inventory.\n";
        return;
    }

    std::cout << "\n=========================================================================================\n";
    std::cout << std::left
              << std::setw(10) << "Book ID"
              << std::setw(30) << "Title"
              << std::setw(24) << "Author"
              << std::setw(8)  << "Year"
              << std::setw(12) << "Status" << "\n";
    std::cout << "=========================================================================================\n";

    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            const Book& b = current->book;
            std::string titleDisplay = b.getTitle().length() > 28
                                           ? b.getTitle().substr(0, 25) + "..."
                                           : b.getTitle();
            std::string authorDisplay = b.getAuthor().length() > 22
                                            ? b.getAuthor().substr(0, 19) + "..."
                                            : b.getAuthor();

            std::cout << std::left
                      << std::setw(10) << b.getBookID()
                      << std::setw(30) << titleDisplay
                      << std::setw(24) << authorDisplay
                      << std::setw(8)  << b.getPublicationYear()
                      << std::setw(12) << (b.isAvailable() ? "Available" : "Checked Out")
                      << "\n";
            current = current->next;
        }
    }
    std::cout << "=========================================================================================\n";
    std::cout << "Total records: " << size << "\n";
}

void ChainedHashTable::resizeAndRehash() {
    size_t oldCapacity = capacity;
    HashNode** oldBuckets = buckets;

    capacity = oldCapacity * 2;
    buckets = new HashNode*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }

    // Re-link existing nodes without reallocating heap memory
    for (size_t i = 0; i < oldCapacity; ++i) {
        HashNode* current = oldBuckets[i];
        while (current != nullptr) {
            HashNode* nextNode = current->next;
            size_t newIndex = hashFunction(current->book.getBookID());
            current->next = buckets[newIndex];
            buckets[newIndex] = current;
            current = nextNode;
        }
    }

    delete[] oldBuckets;
}

void ChainedHashTable::clear() {
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            HashNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        buckets[i] = nullptr;
    }
    size = 0;
}

size_t ChainedHashTable::getSize() const {
    return size;
}

void ChainedHashTable::forEach(const std::function<void(const Book&)>& callback) const {
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            callback(current->book);
            current = current->next;
        }
    }
}