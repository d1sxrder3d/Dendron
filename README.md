# treeCLI

![GitHub](https://img.shields.io/github/license/d1sxrder3d/Dendron)


A simple, cross-platform command-line utility to display a directory tree, written in C++17. An analogue of the `tree` utility in Linux.

## Features

- Display nested structure of files and directories.
- Set maximum recursion depth.
- Different sorting styles (e.g., files can be shown before directories).
- Cross-platform thanks to the C++17 `<filesystem>` standard library.

## Future Features

- **Colorized output**: Different colors for files and directories.
- **Pattern filtering**: Display only files matching a certain pattern (e.g., `*.cpp`).
- **Ignore paths**: Ability to ignore specific files or directories (similar to `.gitignore`).
- **File details**: Option to show file sizes and permissions.
- **Alternative output formats**: Export the tree structure to JSON or XML.

## Building

### Requirements

- A C++ compiler with C++17 support (e.g., GCC 9+, Clang 9+, MSVC 2019+).
- `make` build system (optional, for convenience).

<!-- ### Build Instructions

1.  Clone the repository:
    ```bash
    git clone https://github.com/YOUR_USERNAME/treeCLI.git
    cd treeCLI
    ```

2.  Compile the project. You can do this manually:
    ```bash
    g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp src/tree.cpp -o tree_cli
    ``` -->

<!-- ## Usage

Run the executable, specifying the path to a directory. If no path is provided, the current directory will be used.
 -->
