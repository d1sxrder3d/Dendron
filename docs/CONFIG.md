# Config 

Detailed information about config settings

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

- true - files below directories
- false - files above directories


#### Ignore all files

#### `ignore_files = true/false`
- true - show only directories
- false - show files and directories


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

# default directory (unrecommended to change)
#### `default_directory = ./`

**Sets the default value for building the tree if the directory is not specified.**

