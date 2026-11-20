#ifndef HASHNODE_H
#define HASHNODE_H

#include "Book.h"

class HashNode {
public:
    Book book;
    HashNode* next;

    explicit HashNode(const Book& book) : book(book), next(nullptr) {}
};

#endif // HASHNODE_H