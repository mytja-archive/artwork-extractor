#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <MediaInfoDLL/MediaInfoDLL.h>

#include "include/base64.hpp"


int32_t main(int argc, const char** argv) {
    if (argc < 3) {
        std::cout << "Invalid usage.\n";
    }
    using namespace MediaInfoDLL;
    MediaInfo retriever = MediaInfo();
    retriever.Option("Cover_Data", "base64");
    retriever.Open(argv[1]);
    std::string mime = retriever.Get(Stream_General, 0, "Cover_Mime");
    std::string type = retriever.Get(Stream_General, 0, "Cover_Type");
    std::string description = retriever.Get(Stream_General, 0, "Cover_Description");
    std::cout << mime << std::endl;
    std::cout << type << std::endl;
    std::cout << description << std::endl;
    std::string artworkBase64 = retriever.Get(Stream_General, 0, "Cover_Data");
    std::vector<uint8_t> artwork = base64_decode(artworkBase64);

    return EXIT_SUCCESS;
}
