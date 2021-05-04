# Midnight Commander clone - object oriented programming classes project
My task was to create file manager inspired by iconic Midnight Commander which could be integrated with diffrent 'backends' (thats role for example is to handle user input and popup windows)

# Demo
![Demo](https://github.com/roman-oberenkowski/mc-mcedit/blob/74313e55938961810c96685dfac02d3d9d264826/readme_resources/demo.gif)

# Features
- Two independent windows
- Copy/Move files between directories open in two windows
- Remove files (non recursive mode for safety)
- Create empty file or new directory
- Simple Find files by name in current directory
- Clickable menu on top (thanks to mcedit backend)
- Simple batch mode to queue operations and then bulk execute

# Technical
- Requires C++17 (for std::filesystem to work)
- PDCURSES (integrated into project, but only in x86 configuration)

# Credits
All credits for 'mcedit inspired backend' that is used here go to:
https://github.com/Guslarz/mcedit



