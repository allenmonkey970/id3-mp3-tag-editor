// main.cpp
#include <iostream>
#include <string>
#include "mp3tag.h"

int main() {
    std::string filename;
    
    while (true) {
        // Prompt user for filename
        std::cout << "Enter filename: ";
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
        
        // Update comment if user wants to
        std::cout << "Do you want to update the comment field (Y/N)? ";
        char response;
        std::cin >> response;
        
        if (toupper(response) == 'Y') {
            std::string newComment;
            std::cout << "Enter new comment (max 28 characters): ";
            std::cin.ignore();
            std::getline(std::cin, newComment);
            
            if (mp3.updateComment(newComment)) {
                std::cout << "Updated metadata:\n";
                mp3.displayMetadata();
            }
        }
        
        // Ask if user wants to edit another file
        std::cout << "Do you want to edit another file's tags (Y/N)? ";
        std::cin >> response;
        if (toupper(response) == 'N') {
            break;
        }
    }
    
    return 0;
}