# ID3 MP3 Tag Editor
![License](https://img.shields.io/github/license/allenmonkey970/id3-mp3-tag-editor)
![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![Last Updated](https://img.shields.io/badge/last%20updated-2025--05--02-brightgreen)

A C++ application for viewing and editing ID3 metadata tags in MP3 files.

## Overview

ID3 MP3 Tag Editor is designed to help users view and modify metadata stored in MP3 files through ID3 tags.

## Features

- Support for reading and writing ID3v1
- Edit essential metadata fields such as:
  - Title
  - Artist
  - Album
  - Year
  - Genre
  - Comment
  - Track number
- Cross-platform compatibility

## Technical Details

- Written in C++ (99.4%)
- Uses CMake (0.6%) for build configuration

## Building the Project

### Prerequisites

- C++ compiler with C++11 support or newer
- CMake 3.10+
- Standard development tools (Git, etc.)

### Compilation Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/allenmonkey970/id3-mp3-tag-editor.git
   cd id3-mp3-tag-editor
   ```

2. Generate the build files with CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. Build the project:
   ```bash
   cmake --build .
   ```

## Usage

The application provides functionality to:

- Browse and select MP3 files from your system
- View current tag information
- Edit tag fields through the application interface
- Save modified tags back to the files
- Process multiple files when needed

## Supported ID3 Tags

The editor supports common ID3 tag fields including:

- Basic metadata (title, artist, album)
- Year of release
- Genre classification
- Comments
- Track and disc numbering
- Album artwork (for ID3v1.1)

## Contributing

Contributions to improve the ID3 MP3 Tag Editor are welcome:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/enhancement`)
3. Commit your changes (`git commit -m 'Add new feature'`)
4. Push to the branch (`git push origin feature/enhancement`)
5. Open a Pull Request

## License
[MIT](https://github.com/allenmonkey970/id3-mp3-tag-editor/blob/main/LICENSE)

## Contact

GitHub: [allenmonkey970](https://github.com/allenmonkey970)
