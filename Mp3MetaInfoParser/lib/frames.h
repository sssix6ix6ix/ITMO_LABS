#ifndef LAB7_FRAMES_H
#define LAB7_FRAMES_H

#include <bitset>
#include <iostream>
#include <string>
#include <fstream>

constexpr size_t kIdHeadLength = 3;
constexpr size_t kVersionHeadLength = 2;
constexpr size_t kFlagsHeadLength = 1;
constexpr size_t kSizeHeadLength = 4;

constexpr size_t kIdFrameLength = 4;
constexpr size_t kFlagsFrameLength = 2;
constexpr size_t kSizeFrameLength = 4;

constexpr size_t kFirstPos = 0;
constexpr size_t kStartPosition = 1;
constexpr size_t kVolumeLength = 2;
constexpr size_t kFrequencyLength = 2;
constexpr size_t kReverbLength = 2;
constexpr size_t kEncryptionLength = 2;
constexpr size_t kLanguageLength = 3;
constexpr size_t kBufferSizeLength = 3;
constexpr size_t kActualTextIndex = 4;
constexpr size_t kOffsetToNextFlagLength = 4;
constexpr size_t kBitShift = 7;
constexpr size_t kDatePurchIndex = 8;
constexpr size_t kValidUntilLength = 8;
constexpr size_t kCountTypesOfChannel = 9;

constexpr char kDescriptorOfFrame[] = "\n---------------------------\n";

class ID3v2Tag {
protected:
    char* identifier{}; // max 4 byte
    char* version{}; // max 3 byte
    char* flags{}; // max 2 byte
    uint32_t size{}; // max 4 byte
    std::string info;

    virtual void ReadIdentifier(std::istream& file) = 0;
    virtual void ReadSize(std::istream& file) = 0;
    virtual void ReadFlags(std::istream& file) = 0;
    virtual void ReadVersion(std::istream& file) = 0;
    virtual void Read(std::istream& file) = 0;

};

class ID3v2Frame : public ID3v2Tag {
public:
    ID3v2Frame() {
        identifier = new char[kIdFrameLength];
        flags = new char[kFlagsFrameLength];
        size = 0;
    }

    virtual ~ID3v2Frame() {
        delete[] identifier;
        identifier = nullptr;
        delete[] flags;
        flags = nullptr;
    }

    void Read(std::istream& file) override;

    virtual void Write(std::ostream& file);

    void SetId(const std::string& id);

protected:
    void ReadIdentifier(std::istream& file) override;

    void ReadSize(std::istream& file) override;

    void ReadVersion(std::istream& file) override;

    void ReadFlags(std::istream& file) override;

    void ReadInfo(std::istream& file);

    void WriteFrameHeader(std::ostream& file);
};


class TextFrame : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class URLFrame : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class USLTFrame : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class SYLT : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class RVA2 : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class EQU2 : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class RVRB : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class PCNT : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class POPM : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class RBUF : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class LINK : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class POSS : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class USER : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class OWNE : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class COMR : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class ENCR : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class GRID : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class PRIV : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class SIGN : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class SEEK : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

class COMM : public ID3v2Frame {
    void Write(std::ostream& file) override;
};

ID3v2Frame* Factory(const std::string& identifier);

#endif //LAB7_FRAMES_H
