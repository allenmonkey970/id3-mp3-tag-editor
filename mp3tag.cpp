// mp3tag.cpp
#include "mp3tag.h"
#include <iostream>
#include <cstring>
#include <algorithm>


// Initialize static genre list
const std::vector<std::string> Mp3Tag::genreList = {"Blues", "Classic Rock", "Country", "Dance",
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

Mp3Tag::Mp3Tag(std::string  filename) : filename(std::move(filename)) {
    std::memset(&tag, 0, sizeof(ID3v1Tag));
}

bool Mp3Tag::isValidMp3Filename(const std::string& name) {
    return name.size() > 4 && name.substr(name.size() - 4) == ".mp3";
}

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

bool Mp3Tag::hasValidTag() const {
    return std::strncmp(tag.tag, "TAG", 3) == 0;
}

void Mp3Tag::writeTag() {
    if (openFile()) {
        file.seekp(-128, std::ios::end);
        file.write(reinterpret_cast<char*>(&tag), sizeof(ID3v1Tag));
        file.close();
    }
}

std::string Mp3Tag::extractString(const char* field, size_t maxLength) {
    size_t len = std::min(maxLength, strlen(field));
    return {field, static_cast<std::string::size_type>(len)};
}

void Mp3Tag::displayMetadata() const {
    std::cout << "Title: " << getTitle() << "\n";
    std::cout << "Artist: " << getArtist() << "\n";
    std::cout << "Album: " << getAlbum() << "\n";
    std::cout << "Year: " << getYear() << "\n";
    std::cout << "Comment: " << getComment() << "\n";
    std::cout << "Track Number: " << static_cast<int>(getTrack()) << "\n";
    std::cout << "Genre: " << getGenreString() << "\n";
}

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

bool Mp3Tag::updateTrack(unsigned char newTrack) {
    tag.track = newTrack;
    writeTag();
    return true;
}

bool Mp3Tag::updateGenre(unsigned char newGenre) {
    if (newGenre >= genreList.size()) {
        std::cerr << "Invalid genre index.\n";
        return false;
    }

    tag.genre = newGenre;
    writeTag();
    return true;
}

std::string Mp3Tag::getTitle() const { return extractString(tag.title, 30); }
std::string Mp3Tag::getArtist() const { return extractString(tag.artist, 30); }
std::string Mp3Tag::getAlbum() const { return extractString(tag.album, 30); }
std::string Mp3Tag::getYear() const { return extractString(tag.year, 4); }
std::string Mp3Tag::getComment() const { return extractString(tag.comment, 28); }
unsigned char Mp3Tag::getTrack() const { return tag.track; }
unsigned char Mp3Tag::getGenre() const { return tag.genre; }

std::string Mp3Tag::getGenreString() const {
    if (tag.genre < genreList.size()) {
        return genreList[static_cast<std::vector<std::string>::size_type>(tag.genre)];
    }
    return "Unknown";
}

const std::vector<std::string>& Mp3Tag::getGenreList() {
    return genreList;
}