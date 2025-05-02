// mp3tag.h
#ifndef MP3TAG_H
#define MP3TAG_H

#include <string>
#include <fstream>
#include <vector>

class Mp3Tag {
public:
    // Constructor takes a filename
    explicit Mp3Tag(std::string  filename);

    // Check if file has a valid tag
    bool hasValidTag() const;

    // Read tag from file
    bool readTag();

    // Display tag information
    void displayMetadata() const;

    // Update specific fields
    bool updateComment(const std::string& newComment);
    bool updateTitle(const std::string& newTitle);
    bool updateArtist(const std::string& newArtist);
    bool updateAlbum(const std::string& newAlbum);
    bool updateYear(const std::string& newYear);
    bool updateTrack(unsigned char newTrack);
    bool updateGenre(unsigned char newGenre);

    // Getters
    std::string getTitle() const;
    std::string getArtist() const;
    std::string getAlbum() const;
    std::string getYear() const;
    std::string getComment() const;
    unsigned char getTrack() const;
    unsigned char getGenre() const;
    std::string getGenreString() const;

    // Static method to get genre list
    static const std::vector<std::string>& getGenreList();

private:
    // File handling
    std::string filename;
    mutable std::fstream file;
    bool openFile() const;

    // Tag structure
    struct ID3v1Tag {
        char tag[3];
        char title[30];
        char artist[30];
        char album[30];
        char year[4];
        char comment[28];
        char zeroByte;
        unsigned char track;
        unsigned char genre;
    };
    ID3v1Tag tag{};

    // Helper methods
    void writeTag();
    static std::string extractString(const char* field, size_t maxLength) ;
    static bool isValidMp3Filename(const std::string& name) ;

    // Genres list
    static const std::vector<std::string> genreList;
};

#endif // MP3TAG_H