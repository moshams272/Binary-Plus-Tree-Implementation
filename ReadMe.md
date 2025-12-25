# 🌳 Disk-Based B+ Tree Manager (CGI Project)

A C++ project to manage a B+ Tree structure stored on disk, featuring a Web Interface (HTML) connected via CGI (Common Gateway Interface).

## 📋 Prerequisites
Before you begin, ensure you have the following installed:

1.  **C++ Compiler (G++):** MinGW-w64 is recommended (via MSYS2).
2.  **Python:** Required to run the local CGI server.
3.  **Visual Studio Code:** Recommended code editor.
4.  **Git:** To clone the repository.

---

## 🚀 Setup & Installation

### 1. Clone the Repository
Open your terminal in the desired folder and run:
```bash
git clone <YOUR_REPO_URL_HERE>
cd <YOUR_PROJECT_FOLDER_NAME>

```

### 2. Verify Directory Structure

Ensure your files are organized exactly as follows for the server to work correctly:

```text
Project/
├── common.h            # Shared struct definitions
├── functions.h         # Function prototypes
├── index.html          # Frontend interface
├── server.py           # Python local server script
├── README.md           # This file
└── cgi-bin/            # ⚠️ CRITICAL FOLDER
    ├── main_cgi.cpp    # Frontend-to-Backend logic
    └── btree_dummy.cpp # (Or your actual logic files later)

```

---

## 🛠️ Compilation

**Important:** Browsers cannot run `.cpp` files directly. You must compile them into an executable inside the `cgi-bin` folder.

1. Open the Terminal in VS Code.
2. Navigate to the `cgi-bin` folder:
```bash
cd cgi-bin

```


3. Compile the interface code with the logic code:
```bash
g++ main_cgi.cpp btree_dummy.cpp -o tree_app.exe

```


*(Note: As you progress, replace `btree_dummy.cpp` with your actual implementation files, e.g., `storage.cpp` or `btree_logic.cpp`).*

---

## ▶️ Running the Application

1. Go back to the main project folder:
```bash
cd ..

```


2. Start the Python server:
```bash
python server.py

```


3. Your default browser should open automatically at `http://localhost:8080/index.html`.
4. Try performing an **Insert**, **Search**, or **Delete** operation.

---

## 👨‍💻 Team Workflow

* **Person 1 (Frontend):** Works on `index.html` and `main_cgi.cpp`.
* **Person 2 (Storage):** Creates and implements `storage.cpp` (File I/O).
* **Team (Logic):** Works on the B+ Tree logic files.

**⚠️ Integration Note:**
When a team member finishes a real function (e.g., the actual `insert` logic), remove the "Mock" function from `btree_dummy.cpp` and include the real `.cpp` file in the compilation command.

---

## 🔧 Troubleshooting

**1. Error: `fatal error: bits/requires_hosted.h**`
This means your G++ installation is missing some headers.

* Open **MSYS2 MinGW 64-bit** terminal.
* Run: `pacman -S mingw-w64-x86_64-toolchain`.

**2. Browser downloads a file instead of showing the page**

* Ensure `tree_app.exe` exists inside the `cgi-bin` folder.
* Ensure you are running the server using `python server.py`, not just opening the HTML file directly.

**3. How to stop the server?**

* Press `Ctrl + C` in the terminal where Python is running.
