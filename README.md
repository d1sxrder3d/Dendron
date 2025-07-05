# Dendron

<p align="center">
  <a href="https://github.com/d1sxrder3d/Dendron/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License">
  </a>
  <img src="https://img.shields.io/badge/language-C%2B%2B17-blue.svg" alt="Language C++17">
  <img src="https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg" alt="Platforms">
</p>

<p align="center">
  <a href="docs/ru/README_ru.md">Read this in Russian (Русская версия)</a>
</p>



**Dendron** is a blazingly fast, cross-platform, and simple command-line utility to display directory structures in a tree-like format. Written in modern C++17.

### Demo

Without icons:
```sh
$ dendron . -r 1 --iconsoff
.
├── .vscode
├── configs
├── src
├── .gitignore
├── install.sh
├── LICENSE
├── Makefile
└── README.md
```

With icons (requires a Nerd Font):
```sh
$ dendron . -r 1
.
├──  .vscode
├──  configs
├──  src
├──  .gitignore
├──  install.sh
├──  LICENSE
├──  Makefile
└──  README.md
```

## ✨ Features

### Current
- 🌳 **Nested structure display** of files and directories.
- 📏 **Recursion depth control** to manage output.
- 🎨 **Multiple display styles** (e.g., files before directories).
- 🌈 **Color schemes** for different file and directory types.
- 🖱️ **Clickable file links** in supported terminals.
- ✨ **Icon support** for files and directories (requires a Nerd Font).
- 🔍 **Pattern filtering** to exclude unwanted entries.
- ℹ️ **File details** Option to show file sizes and permissions.
- ↕️ **Flexible sorting** (files before or after directories).
- 📋 **Clipboard integration** Copy the output directly to the clipboard with `-c`/`--copy` or by default via config.
- 📤 **Alternative output formats** Export the tree structure to JSON.
- ⚙️ **Performance** High performance thanks to C++17

### Planned

- 💻 **Integration with git** Display git repositories and information about recent changes to files.

## 🚀 Getting Started

### Prerequisites
- A C++ compiler with C++17 support (e.g., GCC 9+, Clang 9+, MSVC 2019+).
- `make` build system (optional, for convenience).
- "Nerd fonts" for displaying icons (optional).

### Installation (Linux & macOS)

1.  Clone the repository:
    ```sh
    git clone https://github.com/d1sxrder3d/Dendron.git
    cd Dendron
    ```
2.  Run the installation script. It will build the project and install the binary to `/usr/local/bin`.
    ```sh
    ./install.sh
    ```
    *Note: The script may ask for your password to install the application system-wide (`/usr/local/bin`).*

### Manual Compilation
You can also compile the project manually:
```sh
g++ -std=c++17 -Wall -Wextra -g src/main.cpp src/tree.cpp -o dendron
```

## 💻 Usage

```
dendron [path] [options]
```
<h id="section_flags"></h>

| Option         | Long Option           | Description                                                              |
|----------------|-----------------------|--------------------------------------------------------------------------|
| `-r <depth>`   | `--recursion <depth>` | Set the maximum recursion depth.                                         |
| `-d`    | `--details`  | Show details of files and directories.     |
| `-t`           | `--tree`              | Sort files before directories.                                           |
| `-s <0-2>`     | `--style <0-2>`       | Display style. |
| `-i <pattern...>` | `--ignore <pattern...>`  | Ignore files/directories using the pattern. |
| `-f`           | `--files`             | Ignore files in output. |
| `-c`           | `--copy`              | Copy output to clipboard. |
| `-v`           | `--version`           | Show version. |
|| `--json [filename]`| Generate a JSON representation of the tree |
|                | `--iconsoff`          | Disable icons.             |
|                | `--config`            | Open configuration file.             |
| `-h`           | `--help`              | Show this help message.                                                   |

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
