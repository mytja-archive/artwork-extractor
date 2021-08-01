#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <MediaInfoDLL/MediaInfoDLL.h>

#include "include/base64.hpp"
#include "include/string.hpp"

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
    
    ArtworkExtractor(std::string media, std::string output, bool followMime = false): media(media), output(output), followMime(followMime) {
        this->Option("Cover_Data", "base64");
        this->Open(this->media);
        this->isArtworkPresent = this->Get(Stream_General, 0, "Cover") == "Yes";
        this->mime = this->Get(Stream_General, 0, "Cover_Mime");
        this->type = this->Get(Stream_General, 0, "Cover_Type");
        this->description = this->Get(Stream_General, 0, "Cover_Description");
        this->artwork = this->Get(Stream_General, 0, "Cover_Data");
    }

    void info() {
        if (this->isArtworkPresent) {
            std::cout << "Artwork found.\n";
            std::cout << "Artwork mime       : " << this->mime << "\n";
            std::cout << "Artwork type       : " << this->type << "\n";
            std::cout << "Artwork description: " << this->description << "\n";   
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
                else std::cout << "No mime found. Using .jpg as extension.\n";
            }
            std::vector<uint8_t> artwork = base64_decode(this->artwork);
            std::fstream file(this->output, std::ios::out | std::ios::binary);
            file.write(
                (const char*)artwork.data(),
                artwork.size()
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
        std::cout << "Example:\n";
        std::cout << args[0] << " media.mp3 output.png\n";
        return EXIT_FAILURE;
    }
    ArtworkExtractor extractor = ArtworkExtractor(args[1], args[2], followMime);
    extractor.info();
    extractor.write();    
    return EXIT_SUCCESS;
}
