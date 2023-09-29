#include "Mp3.h"

void Mp3::Read(std::istream& stream) {
    header.Read(stream);
    while (!stream.eof()) {
        std::string identifier(kIdFrameLength, kFirstPos);
        stream.read(&identifier[kFirstPos], kIdFrameLength);
        if (identifier[kFirstPos] == kFirstPos) {
            return;
        } else {
            ID3v2Frame* frame = Factory(identifier);
            frame->Read(stream);
            frames.emplace_back(frame);
        }
    }
}

void Mp3::Write(std::ostream& stream) {
    header.Write(stream);
    for (auto frame: frames) {
        static size_t counter = kFirstPos;
        std::cout << "writing frame " << counter++ << '\n';
        frame->Write(stream);
    }
}

std::istream& operator>>(std::istream& stream, Mp3& mp3) {
    mp3.Read(stream);

    return stream;
};

std::ostream& operator<<(std::ostream& stream, Mp3& mp3) {
    mp3.Write(stream);

    return stream;
};