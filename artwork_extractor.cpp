#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <MediaInfoDLL/MediaInfoDLL.h>

#include "include/base64.hpp"


using namespace MediaInfoDLL;


class ArtworkExtractor: public MediaInfo {
public:
    std::string media = "";
    std::string output = "";
    bool isArtworkPresent = false;
    std::string mime = "";
    std::string type = "";
    std::string description = "";
    std::string artwork = "";
    
    ArtworkExtractor(std::string media, std::string output): media(media), output(output) {
        this->Option("Cover_Data", "base64");
        this->Open(this->media);
        this->mime = this->Get(Stream_General, 0, "Cover_Mime");
        this->type = this->Get(Stream_General, 0, "Cover_Type");
        this->description = this->Get(Stream_General, 0, "Cover_Description");
        this->artwork = this->Get(Stream_General, 0, "Cover_Data");
    }

    void info() {
        std::cout << "Artwork mime       : " << this->mime << "\n";
        std::cout << "Artwork type       : " << this->type << "\n";
        std::cout << "Artwork description: " << this->description << "\n";
    }

    void write() {
        std::vector<uint8_t> artwork = base64_decode(this->artwork);
        std::fstream file(this->output, std::ios::out | std::ios::binary);
        file.write(
            (const char*)artwork.data(),
            artwork.size()
        );
        file.flush();
        file.close();
    }
};


int32_t main(int argc, const char** argv) {
    if (argc < 3) {
        std::cout << "Invalid usage.\n";
        std::cout << "Example:\n";
        std::cout << argv[0] << " media.mp3 output.png\n";
        return EXIT_FAILURE;
    }
    ArtworkExtractor extractor = ArtworkExtractor(argv[1], argv[2]);
    extractor.info();
    extractor.write();    
    return EXIT_SUCCESS;
}
