#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <MediaInfoDLL/MediaInfoDLL.h>

#include "include/base64.hpp"
#include "include/string.hpp"

#define RM(x) \
this->size -= (x); \
this->data += (x); \


static inline uint32_t U32_AT(const void* data) {
    uint32_t x;
    memcpy(&x, data, sizeof(x));
    #ifdef _WIN32
    return ((x & 0x000000FF) << 24)
        | ((x & 0x0000FF00) <<  8)
        | ((x & 0x00FF0000) >>  8)
        | ((x & 0xFF000000) >> 24);
    #else
    return __builtin_bswap32(x);
    #endif
}


#ifdef _WIN32
char* strndup(const char* array, size_t size) {
    char* buffer;
    size_t n;
    buffer = (char*)malloc(size +1);
    if (buffer) {
        for (n = 0; ((n < size) && (array[n] != 0)) ; n++) buffer[n] = array[n];
        buffer[n] = 0;
    }
    return buffer;
}
#endif


using namespace MediaInfoDLL;


class ArtworkExtractor: public MediaInfo {
public:
    std::string media = "";
    std::string output = "";
    bool isArtworkPresent = false;
    bool followMime = false;
    std::string mime = "";
    std::string type = "";
    std::string description = "";
    std::string artwork = "";
    uint8_t* data = nullptr;
    size_t size = 0;
    
    ArtworkExtractor(std::string media, std::string output, bool followMime = false): media(media), output(output), followMime(followMime) {
        this->Option("Cover_Data", "base64");
        this->Open(this->media);
        this->isArtworkPresent = this->Get(Stream_General, 0, "Cover") == "Yes";
        this->artwork = this->Get(Stream_General, 0, "Cover_Data");
        std::vector<uint8_t> artwork = base64_decode(this->artwork);
        this->data = new uint8_t[artwork.size()];
        memcpy(this->data, artwork.data(), artwork.size() * sizeof(uint8_t));
        this->size = artwork.size();
        if (
            Strings::split(Strings::toUpperCase(this->media), ".").back() == "OGG" ||
            Strings::split(Strings::toUpperCase(this->media), ".").back() == "FLAC"
        ) {
            size_t header = 0;
            uint32_t length;
            this->type = std::to_string(U32_AT(this->data));
            RM(4);
            length = U32_AT(this->data);
            header += length;
            RM(4);
            this->mime = strndup((const char*)this->data, length);
            RM(length);
            length = U32_AT(this->data);
            header += length;
            RM(4);
            this->description = strndup((const char*)this->data, length);
            RM(length);
            RM(4 * 4);
            length = U32_AT(this->data);
            RM(4);
            header += 32;
            this->size = length;
        }
        else {
            this->mime = this->Get(Stream_General, 0, "Cover_Mime");
            this->type = this->Get(Stream_General, 0, "Cover_Type");
            this->description = this->Get(Stream_General, 0, "Cover_Description");
        }
    }

    void info() {
        if (this->isArtworkPresent) {
            std::cout << "Artwork found.\n";
            std::cout << "Mime        : " << this->mime << "\n";
            std::cout << "Type        : " << this->type << "\n";
            std::cout << "Description : " << this->description << "\n";   
        }
        else {
            std::cout << "No artwork found.\n";
        }
    }

    void write() {
        if (this->isArtworkPresent) {
            if (this->followMime) {
                if (this->mime == "image/png") output += ".png";
                if (Strings::startsWith(this->mime, "image/jp")) output += ".jpg";
                else {
                    output += ".jpg";
                    std::cout << "No mime found. Using .jpg as extension.\n";
                }
            }
            std::fstream file(this->output, std::ios::out | std::ios::binary);
            file.write(
                (const char*)this->data,
                this->size
            );
            file.flush();
            file.close();
            std::cout << "Saved artwork to " << this->output << ".\n";
        }
    }
};


int32_t main(int argc, const char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    bool followMime = false;
    if (std::find(args.begin(), args.end(), "--follow-mime") != args.end()) {
        followMime = true;
    }
    if (args.size() < 3) {
        std::cout << "Invalid usage.\n";
        std::cout << "Example :\n";
        std::cout << args[0] << " media.mp3 output.png\n";
        return EXIT_FAILURE;
    }
    ArtworkExtractor extractor = ArtworkExtractor(args[1], args[2], followMime);
    extractor.info();
    extractor.write();    
    return EXIT_SUCCESS;
}
