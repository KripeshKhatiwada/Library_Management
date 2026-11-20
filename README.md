# Library Book Management System
- **Author:** Team: The Librarians — Dominic Wince, Kaden Jones, Kripesh Khatiwada, Aahmed Kashif
- **Course:** CSC 307 Data Structures and Algorithm Analysis
- **Semester:** Fall 2026

---

## 1. Prerequisites & Environment
Before running this project, ensure your system meets the following requirements:
- **Operating System:** Windows 11 (also compatible with Windows 10, macOS, or Linux)
- **Compiler:** GCC 16.2.0+ (`g++` via MSYS2 UCRT64 / MinGW-w64)
- **Build System:** `g++` CLI / VS Code Build Task (`.vscode/tasks.json`)

### Required Libraries
List any external frameworks used:
- **Standard C++17 Library (`<iostream>`, `<fstream>`, `<sstream>`, `<string>`, `<vector>`, `<iomanip>`, `<algorithm>`, `<cctype>`, `<functional>`):** Core CLI I/O, CSV parsing/serialization, string utilities, and formatting.

---

## 2. Installation & Setup
Provide step-by-step instructions on how to install dependencies.
1. Install **MSYS2** from `https://www.msys2.org/` (or via `winget install -e --id MSYS2.MSYS2` in Windows PowerShell).
2. Open the **MSYS2 UCRT64** terminal and install the GCC C++ compiler:
   ```bash
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc
   ```
3. Add `C:\msys64\ucrt64\bin` to your Windows **Environment Variables -> Path** (User or System variables), then restart VS Code / terminal.


---

## 3. Build Instructions
How do I compile the source code? Provide the exact commands.

**Using Command Line (from the `Library_Management` root folder):**
```bash
g++ -std=c++17 -Wall -Wextra -O2 -Isrc src/main.cpp src/book.cpp src/chainedhashtable.cpp src/libraryapp.cpp -o library_system.exe
```

**Using VS Code:**
- Open the `Library_Management` folder in Visual Studio Code.
- Save all open files (`Ctrl + K S`).
- Press `Ctrl + Shift + B` to run the default **Build Library System** task.

---

## 4. Execution Guide
Once built, how is the program started?
- **Executable Name:** `library_system.exe` (or `./library_system` on Linux/macOS)
- **Entry Point:** Run `.\library_system.exe data\books.csv` (or `.\library_system.exe` to default to `books.csv` in the current working directory) and use the interactive menu options (`1`–`8`).
- **Assets:** Run from the `Library_Management` project root so relative paths like `data\books.csv` resolve properly.

---

## 5. Troubleshooting / Known Issues
- If you see `'g++' is not recognized`, verify `C:\msys64\ucrt64\bin` is in your Windows `Path` and open a fresh terminal/VS Code window.
