#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    int bookID;
    std::string title;
    std::string author;
    int publicationYear;
    bool available;

public:
    Book();
    Book(int id, const std::string& title, const std::string& author, int year, bool available);

    int getBookID() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    int getPublicationYear() const;
    bool isAvailable() const;

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setPublicationYear(int year);
    void setAvailable(bool value);
};

#endif // BOOK_H