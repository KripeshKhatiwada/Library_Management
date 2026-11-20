#include "libraryApp.h"
#include <string>

int main(int argc, char* argv[]) {
    std::string csvPath = "books.csv";
    if (argc > 1) {
        csvPath = argv[1];
    }

    LibraryApp app(csvPath);
    app.run();
    return 0;
}