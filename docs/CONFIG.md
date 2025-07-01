# Config 

Detailed information about config settings

<p align="center">
  <a href="ru/CONFIG_ru.md">Read this in Russian (Русская версия)</a>
</p>


## Customization


#### Branch display styles

#### `branches_style = <0-2>`

**Sets the styles for displaying branches** 
(Default: 2)
- 0 - ╠═ ╚═ ║
- 1 - ┣━ ┗━ ┃
- 2 - ├── └── │


#### Tree display styles

#### `tree_style = true/false`

**Sets the tree display style.**
(Default: false)

- `true` - files are sorted before directories.
- `false` - directories are sorted before files.


#### Ignore all files

#### `ignore_files = true/false`
- true - show only directories
- false - show files and directories


#### Icons

To display icons correctly, you need to have a [Nerd Font](https://www.nerdfonts.com/) installed and selected in your terminal.

#### `active_icons = true/false`

**Enables or disables the display of icons.**
(Default: true)

- true - display icons
- false - do not display icons


#### `icon_* = <icon>`

**Sets a specific icon for a file type or directory.**

- `icon_dir` - icon for all directories.
- `icon_file` - default icon for all files that do not have a specific icon.
- `icon_<extension>` - icon for files with a specific extension. For example, `icon_cpp = ` sets the icon for all `.cpp` files.

You can add any extensions you need.



## Settings



#### Recursion depth

#### `max_recursion_depth = <value>`

**how deep does the utility penetrate into folders**

"-1" - disabled
(Setting a value less than -1 activates safe mode.)


#### Pattern to ignore
#### `ignore_patterns = *.git, main*, *vscode*` 

- *.git - all files and directories ending with .git will be ignored
- main* - all files and directories beginning with main will be ignored
- \*vscode* - all files and directories with substring "vscode" will be ignored


#### default directory (unrecommended to change)
#### `default_directory = ./`

**Sets the default value for building the tree if the directory is not specified.**


#### Whether to output files as hyperlinks
**Are files clickable?**
#### `show_hyperlinks = true/false`

- true - clickables
- false - not clickables


#### Show details by default
#### `show_details = true/false`

- `true` - show details.
- `false` - do not show details.


#### Whether to copy output to the clipboard by default
#### `copy_to_clipboard = true/false`

- `true` - always copy output to the clipboard. Equivalent to always using the `-c` or `--copy` flag.
- `false` - do not copy output by default.

(Default: `false`)


#### Details output format
#### `details_format = <format>`
**Defines which file details to display and in what order.**
(Default: `P, S, T`)

- `P` - permissions.
- `S` - size.
- `T` - last modification time.
- `L` - number of hard links (Unix-like).
- `U` - user/owner (Unix-like).
- `G` - group (Unix-like).

You can combine these characters in any order, e.g., `T, S, P` or `S, T`. Separators (spaces, commas) are ignored.
Example for a full output: `details_format = P,L,U,G,S,T`