# B+ Tree Disk-Based Visualization (CGI)

## 📖 About The Project

This project is a **Disk-Based B+ Tree** implementation written in **C++** that utilizes a **Common Gateway Interface (CGI)** to visualize the tree structure in a web browser.

Unlike a standard in-memory tree, this implementation persists data to a binary file (`tree.dat`), simulating how actual database systems (like MySQL or SQLite) store indexes on a hard drive using fixed-size pages (4096 bytes).

### Key Features

* **B+ Tree Structure:** Implements a B+ Tree of **Order 5**.
* **Disk Persistence:** Nodes are saved to `tree.dat`. If you restart the server, your data remains.
* **Web Interface:** A clear HTML interface to Insert and Search for keys.
* **Visual Debugging:** The C++ backend generates HTML to visually render the tree nodes, distinguishing between Internal Nodes (orange) and Leaf Nodes (green).

---

## 📂 File Structure

* **`index.html`**: The entry point. A web form to input values and select "Insert" or "Search".
* **`server.py`**: A Python script that acts as the web server. It handles the CGI requests and automatically opens your browser.
* **`common.h`**: Configuration file. Defines `Page Size`, `Tree Order`, and the `Node` structure.
* **`functions.h`**: Header file containing function declarations.
* **`storage.cpp`**: Handles low-level file I/O (reading/writing binary pages to disk).
* **`b+_tree.cpp`**: Contains the core B+ Tree logic (splitting nodes, finding leaves, inserting keys).
* **`main_cgi.cpp`**: The bridge between the web and C++. It reads URL parameters, executes tree operations, and outputs HTML code.

---

## ⚙️ Prerequisites

To run this project, you need:

1. **C++ Compiler**: GCC (MinGW for Windows) or Clang.
2. **Python 3.x**: To run the local server.

---

## 🚀 How to Setup and Run

### Step 1: Compile the C++ Code

Open your terminal/command prompt in the project root directory. Run the following command to compile the C++ files into the `cgi-bin` folder:

**For Windows (MinGW):**

```bash
g++ main_cgi.cpp storage.cpp dommy.cpp -o cgi-bin/tree_app.exe
```

**For Linux/Mac:**

```bash
g++ main_cgi.cpp storage.cpp dommy.cpp -o cgi-bin/tree_app.exe
```

### Step 2: Run the Server

In the terminal (still in the project root), run the Python server script:

```bash
python server.py
```

### Step 3: Use the Application

1. The script should automatically open `http://localhost:8080/index.html` in your default browser.
2. Enter a number (e.g., `10`) and click **Do**.
3. The page will reload showing the B+ Tree structure.
4. Keep inserting numbers (e.g., `20`, `5`, `15`, `30`) to see the tree split and grow.

---

### Results:

- A B+ tree web page with two functions `search` and `insert`:

![B+ tree controls](<B+ tree controls.png>)

- A simple visualization for the operation and the B+ tree:

![B+ tree visualization](<B+ tree visualization.png>)
---

## 🗑 Resetting the Data

To clear the database and start with an empty tree, simply delete the **`tree.dat`** file that appears in your project folder after running the app. A new one will be created automatically on the next insert.