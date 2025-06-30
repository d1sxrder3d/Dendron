# Changelog

All notable changes to this project will be documented in this file.
<p align="center">
  <a href="docs/ru/CHANGELOG_ru.md">Read this in Russian (Русская версия)</a>
</p>

---

## [1.1.0] - 2025-07-01
### Added
- **Icon support**: Display icons for files and directories (requires a Nerd Font).
- **Clickable file links**: In supported terminals, file names become clickable.
- **File details**: Option to show file sizes and permissions with the `-d` flag. 
- **Flexible details format**: In `config.ini`, the option ["details_format"](docs/CONFIG.md#details_format) to customize the order and set of displayed details.
- **Performance Optimization**: Reduced the number of system calls to improve performance in verbose mode.


## [1.0.0] - 2025-06-29
### Added
- Initial release of Dendron
- Color scheme for different file types
- Configuration file in `~/.config/dendron/config.ini`: [watch configs](docs/CONFIG.md)
- Flag support: [watch flags](README.md#section_flags)
- installation and uninstallation via install.sh, uninstall.sh
- Make build system support
