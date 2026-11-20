#include "LibraryApp.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

LibraryApp::LibraryApp(const std::string& fileName)
    : table(16), csvFileName(fileName) {}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

int LibraryApp::readValidatedInt(const std::string& prompt, int minVal, int maxVal) const {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cin.clear();
            continue;
        }
        line = trim(line);
        if (line.empty()) {
            std::cout << "  [Error] Input cannot be empty. Please enter a valid integer.\n";
            continue;
        }

        try {
            size_t pos = 0;
            int value = std::stoi(line, &pos);
            if (pos != line.size()) {
                std::cout << "  [Error] Invalid characters detected. Enter digits only.\n";
                continue;
            }
            if (value < minVal || value > maxVal) {
                std::cout << "  [Error] Value must be between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            return value;
        } catch (const std::exception&) {
            std::cout << "  [Error] Invalid number. Please try again.\n";
        }
    }
}

std::string LibraryApp::readValidatedString(const std::string& prompt) const {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cin.clear();
            continue;
        }
        line = trim(line);
        if (line.empty()) {
            std::cout << "  [Error] Input cannot be blank. Please try again.\n";
            continue;
        }
        return line;
    }
}

bool LibraryApp::readValidatedBool(const std::string& prompt) const {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cin.clear();
            continue;
        }
        line = trim(line);
        std::transform(line.begin(), line.end(), line.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        if (line == "y" || line == "yes" || line == "1" || line == "true") {
            return true;
        }
        if (line == "n" || line == "no" || line == "0" || line == "false") {
            return false;
        }
        std::cout << "  [Error] Please enter 'y' (yes) or 'n' (no).\n";
    }
}

void LibraryApp::showMenu() const {
    std::cout << "\n========================================\n"
              << "     LIBRARY BOOK MANAGEMENT SYSTEM     \n"
              << "========================================\n"
              << "  1. Add New Book\n"
              << "  2. Search Book by ID\n"
              << "  3. Update Book Information\n"
              << "  4. Remove Book\n"
              << "  5. Display All Inventory\n"
              << "  6. Save Records to CSV\n"
              << "  7. Reload Records from CSV\n"
              << "  8. Exit\n"
              << "========================================\n";
}

void LibraryApp::run() {
    loadFromCSV();

    bool running = true;
    while (running) {
        showMenu();
        int choice = readValidatedInt("Select an option (1-8): ", 1, 8);

        switch (choice) {
            case 1: addBook(); break;
            case 2: searchBook(); break;
            case 3: updateBook(); break;
            case 4: removeBook(); break;
            case 5: displayInventory(); break;
            case 6:
                if (saveToCSV()) {
                    std::cout << "  [Success] Inventory saved to '" << csvFileName << "'.\n";
                } else {
                    std::cout << "  [Error] Failed to write to '" << csvFileName << "'.\n";
                }
                break;
            case 7:
                loadFromCSV();
                break;
            case 8:
                if (saveToCSV()) {
                    std::cout << "  [Success] Changes auto-saved to '" << csvFileName << "'. Goodbye!\n";
                } else {
                    std::cout << "  [Warning] Could not auto-save to '" << csvFileName << "'. Goodbye!\n";
                }
                running = false;
                break;
        }
    }
}

void LibraryApp::addBook() {
    std::cout << "\n--- Add New Book ---\n";
    int id = readValidatedInt("Enter Book ID (positive integer): ", 1, 1000000000);

    if (table.search(id) != nullptr) {
        std::cout << "  [Error] A book with ID " << id << " already exists.\n";
        return;
    }

    std::string title = readValidatedString("Enter Book Title: ");
    std::string author = readValidatedString("Enter Author Name: ");
    int year = readValidatedInt("Enter Publication Year (1000-2026): ", 1000, 2026);
    bool available = readValidatedBool("Is the book available? (y/n): ");

    Book newBook(id, title, author, year, available);
    if (table.insert(newBook)) {
        std::cout << "  [Success] Book '" << title << "' (ID: " << id << ") added.\n";
    } else {
        std::cout << "  [Error] Could not insert book.\n";
    }
}

void LibraryApp::searchBook() const {
    std::cout << "\n--- Search Book ---\n";
    int id = readValidatedInt("Enter Book ID to search: ", 1, 1000000000);

    Book* found = table.search(id);
    if (found == nullptr) {
        std::cout << "  [Not Found] No book found with ID " << id << ".\n";
        return;
    }

    std::cout << "\n  Book Details:\n"
              << "  -------------------------------------\n"
              << "  Book ID          : " << found->getBookID() << "\n"
              << "  Title            : " << found->getTitle() << "\n"
              << "  Author           : " << found->getAuthor() << "\n"
              << "  Publication Year : " << found->getPublicationYear() << "\n"
              << "  Status           : " << (found->isAvailable() ? "Available" : "Checked Out") << "\n"
              << "  -------------------------------------\n";
}

void LibraryApp::updateBook() {
    std::cout << "\n--- Update Book ---\n";
    int id = readValidatedInt("Enter Book ID to update: ", 1, 1000000000);

    Book* existing = table.search(id);
    if (existing == nullptr) {
        std::cout << "  [Not Found] No book found with ID " << id << ".\n";
        return;
    }

    std::cout << "  Current Title: " << existing->getTitle()
              << " | Author: " << existing->getAuthor()
              << " | Year: " << existing->getPublicationYear()
              << " | Status: " << (existing->isAvailable() ? "Available" : "Checked Out") << "\n";

    std::string title = readValidatedString("Enter New Title: ");
    std::string author = readValidatedString("Enter New Author: ");
    int year = readValidatedInt("Enter New Publication Year (1000-2026): ", 1000, 2026);
    bool available = readValidatedBool("Is the book available? (y/n): ");

    Book updated(id, title, author, year, available);
    if (table.update(id, updated)) {
        std::cout << "  [Success] Book ID " << id << " updated.\n";
    } else {
        std::cout << "  [Error] Update failed.\n";
    }
}

void LibraryApp::removeBook() {
    std::cout << "\n--- Remove Book ---\n";
    int id = readValidatedInt("Enter Book ID to remove: ", 1, 1000000000);

    Book* existing = table.search(id);
    if (existing == nullptr) {
        std::cout << "  [Not Found] No book found with ID " << id << ".\n";
        return;
    }

    std::cout << "  Found: '" << existing->getTitle() << "' by " << existing->getAuthor() << "\n";
    if (!readValidatedBool("Are you sure you want to delete this record? (y/n): ")) {
        std::cout << "  [Cancelled] Deletion aborted.\n";
        return;
    }

    if (table.remove(id)) {
        std::cout << "  [Success] Book ID " << id << " removed from inventory.\n";
    } else {
        std::cout << "  [Error] Removal failed.\n";
    }
}

void LibraryApp::displayInventory() const {
    table.displayAll();
}

std::string LibraryApp::escapeCSV(const std::string& field) const {
    if (field.find_first_of(",\"\n\r") == std::string::npos) {
        return field;
    }
    std::string escaped = "\"";
    for (char ch : field) {
        if (ch == '"') escaped += "\"\"";
        else escaped += ch;
    }
    escaped += "\"";
    return escaped;
}

bool LibraryApp::parseCSVLine(const std::string& line, Book& outBook) const {
    std::vector<std::string> fields;
    std::string current;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (inQuotes) {
            if (ch == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    current += '"';
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                current += ch;
            }
        } else {
            if (ch == '"') {
                inQuotes = true;
            } else if (ch == ',') {
                fields.push_back(trim(current));
                current.clear();
            } else {
                current += ch;
            }
        }
    }
    fields.push_back(trim(current));

    if (fields.size() != 5) return false;

    try {
        int id = std::stoi(fields[0]);
        std::string title = fields[1];
        std::string author = fields[2];
        int year = std::stoi(fields[3]);

        std::string availStr = fields[4];
        std::transform(availStr.begin(), availStr.end(), availStr.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        bool available = (availStr == "1" || availStr == "true" || availStr == "available" || availStr == "yes");

        if (id <= 0 || title.empty() || author.empty() || year < 1000 || year > 2026) {
            return false;
        }

        outBook = Book(id, title, author, year, available);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool LibraryApp::saveToCSV() const {
    std::ofstream outFile(csvFileName);
    if (!outFile.is_open()) {
        return false;
    }

    outFile << "BookID,Title,Author,PublicationYear,Available\n";
    table.forEach([&](const Book& b) {
        outFile << b.getBookID() << ","
                << escapeCSV(b.getTitle()) << ","
                << escapeCSV(b.getAuthor()) << ","
                << b.getPublicationYear() << ","
                << (b.isAvailable() ? "1" : "0") << "\n";
    });

    return outFile.good();
}

void LibraryApp::loadFromCSV() {
    std::ifstream inFile(csvFileName);
    if (!inFile.is_open()) {
        std::cout << "  [Info] No existing '" << csvFileName << "' found. Starting with an empty inventory.\n";
        return;
    }

    table.clear();
    std::string line;
    size_t loadedCount = 0;
    size_t skippedCount = 0;
    bool isFirstLine = true;

    while (std::getline(inFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Skip header row if present
        if (isFirstLine) {
            isFirstLine = false;
            std::string lower = line;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            if (lower.find("bookid") != std::string::npos) {
                continue;
            }
        }

        Book parsed;
        if (parseCSVLine(line, parsed) && table.insert(parsed)) {
            ++loadedCount;
        } else {
            ++skippedCount;
        }
    }

    std::cout << "  [Info] Loaded " << loadedCount << " book(s) from '" << csvFileName << "'.";
    if (skippedCount > 0) {
        std::cout << " (" << skippedCount << " malformed/duplicate row(s) skipped.)";
    }
    std::cout << "\n";
}