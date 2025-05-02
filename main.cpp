// main.cpp
#include <iostream>
#include <string>
#include <limits>
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
    std::cout << "8. Exit\n";
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

int main() {
    std::string filename;

    while (true) {
        // Prompt user for filename
        std::cout << "Enter MP3 filename: ";
        std::cin >> filename;

        // Create Mp3Tag object
        Mp3Tag mp3(filename);

        // Read tag from file
        if (!mp3.readTag()) {
            std::cerr << "Invalid ID3 tag or file could not be read.\n";
            continue;
        }

        std::cout << "File is OK and has a valid ID3v1.1 tag.\n";
        mp3.displayMetadata();

        bool keepEditing = true;
        while (keepEditing) {
            displayMenu();

            int choice;
            std::cin >> choice;
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
                        std::cin >> trackNum;
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
                        std::cin >> genreNum;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (genreNum >= 0 && genreNum < static_cast<int>(Mp3Tag::getGenreList().size())) {
                            success = mp3.updateGenre(static_cast<unsigned char>(genreNum));
                        } else {
                            std::cout << "Invalid genre number.\n";
                        }
                    }
                    break;
                case 8: // Exit
                    keepEditing = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    continue;
            }

            if (choice != 8) {
                if (success) {
                    std::cout << "Updated metadata:\n";
                    mp3.displayMetadata();
                } else if (choice >= 1 && choice <= 7) {
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
    }

    return 0;
}