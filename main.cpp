// main.cpp
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include "mp3tag.h"

void displayMenu() {
    std::cout << "\nEdit Options:\n";
    std::cout << "1. Edit Title\n";
    std::cout << "2. Edit Artist\n";
    std::cout << "3. Edit Album\n";
    std::cout << "4. Edit Year\n";
    std::cout << "5. Edit Comment\n";
    std::cout << "6. Edit Track Number\n";
    std::cout << "7. Edit Genre\n";
    std::cout << "8. Create New Tag\n";
    std::cout << "9. Rename File Based on Tags\n";
    std::cout << "10. Exit\n";
    std::cout << "Enter choice: ";
}

void displayGenres(const Mp3Tag& mp3) {
    const auto& genres = Mp3Tag::getGenreList();
    std::cout << "Available genres:\n";

    for (size_t i = 0; i < genres.size(); ++i) {
        std::cout << i << ": " << genres[static_cast<std::vector<std::string>::size_type>(i)];
        if ((i + 1) % 5 == 0) {
            std::cout << "\n";
        } else {
            std::cout << "  |  ";
        }
    }
    std::cout << "\nCurrent genre: " << mp3.getGenreString() << " (" << static_cast<int>(mp3.getGenre()) << ")\n";
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

int main() {
    std::string filename;

    while (true) {
        // Prompt user for filename
        std::cout << "Enter MP3 filename (or 'quit' to exit): ";
        std::cin >> filename;

        if (filename == "quit") {
            break;
        }

        // Check if the file has the correct extension
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".mp3") {
            std::cerr << "Error: File must have a .mp3 extension.\n";
            continue;
        }

        // Check if the file exists
        if (!fileExists(filename)) {
            std::cerr << "Error: File '" << filename << "' does not exist or cannot be read.\n";
            continue;
        }

        // Create Mp3Tag object
        Mp3Tag mp3(filename);

        // Read tag from file
        if (!mp3.readTag()) {
            std::cout << "No valid ID3v1 tag found in '" << filename << "'. Would you like to create one? (Y/N): ";
            char response;
            std::cin >> response;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (toupper(response) == 'Y') {
                if (mp3.createNewTag()) {
                    std::cout << "New empty tag created successfully.\n";
                } else {
                    std::cout << "Failed to create tag. Please try another file.\n";
                    continue;
                }
            } else {
                continue;
            }
        }

        std::cout << "File is OK and has a valid ID3v1.1 tag.\n";
        mp3.displayMetadata();

        bool keepEditing = true;
        while (keepEditing) {
            displayMenu();

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::string newValue;
            bool success = false;

            switch(choice) {
                case 1: // Title
                    std::cout << "Enter new title (max 30 characters): ";
                    std::getline(std::cin, newValue);
                    success = mp3.updateTitle(newValue);
                    break;
                case 2: // Artist
                    std::cout << "Enter new artist (max 30 characters): ";
                    std::getline(std::cin, newValue);
                    success = mp3.updateArtist(newValue);
                    break;
                case 3: // Album
                    std::cout << "Enter new album (max 30 characters): ";
                    std::getline(std::cin, newValue);
                    success = mp3.updateAlbum(newValue);
                    break;
                case 4: // Year
                    std::cout << "Enter new year (4 digits): ";
                    std::getline(std::cin, newValue);
                    success = mp3.updateYear(newValue);
                    break;
                case 5: // Comment
                    std::cout << "Enter new comment (max 28 characters): ";
                    std::getline(std::cin, newValue);
                    success = mp3.updateComment(newValue);
                    break;
                case 6: // Track
                    {
                        int trackNum;
                        std::cout << "Enter new track number (0-255): ";
                        if (!(std::cin >> trackNum)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (trackNum >= 0 && trackNum <= 255) {
                            success = mp3.updateTrack(static_cast<unsigned char>(trackNum));
                        } else {
                            std::cout << "Invalid track number. Must be between 0-255.\n";
                        }
                    }
                    break;
                case 7: // Genre
                    {
                        displayGenres(mp3);
                        int genreNum;
                        std::cout << "Enter genre number: ";
                        if (!(std::cin >> genreNum)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (genreNum >= 0 && genreNum < static_cast<int>(Mp3Tag::getGenreList().size())) {
                            success = mp3.updateGenre(static_cast<unsigned char>(genreNum));
                        } else {
                            std::cout << "Invalid genre number.\n";
                        }
                    }
                    break;
                case 8: // Create New Tag
                    if (!mp3.hasValidTag()) {
                        if (mp3.createNewTag()) {
                            std::cout << "New empty tag created successfully.\n";
                            mp3.displayMetadata();
                        } else {
                            std::cout << "Failed to create new tag.\n";
                        }
                    } else {
                        std::cout << "File already has a tag.\n";
                    }
                    break;
                case 9: // Rename File Based on Tags
                    {
                        std::string format;
                        std::cout << "Enter filename format (use %artist%, %title%, %album%, %year%, %track%):\n";
                        std::cout << "Example: %artist% - %title% [%year%]\n";
                        std::getline(std::cin, format);

                        if (mp3.renameFile(format)) {
                            std::cout << "File renamed successfully to: " << mp3.getFilename() << "\n";
                        } else {
                            std::cout << "Failed to rename file.\n";
                        }
                    }
                    break;
                case 10: // Exit
                    keepEditing = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    continue;
            }

            if (choice >= 1 && choice <= 7 && choice != 8) {
                if (success) {
                    std::cout << "Updated metadata:\n";
                    mp3.displayMetadata();
                } else {
                    std::cout << "Failed to update tag.\n";
                }
            }
        }

        // Ask if user wants to edit another file
        char response;
        std::cout << "Do you want to edit another file's tags (Y/N)? ";
        std::cin >> response;
        if (toupper(response) == 'N') {
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}