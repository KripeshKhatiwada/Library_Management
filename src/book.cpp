#include "Book.h"

Book::Book()
    : bookID(0), title(""), author(""), publicationYear(0), available(true) {}

Book::Book(int id, const std::string& title, const std::string& author, int year, bool available)
    : bookID(id), title(title), author(author), publicationYear(year), available(available) {}

int Book::getBookID() const { return bookID; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
int Book::getPublicationYear() const { return publicationYear; }
bool Book::isAvailable() const { return available; }

void Book::setTitle(const std::string& newTitle) { title = newTitle; }
void Book::setAuthor(const std::string& newAuthor) { author = newAuthor; }
void Book::setPublicationYear(int year) { publicationYear = year; }
void Book::setAvailable(bool value) { available = value; }