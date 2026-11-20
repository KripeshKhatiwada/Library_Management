#ifndef LIBRARYAPP_H
#define LIBRARYAPP_H

#include "ChainedHashTable.h"
#include <string>

class LibraryApp {
private:
    ChainedHashTable table;
    std::string csvFileName;

    // Input validation & CSV helpers
    int readValidatedInt(const std::string& prompt, int minVal, int maxVal) const;
    std::string readValidatedString(const std::string& prompt) const;
    bool readValidatedBool(const std::string& prompt) const;
    std::string escapeCSV(const std::string& field) const;
    bool parseCSVLine(const std::string& line, Book& outBook) const;

public:
    explicit LibraryApp(const std::string& fileName = "books.csv");

    void run();
    void addBook();
    void searchBook() const;
    void updateBook();
    void removeBook();
    void displayInventory() const;
    bool saveToCSV() const;
    void loadFromCSV();
    void showMenu() const;
};

#endif // LIBRARYAPP_H