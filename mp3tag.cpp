// mp3tag.cpp
#include "mp3tag.h"
#include <iostream>
#include <cstring>
#include <algorithm>

// Initialize static genre list
const std::vector<std::string> Mp3Tag::genreList = {
    "Blues", "Classic Rock", "Country", "Dance",
    "Disco", "Funk", "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age",
    "Oldies", "Other", "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno",
    "Industrial", "Alternative", "Ska", "Death Metal", "Pranks",
    "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal",
    "Jazz+Funk", "Fusion", "Trance", "Classical", "Instrumental", "Acid",
    "House", "Game", "Sound Clip", "Gospel", "Noise", "Alternative Rock",
    "Bass", "Soul", "Punk", "Space", "Meditative", "Instrumental Pop",
    "Instrumental Rock", "Ethnic", "Gothic", "Darkwave",
    "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream",
    "Southern Rock", "Comedy", "Cult", "Gangsta", "Top 40", "Christian Rap",
    "Pop/Funk", "Jungle", "Native American", "Cabaret", "New Wave",
    "Psychadelic", "Rave", "Showtunes", "Trailer", "Lo-Fi", "Tribal",
    "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll",
    "Hard Rock", "Folk", "Folk-Rock", "National Folk", "Swing",
    "Fast Fusion", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass",
    "Avantgarde", "Gothic Rock", "Progressive Rock", "Psychedelic Rock",
    "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", "Easy Listening",
    "Acoustic", "Humour", "Speech", "Chanson", "Opera", "Chamber Music",
    "Sonata", "Symphony", "Booty Bass", "Primus", "Porn Groove", "Satire",
    "Slow Jam", "Club", "Tango", "Samba", "Folklore", "Ballad",
    "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet", "Punk Rock",
    "Drum Solo", "Acapella", "Euro-House", "Dance Hall", "Goa",
    "Drum & Bass", "Club-House", "Hardcore", "Terror", "Indie", "BritPop",
    "Negerpunk", "Polsk Pumk", "Beat", "Christian Gangsta Rap",
    "Heavy Metal", "Black Metal", "Crossover", "Contemporary Christian",
    "Christian Rock", "Merengue", "Salsa", "Thrash Metal", "Anime", "JPop",
    "Synthpop"
};

// Constructor: Initializes filename and zeroes the tag structure
Mp3Tag::Mp3Tag(std::string  filename) : filename(std::move(filename)) {
    std::memset(&tag, 0, sizeof(ID3v1Tag));
}

// Checks if the provided filename ends with ".mp3"
bool Mp3Tag::isValidMp3Filename(const std::string& name) {
    return name.size() > 4 && name.substr(name.size() - 4) == ".mp3";
}

// Opens the file in binary read/write mode and checks extension
bool Mp3Tag::openFile() const {
    if (!isValidMp3Filename(filename)) {
        std::cerr << "Error: File must have a .mp3 extension.\n";
        return false;
    }

    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << "\n";
        return false;
    }
    return true;
}

// Reads the ID3v1 tag from the file (last 128 bytes)
bool Mp3Tag::readTag() {
    if (!openFile()) {
        return false;
    }

    file.seekg(-128, std::ios::end);
    file.read(reinterpret_cast<char*>(&tag), sizeof(ID3v1Tag));

    bool valid = hasValidTag();
    file.close();
    return valid;
}

// Checks if tag contains "TAG" identifier
bool Mp3Tag::hasValidTag() const {
    return std::strncmp(tag.tag, "TAG", 3) == 0;
}

// Writes the current tag to the last 128 bytes of the file
void Mp3Tag::writeTag() {
    if (openFile()) {
        file.seekp(-128, std::ios::end);
        file.write(reinterpret_cast<char*>(&tag), sizeof(ID3v1Tag));
        file.close();
    }
}

// Utility: Extracts a string from a char array up to maxLength or null terminator
std::string Mp3Tag::extractString(const char* field, size_t maxLength) {
    size_t len = std::min(maxLength, strlen(field));
    return {field, static_cast<std::string::size_type>(len)};
}

// Displays the metadata fields to the console
void Mp3Tag::displayMetadata() const {
    std::cout << "Title: " << getTitle() << "\n";
    std::cout << "Artist: " << getArtist() << "\n";
    std::cout << "Album: " << getAlbum() << "\n";
    std::cout << "Year: " << getYear() << "\n";
    std::cout << "Comment: " << getComment() << "\n";
    std::cout << "Track Number: " << static_cast<int>(getTrack()) << "\n";
    std::cout << "Genre: " << getGenreString() << "\n";
}

// Updates the comment field (max 28 chars), writes tag to file
bool Mp3Tag::updateComment(const std::string& newComment) {
    if (newComment.size() > 28) {
        std::cerr << "Comment is too long (max 28 characters).\n";
        return false;
    }

    std::memset(tag.comment, '\0', 28);
    std::strncpy(tag.comment, newComment.c_str(), 28);
    writeTag();
    return true;
}

// Updates the title field (max 30 chars), writes tag to file
bool Mp3Tag::updateTitle(const std::string& newTitle) {
    if (newTitle.size() > 30) {
        std::cerr << "Title is too long (max 30 characters).\n";
        return false;
    }

    std::memset(tag.title, '\0', 30);
    std::strncpy(tag.title, newTitle.c_str(), 30);
    writeTag();
    return true;
}

// Updates the artist field (max 30 chars), writes tag to file
bool Mp3Tag::updateArtist(const std::string& newArtist) {
    if (newArtist.size() > 30) {
        std::cerr << "Artist name is too long (max 30 characters).\n";
        return false;
    }

    std::memset(tag.artist, '\0', 30);
    std::strncpy(tag.artist, newArtist.c_str(), 30);
    writeTag();
    return true;
}

// Updates the album field (max 30 chars), writes tag to file
bool Mp3Tag::updateAlbum(const std::string& newAlbum) {
    if (newAlbum.size() > 30) {
        std::cerr << "Album name is too long (max 30 characters).\n";
        return false;
    }

    std::memset(tag.album, '\0', 30);
    std::strncpy(tag.album, newAlbum.c_str(), 30);
    writeTag();
    return true;
}

// Updates the year field (max 4 chars), writes tag to file
bool Mp3Tag::updateYear(const std::string& newYear) {
    if (newYear.size() > 4) {
        std::cerr << "Year is too long (max 4 characters).\n";
        return false;
    }

    std::memset(tag.year, '\0', 4);
    std::strncpy(tag.year, newYear.c_str(), 4);
    writeTag();
    return true;
}

// Updates the track number field, writes tag to file
bool Mp3Tag::updateTrack(unsigned char newTrack) {
    tag.track = newTrack;
    writeTag();
    return true;
}

// Updates the genre field, checks for valid index, writes tag to file
bool Mp3Tag::updateGenre(unsigned char newGenre) {
    if (newGenre >= genreList.size()) {
        std::cerr << "Invalid genre index.\n";
        return false;
    }

    tag.genre = newGenre;
    writeTag();
    return true;
}

// Creates a new, empty ID3v1 tag at the end of the file if not present
bool Mp3Tag::createNewTag() {
    if (!openFile()) {
        return false;
    }

    // Initialize a new empty tag with "TAG" identifier
    std::memset(&tag, 0, sizeof(ID3v1Tag));
    std::strncpy(tag.tag, "TAG", 3);

    // Check if file already has a tag
    file.seekg(-128, std::ios::end);
    char checkTag[3];
    file.read(checkTag, 3);
    if (std::strncmp(checkTag, "TAG", 3) == 0) {
        file.close();
        std::cerr << "File already has an ID3 tag.\n";
        return false;
    }

    // Write the new empty tag at the end of the file
    file.seekp(0, std::ios::end);
    file.write(reinterpret_cast<char*>(&tag), sizeof(ID3v1Tag));
    file.close();

    return true;
}

// Renames the file based on tag fields and a formatting string
bool Mp3Tag::renameFile(const std::string& format) {
    if (!hasValidTag()) {
        std::cerr << "Cannot rename file without valid tag information.\n";
        return false;
    }

    // Close the file if it's open
    if (file.is_open()) {
        file.close();
    }

    // Parse format string and create new filename
    std::string newFilename = format;

    // Replace placeholders with tag values
    size_t pos;
    while ((pos = newFilename.find("%artist%")) != std::string::npos) {
        newFilename.replace(pos, 8, getArtist());
    }
    while ((pos = newFilename.find("%title%")) != std::string::npos) {
        newFilename.replace(pos, 7, getTitle());
    }
    while ((pos = newFilename.find("%album%")) != std::string::npos) {
        newFilename.replace(pos, 7, getAlbum());
    }
    while ((pos = newFilename.find("%year%")) != std::string::npos) {
        newFilename.replace(pos, 6, getYear());
    }
    while ((pos = newFilename.find("%track%")) != std::string::npos) {
        // Format track number with leading zero if needed
        std::string trackStr = std::to_string(static_cast<int>(getTrack()));
        if (getTrack() < 10) {
            trackStr = "0" + trackStr;
        }
        newFilename.replace(pos, 7, trackStr);
    }

    // Sanitize filename (replace invalid characters)
    const std::string invalidChars = "\\/:*?\"<>|";
    for (char c : invalidChars) {
        newFilename.erase(std::remove(newFilename.begin(), newFilename.end(), c), newFilename.end());
    }

    // Ensure .mp3 extension
    if (newFilename.substr(newFilename.size() - 4) != ".mp3") {
        newFilename += ".mp3";
    }

    // Rename the file
    if (std::rename(filename.c_str(), newFilename.c_str()) != 0) {
        std::cerr << "Error renaming file.\n";
        return false;
    }

    // Update the filename in our object
    filename = newFilename;
    return true;
}

// Returns the current filename
std::string Mp3Tag::getFilename() const {
    return filename;
}

// Getter methods for tag fields
std::string Mp3Tag::getTitle() const { return extractString(tag.title, 30); }
std::string Mp3Tag::getArtist() const { return extractString(tag.artist, 30); }
std::string Mp3Tag::getAlbum() const { return extractString(tag.album, 30); }
std::string Mp3Tag::getYear() const { return extractString(tag.year, 4); }
std::string Mp3Tag::getComment() const { return extractString(tag.comment, 28); }
unsigned char Mp3Tag::getTrack() const { return tag.track; }
unsigned char Mp3Tag::getGenre() const { return tag.genre; }

// Returns the genre string for the current genre index
std::string Mp3Tag::getGenreString() const {
    if (tag.genre < genreList.size()) {
        return genreList[static_cast<std::vector<std::string>::size_type>(tag.genre)];
    }
    return "Unknown";
}

// Returns the genre list
const std::vector<std::string>& Mp3Tag::getGenreList() {
    return genreList;
}