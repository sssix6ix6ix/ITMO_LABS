#ifndef LAB7_HEADER_H
#define LAB7_HEADER_H

#include "frames.h"

class ID3v2Header : public ID3v2Tag {
public:
    ID3v2Header() {
        identifier = new char[kIdHeadLength];
        version = new char[kVersionHeadLength];
        flags = new char[kFlagsHeadLength];
    }

    ~ID3v2Header() {
        delete[] identifier;
        delete[] version;
        delete[] flags;
        identifier = nullptr;
        version = nullptr;
        flags = nullptr;
    }

    void Read(std::istream& file) override;

    void Write(std::ostream& file) const;

private:
    void ReadIdentifier(std::istream& file) override;

    void ReadVersion(std::istream& file) override;

    void ReadSize(std::istream& file) override;

    void ReadFlags(std::istream& file) override;


};


#endif //LAB7_HEADER_H
