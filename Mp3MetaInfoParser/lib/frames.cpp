#include "frames.h"

void ID3v2Frame::ReadIdentifier(std::istream& file) {
    file.read(identifier, kIdFrameLength);
}

void ID3v2Frame::ReadSize(std::istream& file) {
    char c;
    file.get(c);
    size += c;
    for (size_t i = kFirstPos; i < 3; i++) {

        file.get(c);
        size = size << kBitShift;
        size += c;
    }
}

void ID3v2Frame::ReadFlags(std::istream& file) {
    file.read(flags, kFlagsFrameLength);
}

void ID3v2Frame::ReadInfo(std::istream& file) {
    info = std::string(size, ' ');
    file.read(&info[kFirstPos], size);
}

void ID3v2Frame::ReadVersion(std::istream& file) {}

void ID3v2Frame::Read(std::istream& file) {
    ReadSize(file);
    ReadFlags(file);
    ReadInfo(file);
}

void ID3v2Frame::Write(std::ostream& file) {
    WriteFrameHeader(file);
}

void ID3v2Frame::WriteFrameHeader(std::ostream& file) {
    if (*identifier == '0') {
        std::cout << '\n' << "END";
        return;
    }
    std::cout << std::endl;
    file << "Identifier: ";
    file.write(identifier, kIdFrameLength);
    file << std::endl << "size :" << size << '\n';
    file << "Info : \n";
}

void ID3v2Frame::SetId(const std::string& id) {
    id.copy(identifier, kIdFrameLength);
}

void USLTFrame::Write(std::ostream& file) {
    ID3v2Frame::Write(file);
    size_t index = kFirstPos;
    file << "Text encoding : " << uint8_t(info[index++]);
    file << "\nLanguage : ";
    file.write(&info[index], kLanguageLength);
    index += kLanguageLength;
    file << "\nContent descriptor: ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    index++;
    file << "Lyrics : " << info.substr(index);
    file << kDescriptorOfFrame;
}

void SYLT::Write(std::ostream& file) {
    ID3v2Frame::Write(file);
    size_t index = kFirstPos;
    file << "Text encoding : " << uint8_t(info[index++]);
    file << "\nLanguage : ";
    file.write(&info[index], kLanguageLength);
    index += kLanguageLength;
    file << "\nTime stamp format : " << info[index++];
    file << "\nContent type : " << info[index++];
    file << "\nContent descriptor: ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    file << kDescriptorOfFrame;
}

void RVA2::Write(std::ostream& file) {
    ID3v2Frame::Write(file);
    std::string identification;

    size_t index = kFirstPos;

    while (info[index] != kFirstPos) {
        identification += info[index];
        ++index;
    }

    file << "Identification : " << identification << std::endl;
    ++index;

    for (size_t type_of_channel = kFirstPos; type_of_channel < kCountTypesOfChannel; ++type_of_channel) {

        file << "Type of channel : " << info[index] + '0' << std::endl;
        ++index;

        file << "Volume adjusment : ";
        file.write(&info[index], kVolumeLength);
        index += kVolumeLength;

        uint8_t bits_representing_peak = info[index];

        file << std::endl << "Bits representing peak : " << bits_representing_peak << std::endl;

        file << "Peak volume : ";
        file.write(&info[index], bits_representing_peak);
        file << std::endl;
    }
}

void EQU2::Write(std::ostream& file) {
    ID3v2Frame::Write(file);
    size_t index = kFirstPos;
    file << "Interpolation method : " << info[index++] << std::endl;

    std::string identification;
    while (info[index] != kFirstPos) {
        identification += info[index];
        ++index;
    }

    file << "Identification : " << identification << std::endl;
    ++index;

    file << "Frequency : " << info.substr(index, kFrequencyLength) << std::endl;
    index += kFrequencyLength;

    file << "Volume adjustment : " << info.substr(index, kVolumeLength) << std::endl;
    index += kVolumeLength;

    file << kDescriptorOfFrame;
}

void RVRB::Write(std::ostream& file) {
    ID3v2Frame::Write(file);
    size_t index = kFirstPos;

    file << "Reverb left : " << info.substr(index, kReverbLength) << std::endl;
    index += kReverbLength;

    file << "Reverb right : " << info.substr(index, kReverbLength) << std::endl;
    index += kReverbLength;

    file << "Reverb bounces, left : " << info[index++] << std::endl;
    file << "Reverb bounces, right : " << info[index++] << std::endl;
    file << "Reverb feedback, left to left : " << info[index++] << std::endl;
    file << "Reverb feedback, left to right : " <<  info[index++] << std::endl;
    file << "Reverb feedback, right to right : " << info[index++] << std::endl;
    file << "Reverb feedback, right to left : " << info[index++] << std::endl;
    file << "Premix left to right : " << info[index++] << std::endl;
    file << "Premix right to left : " << info[index++] << std::endl;

    file << kDescriptorOfFrame;
}

void PCNT::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "counter : " << info << std::endl;

    file << kDescriptorOfFrame;
}

void POPM::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    uint32_t index = kFirstPos;
    file << "Email to user : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "Rating : " << info[index] << std::endl;
    file << "Counter : ";
    file.write(&info[index], size - index);
    file << kDescriptorOfFrame;
}

void LINK::Write(std::ostream& file) {
    ID3v2Frame::Write(file);
    file << "Frame identifier : ";
    uint32_t index = kFirstPos;

    file.write(&info[index], 4);
    index += 4;
    file << std::endl << "URL : ";

    while (info[index] != kFirstPos) {
        file << info[index];
    }

    ++index;
    file << std::endl << "ID and additional data : ";
    file.write(&info[index], size - index);
    file << kDescriptorOfFrame;
}

void POSS::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "Time stamp format : " << (int8_t)info[kFirstPos] << std::endl;
    file << "Position : ";
    file.write(&info[kStartPosition], size - kStartPosition);
    file << kDescriptorOfFrame;
}

void USER::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "Text encoding : " << uint8_t(info[kFirstPos]) << std::endl;
    file << "Language : ";
    file.write(&info[kStartPosition], kLanguageLength);
    file << std::endl << "The actual text : ";
    file.write(&info[kActualTextIndex], size - kActualTextIndex);
    file << kDescriptorOfFrame;
}

void OWNE::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    uint32_t index = kFirstPos;
    file << "Text encoding : " << uint8_t(info[index++]) << std::endl;
    file << "Price paid : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "Date purch : ";
    file.write(&info[index], kDatePurchIndex);
    index += kDatePurchIndex;

    file << std::endl << "Seller : ";
    file.write(&info[index], size - index);
    file << kDescriptorOfFrame;
}

void COMR::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    uint32_t index = kFirstPos;
    file << "Text encoding : " << static_cast<uint8_t>(info[index++]) << std::endl;
    file << "Price string : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "Valid until : ";
    file.write(&info[index], kValidUntilLength);
    index += kValidUntilLength;
    file << std::endl << "Contact URL : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << " Received as : " << info[index] << std::endl;
    ++index;
    file << "Name of seller : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "Description : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "Picture MIME type : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "Seller logo : ";
    file.write(&info[index], size - index);

    file << kDescriptorOfFrame;
}

void ENCR::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "Owner identifier : " << info[kFirstPos] << std::endl;
    file << "Method symbol : " << info[kStartPosition] << std::endl;
    file << "Encryption data : ";
    file.write(&info[kEncryptionLength], size - kEncryptionLength);
    file << std::endl;
    file << kDescriptorOfFrame;
}

void GRID::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    uint32_t index = kFirstPos;
    file << "Owner identifier : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "Group symbol : " << info[index++] << std::endl;
    file << "Group dependent data : ";
    file.write(&info[index], size - index);
    file << kDescriptorOfFrame;
}

void PRIV::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    uint32_t index = kFirstPos;
    file << "Owner identifier : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file <<std::endl << "The private data : ";
    file.write(&info[index], size - index);
    file <<kDescriptorOfFrame;
}

void SIGN::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "Group symbol : " << info[kFirstPos] << std::endl;
    file << "Signature : ";
    file.write(&info[kStartPosition], size - kStartPosition);
    file << kDescriptorOfFrame;
}

void SEEK::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "Minimum offset to next tag : " << info;
    file << kDescriptorOfFrame;
}

void RBUF::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "Buffer size : ";
    file.write(&info[kFirstPos], kBufferSizeLength);
    file << std::endl << "Embedded info flag : " << (uint8_t)info[3] << std::endl;
    file << "Offset to next flag : ";
    file.write(&info[kOffsetToNextFlagLength], kOffsetToNextFlagLength);
    file << kDescriptorOfFrame;
}

void COMM::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    uint32_t index = kFirstPos;
    file << "Text encoding : " << uint8_t(info[index++]) << std::endl;
    file << "Language : ";
    file.write(&info[index], kLanguageLength);
    index += kLanguageLength;
    file << std::endl << "Short content decription : ";

    while (info[index] != kFirstPos) {
        file << info[index++];
    }

    ++index;
    file << std::endl << "The actual text : ";
    file.write(&info[index], size - index);
    file << kDescriptorOfFrame;
}

void TextFrame::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    for (char c : info) {
        if (std::isalpha(c)) {
            file << c;
        } else {
            file << ' ';
        }
    }
    file << kDescriptorOfFrame;
}

ID3v2Frame* Factory(const std::string& identifier) {
    ID3v2Frame* temp;

    if (identifier[kFirstPos] == 'T') {
        temp = new TextFrame;
    } else if (identifier[kFirstPos] == 'W') {
        temp = new URLFrame;
    } else if (identifier == "USLT") {
        temp = new USLTFrame;
    } else if (identifier == "SYLT") {
        temp = new SYLT;
    } else if (identifier == "RVA2") {
        temp = new RVA2;
    } else if (identifier == "EQU2") {
        temp = new EQU2;
    } else if (identifier == "RVRB") {
        temp = new RVRB;
    } else if (identifier == "PCNT") {
        temp = new PCNT;
    } else if (identifier == "POPM") {
        temp = new POPM;
    } else if (identifier == "RBUF") {
        temp = new RBUF;
    } else if (identifier == "LINK") {
        temp = new LINK;
    } else if (identifier == "POSS") {
        temp = new POSS;
    } else if (identifier == "USER") {
        temp = new USER;
    } else if (identifier == "OWNE") {
        temp = new OWNE;
    } else if (identifier == "COMR") {
        temp = new COMR;
    } else if (identifier == "ENCR") {
        temp = new ENCR;
    } else if (identifier == "GRID") {
        temp = new GRID;
    } else if (identifier == "PRIV") {
        temp = new PRIV;
    } else if (identifier == "SIGN") {
        temp = new SIGN;
    } else if (identifier == "SEEK") {
        temp = new SEEK;
    } else if (identifier == "COMM") {
        temp = new COMM;
    } else {
        temp = new ID3v2Frame;
    }

    temp->SetId(identifier);

    return temp;
}

void URLFrame::Write(std::ostream& file) {
    ID3v2Frame::Write(file);

    file << "URL : " << info;
    file << kDescriptorOfFrame;
}
