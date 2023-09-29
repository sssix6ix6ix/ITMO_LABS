#ifndef LAB7_MP3_H
#define LAB7_MP3_H

#include "Header.h"

#include <vector>

class Mp3 {
private:
    ID3v2Header header;
    std::vector<ID3v2Frame*> frames;

public:
    ~Mp3() {
        for (ID3v2Frame* frame : frames) {
            delete frame;
        }
    }

    void Read(std::istream& file);

    void Write(std::ostream& file);
};

std::istream& operator>>(std::istream& stream, Mp3& mp3);

std::ostream& operator<<(std::ostream& stream, Mp3& mp3);

#endif //LAB7_MP3_H
