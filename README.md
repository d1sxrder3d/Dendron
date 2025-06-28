# Dendron

<p align="center">
  <a href="https://github.com/d1sxrder3d/Dendron/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License">
  </a>
  <img src="https://img.shields.io/badge/language-C%2B%2B17-blue.svg" alt="Language C++17">
  <img src="https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg" alt="Platforms">
</p>

<p align="center">
  <a href="[docs/README_ru.md](https://github.com/d1sxrder3d/Dendron/blob/main/docs/README_ru.md)">Read this in Russian (Русская версия)</a>
</p>



**Dendron** is a blazingly fast, cross-platform, and simple command-line utility to display directory structures in a tree-like format. Written in modern C++17.

### Demo

```sh
$ dendron . -r 1 -s 0
.
╠═ .vscode
╠═ configs
╠═ src
╠═ .gitignore
╠═ install.sh
╠═ LICENSE
╠═ Makefile
╚═ README.md
```

## ✨ Features

### Current
- Display nested structure of files and directories.
- Control recursion depth.
- Different display styles (for example, files can be shown before directories).
- Different color schemes for files and directories.
- Pattern filtering.
- Sort files before directories.
- Cross-platform (Linux, macOS, Windows) thanks to C++17 `<filesystem>`.


### Planned
- ℹ️ **File details**: Option to show file sizes and permissions.
- 📤 **Alternative output formats**: Export the tree structure to JSON or XML.

## 🚀 Getting Started

### Prerequisites
- A C++ compiler with C++17 support (e.g., GCC 9+, Clang 9+, MSVC 2019+).
- `make` build system (optional, for convenience).

### Installation (Linux/macOS)

1.  Clone the repository:
    ```sh
    git clone https://github.com/d1sxrder3d/Dendron.git
    cd Dendron
    ```
2.  Run the installation script. It will build the project and install the binary to `/usr/local/bin`.
    ```sh
    ./install.sh
    ```
    *Note: The script may ask for your password to install the application system-wide.*

### Manual Compilation
You can also compile the project manually:
```sh
g++ -std=c++17 -Wall -Wextra -g src/main.cpp src/tree.cpp -o dendron
```

## 💻 Usage

```
dendron [path] [options]
```

| Option         | Long Option           | Description                                                              |
|----------------|-----------------------|--------------------------------------------------------------------------|
| `-d <path>`    | `--directory <path>`  | Specify the directory to display. Defaults to the current directory.     |
| `-r <depth>`   | `--recursion <depth>` | Set the maximum recursion depth.                                         |
| `-t`           | `--tree`              | Sort files before directories.                                           |
| `-s <0-2>`     | `--style <0-2>`       | Display style. |
| `-i <pattern>` | `--ignore <pattern>`  | Ignoring files/directories by pattern. |
| `-f`           | `--files`             | Disable display of files in the tree. |
| `-h`           | `--help`              | Show the help message.                                                   |

### Examples
1.  Display the tree for the current directory:
    ```sh
    dendron
    ```
2.  Display the tree for `/var/log` with a maximum depth of 2:
    ```sh
    dendron /var/log -r 2
    ```
3.  Display the tree for the current directory, showing files before directories:
    ```sh
    dendron . -t
    ```

## 🔧 Uninstallation
To uninstall Dendron, navigate to the cloned repository directory and run the uninstall script:
```sh
./uninstall.sh
```

## 📜 License
This project is licensed under the MIT License - see the LICENSE file for details.

