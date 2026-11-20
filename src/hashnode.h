#ifndef HASHNODE_H
#define HASHNODE_H

#include "book.h"

class HashNode {
public:
    Book book;
    HashNode* next;

    explicit HashNode(const Book& book) : book(book), next(nullptr) {}
};

#endif // HASHNODE_H